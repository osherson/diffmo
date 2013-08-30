#include "Analysis/JetAnalysis/interface/JetStudies2011.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <sstream>
#include "TRandom.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/Math/interface/deltaR.h"


using namespace std;



JetStudies2011::JetStudies2011(const edm::ParameterSet& iConfig, TFileDirectory& iDir) :
  edm::BasicAnalyzer(iConfig,iDir),
  theDir(iDir),
  jetSrc_        ( iConfig.getParameter<edm::InputTag>("jetSrc")),
  rhoSrc_        ( iConfig.getParameter<edm::InputTag>("rhoSrc")),
  pvSrc_         ( iConfig.getParameter<edm::InputTag>("pvSrc")),
  trigSrc_       ( iConfig.getParameter<edm::InputTag>("trigSrc")),
  genJetsSrc_    ( iConfig.getParameter<edm::InputTag>("genJetsSrc")),
  useCA8GenJets_ ( iConfig.getParameter<bool>         ("useCA8GenJets")),
  weightPV_      ( iConfig.getParameter<bool>         ("weightPV")),
  jecPayloads_   ( iConfig.getParameter<std::vector<std::string> >  ("jecPayloads")),
  trigs_         ( iConfig.getParameter<std::vector<std::string> >("trigs") ),
  binPtTrig_     ( iConfig.getParameter<bool>         ("binPtTrig") ),
  ptTrigBins_    ( iConfig.getParameter<std::vector<double> > ("ptTrigBins") ),
  useBTags_      ( iConfig.getParameter<bool>         ("useBTags") ),
  orderByMass_   ( iConfig.getParameter<bool>         ("orderByMass") )
{

  std::cout << "Hello from jet studies!" << std::endl;

  // Get the factorized jet corrector. 
  // The payloads contain N elements, 
  // the Nth is the uncertainty, and the first N-1 elements are the
  // actual correction levels. 
  vector<JetCorrectorParameters> vPar;
  for ( std::vector<std::string>::const_iterator ipayload = jecPayloads_.begin(),
	  ipayloadEnd = jecPayloads_.end(); ipayload != ipayloadEnd - 1; ++ipayload ) {
    std::cout << "Adding payload " << *ipayload << std::endl;
    JetCorrectorParameters pars(*ipayload);
    vPar.push_back(pars);
  }
  
  jec_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );
  
  jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecPayloads_.back()));


  if ( useBTags_ ) {
    muonSrc_ = iConfig.getParameter<edm::InputTag>("muonSrc");
    rCut_    = iConfig.getParameter<double>       ("rCut");
    pfMuonSelector_ = 
      boost::shared_ptr<PFMuonSelector>( new PFMuonSelector(iConfig.getParameter<edm::ParameterSet>( "muonInJetSelector" )) );
  } else {
    rCut_ = 0.0;
  }

  // if ( weightPV_ ) {
  //   lumiWeighting_ = 
  //     boost::shared_ptr<edm::LumiWeighting>( new edm::LumiWeighting( iConfig.getParameter<edm::ParameterSet>("lumiWeighting").getParameter<std::string>("generatedFile"),
  // 								     iConfig.getParameter<edm::ParameterSet>("lumiWeighting").getParameter<std::string>("dataFile") ) );
  // }

  theDir.make<TH1F>( "nPV",   ";N_{Primary Vertex}", 25, 0, 25 );
  theDir.make<TH1F>( "nPVRewightedNPV", ";N_{Primary Vertex}", 25, 0, 25 );  
  theDir.make<TH1F>( "nPVRewightedPtHat", ";N_{Primary Vertex}", 25, 0, 25 );

  for ( std::vector<std::string>::const_iterator itrig = trigs_.begin(),
	  itrigEnd = trigs_.end();
	itrig != itrigEnd; ++itrig ) {
    dirs_.push_back( theDir.mkdir( *itrig ) );
    dirs_.back().make<TH1F>( "jetPt",       "Jet p_{T};p_{T} (GeV/c)",    100, 0, 1000);
    dirs_.back().make<TH1F>( "jetArea",     "Jet Area;Area (radians)",               100, 0, TMath::Pi() );
    dirs_.back().make<TH1F>( "jetEta",      "Jet #eta;#eta (radians)",               100, -5.0, 5.0 );    
    dirs_.back().make<TH1F>( "jetPhi",      "Jet #phi;#phi (radians)",               100, -TMath::Pi(), TMath::Pi() );
    dirs_.back().make<TH1F>( "jetMass",     "Jet Mass;Mass (GeV/c^{2})",             100, 0, 200. );
    dirs_.back().make<TH2F>( "jetMassVsPt", "Jet Mass versus p_{T};p_{T} (GeV/c);Mass (GeV/c^{2})",   20, 0, 1000, 20, 0, 200. );
  }
  dirs_.push_back( theDir.mkdir( "MC") );
  dirs_.back().make<TH1F>( "jetPt",       "Jet p_{T};p_{T} (GeV/c)",    100, 0, 1000);
  dirs_.back().make<TH1F>( "jetArea",     "Jet Area;Area (radians)",               100, 0, TMath::Pi() );
  dirs_.back().make<TH1F>( "jetEta",      "Jet #eta;#eta (radians)",               100, -5.0, 5.0 );    
  dirs_.back().make<TH1F>( "jetPhi",      "Jet #phi;#phi (radians)",               100, -TMath::Pi(), TMath::Pi() );
  dirs_.back().make<TH1F>( "jetMass",     "Jet Mass;Mass (GeV/c^{2})",             100, 0, 200. );
  dirs_.back().make<TH2F>( "jetMassVsPt", "Jet Mass versus p_{T};p_{T} (GeV/c);Mass (GeV/c^{2})",   20, 0, 1000, 20, 0, 200. );
  dirs_.back().make<TH2F>( "jetRes",      "Jet Response;Response",          25,  0, 1000., 25., 0., 2.5 );
            
}


///////////////////
/// The event loop
//////////////////
void JetStudies2011::analyze(const edm::EventBase& iEvent)
{

  // Get the PV's to make plots versus NPV
  edm::Handle<std::vector<reco::Vertex> > h_pv;

  iEvent.getByLabel( pvSrc_, h_pv );
  theDir.getObject<TH1>( "nPV" )->Fill( h_pv->size() );
  if ( h_pv->size() == 0 ) 
    return;

  // Get the jets, the rho variable, and (optionally) the triggers.
  edm::Handle<std::vector<pat::Jet> > h_jets;
  edm::Handle<double> h_rho;
  edm::Handle<pat::TriggerEvent> h_trig;

  double weightMC = 1.0;
  bool passed = false;
  std::vector<std::pair<unsigned int, unsigned int> > fired;
  // For real data, break up the plots by trigger path
  if ( iEvent.isRealData() ) {
    iEvent.getByLabel( trigSrc_,h_trig );
    // This expects the HLT trigger list to be "sorted" by priority
    for ( std::vector<std::string>::const_iterator itrigBegin = trigs_.begin(),
	    itrigEnd = trigs_.end(), itrig = itrigBegin;
	  itrig != itrigEnd; ++itrig ) {
      if ( h_trig->wasRun() && h_trig->wasAccept() && h_trig->paths() > 0 ) {
	int indexPath = h_trig->indexPath( *itrig );
	if ( indexPath > 0 ) {
	  pat::TriggerPath const * path = h_trig->path( *itrig );
	  if ( path != 0 && path->wasRun() && path->wasAccept() ) {
	    passed = true;
	    fired.push_back( std::make_pair<unsigned int, unsigned int>( itrig - itrigBegin, path->prescale() ) );
	  }
	}
      }
    }
  }
  // For MC, just plot everything
  else {
    passed = true;
    // if ( weightPV_ ) {
    //   weightMC *= lumiWeighting_->weight( h_pv->size() );
    //   theDir.getObject<TH1>( "nPVRewightedNPV" )->Fill( h_pv->size(), weightMC );
    // }
    edm::Handle<GenEventInfoProduct>    genEvt;
    iEvent.getByLabel( edm::InputTag("generator"),  genEvt );
    if( genEvt.isValid() )  {
      weightMC *= genEvt->weight() ;
    }
    theDir.getObject<TH1>( "nPVRewightedPtHat" )->Fill( h_pv->size(), weightMC );
  }

  if ( !passed ) 
    return;

  // Require exactly 2 jets
  iEvent.getByLabel( jetSrc_, h_jets );
  iEvent.getByLabel( rhoSrc_, h_rho );

  if ( h_jets->size() != 2 ) 
    return;

  double rho = *h_rho;

  int index0 = 0, index1 = 1;

  // Order by mass instead of pt
  if ( orderByMass_ ) {
    if ( h_jets->at(0).mass() < h_jets->at(1).mass() ) {
      index0 = 1;
      index1 = 0;
    }
  }
  

  pat::Jet const & jet0 = h_jets->at(index0);
  pat::Jet const & jet1 = h_jets->at(index1);

  // get a copy of the uncorrected p4
  reco::Candidate::LorentzVector uncorrJet0 = jet0.correctedP4(0);
  reco::Candidate::LorentzVector uncorrJet1 = jet1.correctedP4(0);

  // Then get the correction (L1+L2+L3 [+L2L3 for data])
  jec_->setJetEta( uncorrJet0.eta() );
  jec_->setJetPt ( uncorrJet0.pt() );
  jec_->setJetE  ( uncorrJet0.energy() );
  jec_->setJetA  ( jet0.jetArea() );
  jec_->setRho   ( *(h_rho.product()) );
  jec_->setNPV   ( h_pv->size() );

  double corr0 = jec_->getCorrection();

  jec_->setJetEta( uncorrJet1.eta() );
  jec_->setJetPt ( uncorrJet1.pt() );
  jec_->setJetE  ( uncorrJet1.energy() );
  jec_->setJetA  ( jet0.jetArea() );
  jec_->setRho   ( *(h_rho.product()) );
  jec_->setNPV   ( h_pv->size() );

  double corr1 = jec_->getCorrection();

  // Here will be the working variable for all the jet energy effects
  reco::Candidate::LorentzVector scaledJet0P4 = uncorrJet0 * corr0;
  reco::Candidate::LorentzVector scaledJet1P4 = uncorrJet1 * corr1;


  if ( reco::deltaR<reco::Candidate::LorentzVector, reco::Candidate::LorentzVector> ( scaledJet0P4, scaledJet1P4 ) < TMath::Pi() / 2.0 ) {
    return;
  }


  // Optionally check that both jets satisfy
  // "muon-in-jet" OR "SSVHEM"
  if ( useBTags_ ) {
    edm::Handle<std::vector<pat::Muon> > h_muons;
    iEvent.getByLabel( muonSrc_, h_muons );
    bool tagged0 = false;
    bool tagged1 = false;
    if ( jet0.bDiscriminator("simpleSecondaryVertexHighEffBJetTags") > 2.74 ) tagged0 = true;
    if ( jet1.bDiscriminator("simpleSecondaryVertexHighEffBJetTags") > 2.74 ) tagged1 = true;

    // for ( std::vector<pat::Muon>::const_iterator imu = h_muons->begin(),
    // 	    imuEnd = h_muons->end(); imu != imuEnd; ++imu ) {
    //   bool passedCuts = (*pfMuonSelector_)( *imu );
    //   if ( passedCuts ) {
    // 	if ( deltaR<reco::Candidate, reco::Candidate>(*imu, jet0) < rCut_ ) {
    // 	  tagged0 = true;
    // 	}
    // 	if ( deltaR<reco::Candidate, reco::Candidate>(*imu, jet1) < rCut_ ) {
    // 	  tagged1 = true;
    // 	}
    //   }
    // }
    if ( (!tagged0 || !tagged1) )
      return;
  }
  

  // Now get the jet pt's and correct
  double pt0 = scaledJet0P4.pt();
  double area0 = jet0.jetArea();
  double mass0 = scaledJet0P4.mass();
  double eta0  = scaledJet0P4.eta();
  double phi0  = scaledJet0P4.phi();


  // check to see if we're binning in pt
  int trigToPlot = -1;
  if ( binPtTrig_ ) {
    std::vector<double>::const_iterator found = lower_bound( ptTrigBins_.begin(), ptTrigBins_.end(), scaledJet0P4.pt() );
    trigToPlot = found - ptTrigBins_.begin() - 1;
  }



  // double pt1 = jet1.pt();
  // double area1 = jet1.jetArea();
  // double mass1 = jet1.mass();
  // double eta1  = jet1.eta();
  // double phi1  = jet1.phi();

  if ( iEvent.isRealData() ) {    
    for ( std::vector<std::pair<unsigned int, unsigned int> >::const_iterator ifired = fired.begin(),
	    ifiredBegin = fired.begin(),
	    ifiredEnd = fired.end();
	  ifired != ifiredEnd; ++ifired ) { 

      // If we're binning in pt bins by trigger, ignore pt's outside of "this" trigger's range
      if ( trigToPlot >= 0 && ifired->first != static_cast<unsigned int>(trigToPlot) ) 
	continue;

   
      dirs_.at( ifired->first ).getObject<TH1>( "jetArea"      )->Fill( area0, weightMC * ifired->second );
      dirs_.at( ifired->first ).getObject<TH1>( "jetPt"        )->Fill( pt0, weightMC * ifired->second );
      dirs_.at( ifired->first ).getObject<TH1>( "jetEta"       )->Fill( eta0, weightMC * ifired->second );
      dirs_.at( ifired->first ).getObject<TH1>( "jetPhi"       )->Fill( phi0, weightMC * ifired->second );
      dirs_.at( ifired->first ).getObject<TH1>( "jetMass"      )->Fill( mass0, weightMC * ifired->second );
      static_cast<TH2*>(dirs_.at( ifired->first ).getObject<TH1>( "jetMassVsPt" ))->Fill( pt0, mass0, weightMC * ifired->second );
    }
  } else {
    double ptGen0 = 0.0;
    if ( useCA8GenJets_ ) {
      edm::Handle<std::vector<reco::GenJet> > h_genJets;
      iEvent.getByLabel( genJetsSrc_, h_genJets );
      
      for ( std::vector<reco::GenJet>::const_iterator igenBegin = h_genJets->begin(),
	      igenEnd = h_genJets->end(), igen = igenBegin;
	    igen != igenEnd; ++igen ) {
	if ( reco::deltaR<pat::Jet,reco::GenJet>( jet0, *igen ) < 0.8 ) {
	  ptGen0 = igen->pt();
	  break;
	}
      }
    } else {
      reco::GenJet const * igen = jet0.genJet();
      if ( igen > 0 )
	ptGen0 = jet0.genJet()->pt();
    }
    if ( ptGen0 > 0.0 ) {
      dirs_.back().getObject<TH1>( "jetArea"      )->Fill( area0, weightMC );
      dirs_.back().getObject<TH1>( "jetPt"        )->Fill( pt0, weightMC );
      dirs_.back().getObject<TH1>( "jetEta"       )->Fill( eta0, weightMC );
      dirs_.back().getObject<TH1>( "jetPhi"       )->Fill( phi0, weightMC );
      dirs_.back().getObject<TH1>( "jetMass"      )->Fill( mass0, weightMC );
      static_cast<TH2*>(dirs_.back().getObject<TH1>( "jetMassVsPt" ))->Fill( pt0, mass0, weightMC );
      static_cast<TH2*>(dirs_.back().getObject<TH1>( "jetRes"      ))->Fill( ptGen0, pt0/ptGen0, weightMC );
    }
  }

}
