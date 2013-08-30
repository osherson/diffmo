#include "Analysis/BoostedTopAnalysis/interface/Type12Analysis.h"
#include "DataFormats/Math/interface/deltaR.h"

Type12Analysis::Type12Analysis( const edm::ParameterSet & iConfig,  TFileDirectory & iDir )  :
  theDir( iDir ),
  pfJetIdParams_         (iConfig.getParameter<edm::ParameterSet>("pfJetIDParams") ),
  pfJetSel_              (new PFJetIDSelectionFunctor(pfJetIdParams_)),
  bTagOP_                (iConfig.getParameter<double>("bTagOP") )
{
  cout  <<"Instantiated from Type12Analysis"  << endl;
  histograms1d["nBs"]               = theDir.make<TH1F>("nBs",      "Number of B Jets",   10,   -0.5,   9.5 );
  histograms1d["bJetPtType1"]       = theDir.make<TH1F>("bJetPtType1",    "bJet pT from Type1",   200,    0,  1000  );
  histograms1d["dRBs"]              = theDir.make<TH1F>("dRBs",     "#delta R of bJets",  50,   0.,     6.0 );
  histograms1d["diJetMass"]         = theDir.make<TH1F>("diJetMass",  "Dijet Mass from type2",    200,    0,  1000  );
  histograms1d["diJetMassPred"]     = theDir.make<TH1F>("diJetMassPred",  "Dijet Mass Bkg",       200,    0,  1000  );

  mistagFile_     =   TFile::Open( "mistag.root" );
  if( bTagOP_ < 3 )
    bMistag_         =   (TH1F*) mistagFile_->Get("bTagLoosePt");
  if( bTagOP_ > 3 )
    bMistag_         =   (TH1F*) mistagFile_->Get("bTagTightPt");


}

void Type12Analysis::analyze( const edm::EventBase & iEvent )
{
  edm::Handle<vector<pat::Jet>  >   jetHandle;
  iEvent.getByLabel(  edm::InputTag("selectedPatJetsPFlow"),  jetHandle );

  std::vector<edm::Ptr<pat::Jet> >        allPfJets;
  std::vector<edm::Ptr<pat::Jet> >        bJets0;
  std::vector<edm::Ptr<pat::Jet> >        bJets1;
  std::vector<edm::Ptr<pat::Jet> >        notags0;
  std::vector<edm::Ptr<pat::Jet> >        notags1;

  pat::strbitset retPFJet = pfJetSel_->getBitTemplate();

  for( vector<pat::Jet>::const_iterator jetBegin=jetHandle->begin(), jetEnd=jetHandle->end(), ijet=jetBegin ;
    ijet!=jetEnd; ijet++ )  
  {
    if( ijet->pt() > 30.0 && fabs( ijet->eta() ) < 2.4 ) {
      retPFJet.set(false);
      bool passJetID = (*pfJetSel_)( *ijet, retPFJet );
      if( passJetID )
        allPfJets.push_back( edm::Ptr<pat::Jet>(jetHandle, ijet-jetBegin )  );
    }
  }

  if( allPfJets.size() != 3 )    return;
  pat::Jet const &  leadJet = (*allPfJets.at(0));
  
  for( size_t i=0;  i<allPfJets.size(); i++ ) {
    double dPhi_  = fabs( reco::deltaPhi<double>( leadJet.phi(),  allPfJets.at(i)->phi() ) );
    bool btagged = (allPfJets.at(i)->bDiscriminator("trackCountingHighEffBJetTags") > bTagOP_ );
    if( dPhi_ < TMath::Pi()/2 ) {
      if( btagged )     bJets0.push_back( allPfJets.at(i) );
      else              notags0.push_back( allPfJets.at(i) );
    }
    else  {
      if( btagged )     bJets1.push_back( allPfJets.at(i) );
      else              notags1.push_back( allPfJets.at(i) );
    }
  }

  histograms1d["nBs"]       ->    Fill( bJets0.size() + bJets1.size() );

  if( bJets0.size() == 1 && bJets1.size() ==2 ) {
    histograms1d["bJetPtType1"]       ->    Fill( bJets0.at(0)->pt() );
    double dR = reco::deltaR<double>( bJets1.at(0)->eta(),  bJets1.at(0)->phi(),  bJets1.at(1)->eta(), bJets1.at(1)->phi() );
    double mass = (bJets1.at(0)->p4()+bJets1.at(1)->p4()).mass();
    histograms1d["dRBs"]              ->    Fill( dR );
    histograms1d["diJetMass"]         ->    Fill( mass );
  }

  if( bJets1.size() == 1 && bJets0.size() ==2 ) {
    histograms1d["bJetPtType1"]       ->    Fill( bJets1.at(0)->pt() );
    double dR = reco::deltaR<double>( bJets0.at(0)->eta(),  bJets0.at(0)->phi(),  bJets0.at(1)->eta(), bJets0.at(1)->phi() );
    double mass = (bJets0.at(0)->p4()+bJets0.at(1)->p4()).mass();
    histograms1d["dRBs"]              ->    Fill( dR );
    histograms1d["diJetMass"]         ->    Fill( mass );
  }

  if( notags0.size() == 1 && bJets1.size() == 2 ) {
    double pt = notags0.at(0)->pt();
    double bin =  bMistag_->FindBin(pt);
    double weight = bMistag_->GetBinContent(bin);
    double mass = (bJets1.at(0)->p4()+bJets1.at(1)->p4()).mass();
    histograms1d["diJetMassPred"]     ->    Fill( mass, weight );
  }

  if( notags1.size() == 1 && bJets0.size() == 2 ) {
    double pt = notags1.at(0)->pt();
    double bin = bMistag_->FindBin(pt);
    double weight = bMistag_->GetBinContent(bin);
    double mass = (bJets0.at(0)->p4()+bJets0.at(1)->p4()).mass();
    histograms1d["diJetMassPred"]     ->    Fill( mass, weight );
  }

} 


