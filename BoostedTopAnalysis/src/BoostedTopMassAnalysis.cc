#include "Analysis/BoostedTopAnalysis/interface/BoostedTopMassAnalysis.h"
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"

using namespace std;


BoostedTopMassAnalysis::BoostedTopMassAnalysis(const edm::ParameterSet& iConfig, TFileDirectory& iDir) :
  shyftSelector_(iConfig.getParameter<edm::ParameterSet>("shyftSelection")),
  boostedTopWTagFunctor_(iConfig.getParameter<edm::ParameterSet>("hadronicWParams")),
  theDir(iDir)
{

  theDir.make<TH1F>( "jetPt_L2L3",  "L2+L3 Correction;Jet p_{T} (GeV/c)",    100, 0, 1000);
  theDir.make<TH1F>( "jetPt_L1L2L3","L1+L2+L3 Correction;Jet p_{T} (GeV/c)", 100, 0, 1000);
  theDir.make<TH1F>( "jetArea",     "Jet Area;Area (radians)",               100, 0, TMath::Pi() );
  theDir.make<TH1F>( "jetEta",      "Jet #eta;#eta (radians)",               100, -5.0, 5.0 );    
  theDir.make<TH1F>( "jetPhi",      "Jet #phi;#phi (radians)",               100, -TMath::Pi(), TMath::Pi() );
  theDir.make<TH1F>( "jetMass",     "Jet Mass;Mass (GeV/c^{2})",             100, 0, 200. );  
  theDir.make<TH1F>( "jetMu",       "Jet Mass Drop;#frac{m_{0}}{m_{jet}}",   100, 0, 1.0 );  
  theDir.make<TH1F>( "jetMassCut",  "Jet Mass After Cut;Mass (GeV/c^{2})",   100, 0, 200. );  

  semilepRet_ = shyftSelector_.getBitTemplate();
  passAllSemilep_ = pat::strbitset::index_type(&semilepRet_, std::string(">=1 Jets"));
  wtagRet_ = boostedTopWTagFunctor_.getBitTemplate();
}


template<typename T>
struct GreaterByMass {
  typedef T first_argument_type;
  typedef T second_argument_type;
  bool operator()( const T & t1, const T & t2 ) const {
    return t1->mass() > t2->mass();
  }
};

template<typename T>
struct LessByMass {
  typedef T first_argument_type;
  typedef T second_argument_type;
  bool operator()( const T & t1, const T & t2 ) const {
    return t1->mass() < t2->mass();
  }
};



void BoostedTopMassAnalysis::analyze(const edm::EventBase& iEvent)
{

  semilepRet_.set(false);
  wtagRet_.set(false);

  shyftSelector_(iEvent, semilepRet_);

  if ( semilepRet_[passAllSemilep_] ) {

    SHyFTPFSelector::collection_type const & jets      = shyftSelector_.selectedJets      ();
    SHyFTPFSelector::collection_type const & muons     = shyftSelector_.selectedTightMuons     ();
    SHyFTPFSelector::collection_type const & electrons = shyftSelector_.selectedTightElectrons ();
    SHyFTPFSelector::value_type      const & met       = shyftSelector_.selectedMET       ();

    typedef std::vector<SHyFTPFSelector::const_iterator> candidate_const_iterator_collection;
    
    // Classify jets as "leptonic side" or "hadronic side" depending on a
    // hemicylinder requirement to the lepton. 
    candidate_const_iterator_collection jetsLep;
    candidate_const_iterator_collection jetsHad;
    for ( SHyFTPFSelector::const_iterator ijetBegin = jets.begin(),
	    ijetEnd = jets.end(), ijet = ijetBegin;
	  ijet != ijetEnd; ++ijet ) {
      // Get the lepton
      reco::ShallowClonePtrCandidate const * theLepton = 0;
      if ( muons.size() > 0 ) {
	theLepton = &(muons[0]);
      } else if ( electrons.size() > 0 ) {
	theLepton = &(electrons[0]);
      }
      // Classify the jets into the vectors for leptonic or hadronic side
      if ( reco::deltaPhi<double>( theLepton->phi(), ijet->phi() ) < TMath::Pi() / 2.0 ) {
	jetsLep.push_back( ijet );
      }// end if leptonic-side jet
      else {
	jetsHad.push_back( ijet );
      }// end if hadronic-side jet
    }// end loop over jets
 
    // The idea is that the hadronic-side jet contains a W-jet. Order the list wrt mass.
    std::sort( jetsHad.begin(), jetsHad.end(),
	       GreaterByMass<SHyFTPFSelector::const_iterator> () );
    for ( candidate_const_iterator_collection::const_iterator iptrJet = jetsHad.begin(),
	    iendPtrJet = jetsHad.end(); iptrJet != iendPtrJet; ++iptrJet ) {
      pat::Jet const * patJet = dynamic_cast<pat::Jet const *>( (**iptrJet).masterClonePtr().get() );
      reco::PFJet const * da0 = dynamic_cast<reco::PFJet const *>(patJet->daughter(0));
      reco::PFJet const * da1 = dynamic_cast<reco::PFJet const *>(patJet->daughter(1));
      if ( da0 == 0 || da1 == 0 ) {
	std::cout << " PROBLEM WITH JET!" << std::endl;
      }
      else {
	double y, mu, dR;
	pat::subjetHelper( *patJet,
			   y,
			   mu,
			   dR );
	theDir.getObject<TH1>( "jetPt_L2L3" )->Fill ( patJet->pt() );
	theDir.getObject<TH1>( "jetEta"     )->Fill ( patJet->eta() );
	theDir.getObject<TH1>( "jetPhi"     )->Fill ( patJet->phi() );
	theDir.getObject<TH1>( "jetMass"    )->Fill ( patJet->mass() );
	theDir.getObject<TH1>( "jetMu"      )->Fill ( mu );
	if ( mu < 0.25 ) {
	  theDir.getObject<TH1>( "jetMassCut"    )->Fill ( patJet->mass() );	  
	} 
      }

    }

  }// end if SHyFT PF selection passed

  
}// end of function

