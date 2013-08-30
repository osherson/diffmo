#include "Analysis/JetAnalysis/interface/MultijetAnalyzer.h"


#include <vector>
#include <iterator>

MultijetAnalyzer::MultijetAnalyzer(const edm::ParameterSet& iConfig, TFileDirectory& iDir) :
  caloSelector( iConfig.getParameter<edm::ParameterSet>("jetIDSelector"),
		iConfig.getParameter<edm::ParameterSet>("pfJetIDSelector"),
		iConfig.getParameter<edm::ParameterSet>("jetStudies") ),
  pfSelector( iConfig.getParameter<edm::ParameterSet>("jetIDSelector"),
	      iConfig.getParameter<edm::ParameterSet>("pfJetIDSelector"),
	      iConfig.getParameter<edm::ParameterSet>("pfJetStudies") ),
  theDir(iDir)
{


  caloSelector.set("Calo Delta Phi", false);
  caloSelector.set("PF Delta Phi", false);
  pfSelector.set("Calo Delta Phi", false);
  pfSelector.set("PF Delta Phi", false);


  edm::ParameterSet plotParameters = iConfig.getParameter<edm::ParameterSet>("plotParameters");
  doTracks = plotParameters.getParameter<bool>("doTracks");
  useMC    = plotParameters.getParameter<bool>("useMC");






  hists["hist_nJet"                     ] = theDir.make<TH1D>( "hist_nJet"                    ,"Number of Calo Jets", 10, 0, 10 ) ;
  hists["hist_nPFJet"                   ] = theDir.make<TH1D>( "hist_nPFJet"                  ,"Number of PF Jets", 10, 0, 10 ) ;
  hists["hist_hT"                       ] = theDir.make<TH1D>( "hist_hT"                      ,"H_{T}, Calo Jets", 400, 0, 400 );
  hists["hist_pf_hT"                    ] = theDir.make<TH1D>( "hist_pf_hT"                   ,"H_{T}, PF Jets", 400, 0, 400 );

  hists["hist_jetPt"                    ] = theDir.make<TH1D>( "hist_jetPt"                   , "Jet p_{T}", 400, 0, 400 ) ;
  hists["hist_jetEtaVsPhi"              ] = theDir.make<TH2D>( "hist_jetEtaVsPhi"             , "Jet #phi versus #eta;#eta;#phi", 50, -5.0, 5.0, 50, -TMath::Pi(), TMath::Pi() ) ;
  hists["hist_jetNTracks"               ] = theDir.make<TH1D>( "hist_jetNTracks"              , "Jet N_{TRACKS}", 20, 0, 20 ) ;
  hists["hist_jetNTracksVsPt"           ] = theDir.make<TH2D>( "hist_jetNTracksVsPt"          , "Number of Tracks versus Jet p_{T};Jet p_{T}(GeV/c) ;N_{Tracks}",20, 0, 200, 20, 0, 20 ) ;
  hists["hist_jetEMF"                   ] = theDir.make<TH1D>( "hist_jetEMF"                  , "Jet EMF", 200, 0, 1) ;
  hists["hist_jetPdgID"                 ] = theDir.make<TH1D>( "hist_jetPdgID"                , "PDG Id of Jet Constituents", 10000, 0, 10000 ) ;
  hists["hist_jetGenEmE"                ] = theDir.make<TH1D>( "hist_jetGenEmE"               , "Gen Jet EM Energy", 200, 0, 200 ) ;
  hists["hist_jetGenHadE"               ] = theDir.make<TH1D>( "hist_jetGenHadE"              , "Gen Jet HAD Energy", 200, 0, 200 ) ;
  hists["hist_jetGenEMF"                ] = theDir.make<TH1D>( "hist_jetGenEMF"               , "Gen Jet EMF", 200, 0, 1) ;
  hists["hist_jetEoverGenE"             ] = theDir.make<TH1D>( "hist_jetEoverGenE"            , "Energy of reco Jet / Energy of gen Jet", 200, 0, 2.0) ;
  hists["hist_jetCorr"                  ] = theDir.make<TH1D>( "hist_jetCorr"                 , "Jet Correction Factor", 200, 0, 1.0 ) ;
  hists["hist_n90Hits"                  ] = theDir.make<TH1D>( "hist_n90Hits"                 , "Jet n90Hits", 20, 0, 20) ;
  hists["hist_fHPD"                     ] = theDir.make<TH1D>( "hist_fHPD"                    , "Jet fHPD", 200, 0, 1) ;
  hists["hist_nConstituents"            ] = theDir.make<TH1D>( "hist_nConstituents"           , "Jet nConstituents", 20, 0, 20 ) ;
  hists["hist_jetCHF"                   ] = theDir.make<TH1D>( "hist_jetCHF"                  , "Jet Charged Hadron Fraction", 200, 0, 1.0) ;
	                                      
  hists["hist_good_jetPt"             ] = theDir.make<TH1D>( "hist_good_jetPt"            , "Jet p_{T}", 400, 0, 400 ) ;
  hists["hist_good_jetEtaVsPhi"       ] = theDir.make<TH2D>( "hist_good_jetEtaVsPhi"      , "Jet #phi versus #eta;#eta;#phi", 50, -5.0, 5.0, 50, -TMath::Pi(), TMath::Pi() ) ;
  hists["hist_good_jetNTracks"        ] = theDir.make<TH1D>( "hist_good_jetNTracks"       , "Jet N_{TRACKS}", 20, 0, 20 ) ;
  hists["hist_good_jetNTracksVsPt"    ] = theDir.make<TH2D>( "hist_good_jetNTracksVsPt"   , "Number of Tracks versus Jet p_{T};Jet p_{T}(GeV/c) ;N_{Tracks}",20, 0, 200, 20, 0, 20 ) ;
  hists["hist_good_jetEMF"            ] = theDir.make<TH1D>( "hist_good_jetEMF"           , "Jet EMF", 200, 0, 1) ;
  hists["hist_good_jetPdgID"          ] = theDir.make<TH1D>( "hist_good_jetPdgID"         , "PDG Id of Jet Constituents", 10000, 0, 10000 ) ;
  hists["hist_good_jetGenEmE"         ] = theDir.make<TH1D>( "hist_good_jetGenEmE"        , "Gen Jet EM Energy", 200, 0, 200 ) ;
  hists["hist_good_jetGenHadE"        ] = theDir.make<TH1D>( "hist_good_jetGenHadE"       , "Gen Jet HAD Energy", 200, 0, 200 ) ;
  hists["hist_good_jetGenEMF"         ] = theDir.make<TH1D>( "hist_good_jetGenEMF"        , "Gen Jet EMF", 200, 0, 1) ;
  hists["hist_good_jetEoverGenE"      ] = theDir.make<TH1D>( "hist_good_jetEoverGenE"     , "Energy of reco Jet / Energy of gen Jet", 200, 0, 2.0) ;
  hists["hist_good_jetCorr"           ] = theDir.make<TH1D>( "hist_good_jetCorr"          , "Jet Correction Factor", 200, 0, 1.0 ) ;
  hists["hist_good_n90Hits"           ] = theDir.make<TH1D>( "hist_good_n90Hits"          , "Jet n90Hits", 20, 0, 20) ;
  hists["hist_good_fHPD"              ] = theDir.make<TH1D>( "hist_good_fHPD"             , "Jet fHPD", 200, 0, 1) ;
  hists["hist_good_nConstituents"     ] = theDir.make<TH1D>( "hist_good_nConstituents"    , "Jet nConstituents", 20, 0, 20 ) ;
  hists["hist_good_jetCHF"            ] = theDir.make<TH1D>( "hist_good_jetCHF"           , "Jet Charged Hadron Fraction", 200, 0, 1.0) ;
   	                                                  
	                                      
  hists["hist_pf_jetPt"                 ] = theDir.make<TH1D>( "hist_pf_jetPt"                , "PFJet p_{T}", 400, 0, 400 ) ;
  hists["hist_pf_jetEtaVsPhi"           ] = theDir.make<TH2D>( "hist_pf_jetEtaVsPhi"          , "PFJet #phi versus #eta;#eta;#phi", 50, -5.0, 5.0, 50, -TMath::Pi(), TMath::Pi() ) ;
  hists["hist_pf_jetNTracks"            ] = theDir.make<TH1D>( "hist_pf_jetNTracks"           , "PFJet N_{TRACKS}", 20, 0, 20 ) ;
  hists["hist_pf_jetNTracksVsPt"        ] = theDir.make<TH2D>( "hist_pf_jetNTracksVsPt"       , "Number of Tracks versus Jet p_{T};Jet p_{T}(GeV/c) ;N_{Tracks}",20, 0, 200, 20, 0, 20 ) ;
  hists["hist_pf_jetEMF"                ] = theDir.make<TH1D>( "hist_pf_jetEMF"               , "PFJet EMF", 200, 0, 1) ;
  hists["hist_pf_jetCHF"                ] = theDir.make<TH1D>( "hist_pf_jetCHF"               , "PFJet CHF", 200, 0, 1) ;
  hists["hist_pf_jetNHF"                ] = theDir.make<TH1D>( "hist_pf_jetNHF"               , "PFJet NHF", 200, 0, 1) ;
  hists["hist_pf_jetCEF"                ] = theDir.make<TH1D>( "hist_pf_jetCEF"               , "PFJet CEF", 200, 0, 1) ;
  hists["hist_pf_jetNEF"                ] = theDir.make<TH1D>( "hist_pf_jetNEF"               , "PFJet NEF", 200, 0, 1) ;
  hists["hist_pf_jetPdgID"              ] = theDir.make<TH1D>( "hist_pf_jetPdgID"             , "PDG Id of Jet Constituents", 10000, 0, 10000 ) ;
  hists["hist_pf_jetGenEmE"             ] = theDir.make<TH1D>( "hist_pf_jetGenEmE"            , "Gen Jet EM Energy", 200, 0, 200 ) ;
  hists["hist_pf_jetGenHadE"            ] = theDir.make<TH1D>( "hist_pf_jetGenHadE"           , "Gen Jet HAD Energy", 200, 0, 200 ) ;
  hists["hist_pf_jetGenEMF"             ] = theDir.make<TH1D>( "hist_pf_jetGenEMF"            , "Gen Jet EMF", 200, 0, 1) ;
  hists["hist_pf_jetEoverGenE"          ] = theDir.make<TH1D>( "hist_pf_jetEoverGenE"         , "Energy of reco Jet / Energy of gen Jet", 200, 0, 2.0) ;
  hists["hist_pf_jetCorr"               ] = theDir.make<TH1D>( "hist_pf_jetCorr"              , "PFJet Correction Factor", 200, 0, 1.0 ) ;
  hists["hist_pf_nConstituents"         ] = theDir.make<TH1D>( "hist_pf_nConstituents"        , "PFJet nConstituents", 20, 0, 20 ) ;
	                                      
  hists["hist_pf_good_jetPt"          ] = theDir.make<TH1D>( "hist_pf_good_jetPt"         , "PFJet p_{T}", 400, 0, 400 ) ;
  hists["hist_pf_good_jetEtaVsPhi"    ] = theDir.make<TH2D>( "hist_pf_good_jetEtaVsPhi"   , "PFJet #phi versus #eta;#eta;#phi", 50, -5.0, 5.0, 50, -TMath::Pi(), TMath::Pi() ) ;
  hists["hist_pf_good_jetNTracks"     ] = theDir.make<TH1D>( "hist_pf_good_jetNTracks"    , "PFJet N_{TRACKS}", 20, 0, 20 ) ;
  hists["hist_pf_good_jetNTracksVsPt" ] = theDir.make<TH2D>( "hist_pf_good_jetNTracksVsPt", "Number of Tracks versus Jet p_{T};Jet p_{T}(GeV/c) ;N_{Tracks}",20, 0, 200, 20, 0, 20 ) ;
  hists["hist_pf_good_jetEMF"         ] = theDir.make<TH1D>( "hist_pf_good_jetEMF"        , "PFJet EMF", 200, 0, 1) ;
  hists["hist_pf_good_jetCHF"         ] = theDir.make<TH1D>( "hist_pf_good_jetCHF"        , "PFJet CHF", 200, 0, 1) ;
  hists["hist_pf_good_jetNHF"         ] = theDir.make<TH1D>( "hist_pf_good_jetNHF"        , "PFJet NHF", 200, 0, 1) ;
  hists["hist_pf_good_jetCEF"         ] = theDir.make<TH1D>( "hist_pf_good_jetCEF"        , "PFJet CEF", 200, 0, 1) ;
  hists["hist_pf_good_jetNEF"         ] = theDir.make<TH1D>( "hist_pf_good_jetNEF"        , "PFJet NEF", 200, 0, 1) ;
  hists["hist_pf_good_jetPdgID"       ] = theDir.make<TH1D>( "hist_pf_good_jetPdgID"      , "PDG Id of Jet Constituents", 10000, 0, 10000 ) ;
  hists["hist_pf_good_jetGenEmE"      ] = theDir.make<TH1D>( "hist_pf_good_jetGenEmE"     , "Gen Jet EM Energy", 200, 0, 200 ) ;
  hists["hist_pf_good_jetGenHadE"     ] = theDir.make<TH1D>( "hist_pf_good_jetGenHadE"    , "Gen Jet HAD Energy", 200, 0, 200 ) ;
  hists["hist_pf_good_jetGenEMF"      ] = theDir.make<TH1D>( "hist_pf_good_jetGenEMF"     , "Gen Jet EMF", 200, 0, 1) ;
  hists["hist_pf_good_jetEoverGenE"   ] = theDir.make<TH1D>( "hist_pf_good_jetEoverGenE"  , "Energy of reco Jet / Energy of gen Jet", 200, 0, 2.0) ;
  hists["hist_pf_good_jetCorr"        ] = theDir.make<TH1D>( "hist_pf_good_jetCorr"       , "PFJet Correction Factor", 200, 0, 1.0 ) ;
  hists["hist_pf_good_nConstituents"  ] = theDir.make<TH1D>( "hist_pf_good_nConstituents" , "PFJet nConstituents", 20, 0, 20 ) ;

   

}




///////////////////
/// The event loop
//////////////////
void MultijetAnalyzer::analyze(const edm::EventBase& event)
{

    pat::strbitset retCalo = caloSelector.getBitTemplate();
    bool passedCalo = caloSelector( event, retCalo );
    pat::strbitset retPF = pfSelector.getBitTemplate();
    bool passedPF = pfSelector( event, retPF );


    ///------------------
    /// CALO JETS
    ///------------------
    if ( retCalo.test("Calo Dijet HT") ) {

      for ( std::vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin = caloSelector.allCaloJets().begin(),
	      jetEnd = caloSelector.allCaloJets().end(), ijet = jetBegin;
	    ijet != jetEnd; ++ijet ) {

	
	const pat::Jet & jet = **ijet;

	double pt = jet.pt();

	const reco::TrackRefVector & jetTracks = jet.associatedTracks();

	hists["hist_jetPt"]->Fill( pt );
	hists["hist_jetEtaVsPhi"]->Fill( jet.eta(), jet.phi() );
	hists["hist_jetNTracks"]->Fill( jetTracks.size() );
	hists["hist_jetNTracksVsPt"]->Fill( pt, jetTracks.size() );
	hists["hist_jetEMF"]->Fill( jet.emEnergyFraction() );	
	hists["hist_jetCorr"]->Fill( jet.jecFactor(0) );
	hists["hist_n90Hits"]->Fill( static_cast<int>(jet.jetID().n90Hits) );
	hists["hist_fHPD"]->Fill( jet.jetID().fHPD );
	hists["hist_nConstituents"]->Fill( jet.nConstituents() );

	if ( useMC && jet.genJet() != 0 ) {
	  hists["hist_jetGenEmE"]->Fill( jet.genJet()->emEnergy() );
	  hists["hist_jetGenHadE"]->Fill( jet.genJet()->hadEnergy() );
	  hists["hist_jetEoverGenE"]->Fill( jet.energy() / jet.genJet()->energy() );
	  hists["hist_jetGenEMF"]->Fill( jet.genJet()->emEnergy() / jet.genJet()->energy() );
	}
	if ( doTracks ) {
	  TLorentzVector p4_tracks(0,0,0,0);
	  for ( reco::TrackRefVector::const_iterator itrk = jetTracks.begin(),
		  itrkBegin = jetTracks.begin(), itrkEnd = jetTracks.end();
		itrk != itrkEnd; ++itrk ) {
	    TLorentzVector p4_trk;
	    double M_PION = 0.140;
	    p4_trk.SetPtEtaPhiM( (*itrk)->pt(), (*itrk)->eta(), (*itrk)->phi(), M_PION );
	    p4_tracks += p4_trk;
	  }
	  hists["hist_jetCHF"]->Fill( p4_tracks.Energy() / jet.energy() );
	}

      
      } // end loop over jets

    

      if ( retCalo.test("Calo Jet ID") ) {
	hists["hist_nJet"]->Fill( caloSelector.caloJets().size() );
	double hT = 0.0;
	for ( std::vector<edm::Ptr<pat::Jet> >::const_iterator jetsBegin = caloSelector.caloJets().begin(),
		jetsEnd = caloSelector.caloJets().end(),
		ijet = jetsBegin;
	      ijet != jetsEnd; ++ijet ) {


	  pat::Jet const & jet0 = **ijet;
	  hT += jet0.pt();
	  hists["hist_good_jetPt"]->Fill( jet0.pt() );
	  hists["hist_good_jetEtaVsPhi"]->Fill( jet0.eta(), jet0.phi() );
	  hists["hist_good_jetNTracks"]->Fill( jet0.associatedTracks().size() );
	  hists["hist_good_jetNTracksVsPt"]->Fill( jet0.pt(), jet0.associatedTracks().size() );
	  hists["hist_good_jetEMF"]->Fill( jet0.emEnergyFraction() );	
	  hists["hist_good_jetCorr"]->Fill( jet0.jecFactor(0) );
	  hists["hist_good_n90Hits"]->Fill( static_cast<int>(jet0.jetID().n90Hits) );
	  hists["hist_good_fHPD"]->Fill( jet0.jetID().fHPD );
	  hists["hist_good_nConstituents"]->Fill( jet0.nConstituents() );
	}// end loop over good calo jets

	hists["hist_hT"]->Fill( hT );
	caloSummary.push_back( EventSummary(hT, event.id().run(), event.id().event(), event.id().luminosityBlock() ) );
      }// end if passed calo jet id

    }// end if passed kin cuts


    ///------------------
    /// PF JETS
    ///------------------
    if ( retPF.test("PF Dijet HT") ) {

      for ( std::vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin = pfSelector.allPFJets().begin(),
	      jetEnd = pfSelector.allPFJets().end(), ijet = jetBegin;
	    ijet != jetEnd; ++ijet ) {
	
	const pat::Jet & jet = **ijet;
	
	double pt = jet.pt();
      
	hists["hist_pf_jetPt"]->Fill( pt );
	hists["hist_pf_jetEtaVsPhi"]->Fill( jet.eta(), jet.phi() );
	hists["hist_pf_nConstituents"]->Fill( jet.nConstituents() );
	hists["hist_pf_jetCEF"]->Fill( jet.chargedEmEnergyFraction()  );
	hists["hist_pf_jetNEF"]->Fill( jet.neutralEmEnergyFraction()  );
	hists["hist_pf_jetCHF"]->Fill( jet.chargedHadronEnergyFraction()  );
	hists["hist_pf_jetNHF"]->Fill( jet.neutralHadronEnergyFraction()  );


	if ( useMC && jet.genJet() != 0 ) {
	  hists["hist_pf_jetGenEmE"]->Fill( jet.genJet()->emEnergy() );
	  hists["hist_pf_jetGenHadE"]->Fill( jet.genJet()->hadEnergy() );
	  hists["hist_pf_jetEoverGenE"]->Fill( jet.energy() / jet.genJet()->energy() );

	  hists["hist_pf_jetGenEMF"]->Fill( jet.genJet()->emEnergy() / jet.genJet()->energy() );
	}
 
      } // end loop over jets

    

      if ( retPF.test("PF Jet ID") ) {

	hists["hist_nPFJet"]->Fill( pfSelector.pfJets().size() );
	double hT = 0.0;
	std::vector<edm::Ptr<pat::Jet> > const & goodPFJets = pfSelector.pfJets();

	for ( std::vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin = goodPFJets.begin(),
		jetEnd = goodPFJets.end(), ijet = jetBegin;
	      ijet != jetEnd; ++ijet ) {
	
	  const pat::Jet & jet = **ijet;
	
	  double pt = jet.pt();
	  hT += pt;
      
	  hists["hist_pf_good_jetPt"]->Fill( pt );
	  hists["hist_pf_good_jetEtaVsPhi"]->Fill( jet.eta(), jet.phi() );
	  hists["hist_pf_good_nConstituents"]->Fill( jet.nConstituents() );
	  hists["hist_pf_good_jetCEF"]->Fill( jet.chargedEmEnergyFraction()  );
	  hists["hist_pf_good_jetNEF"]->Fill( jet.neutralEmEnergyFraction()  );
	  hists["hist_pf_good_jetCHF"]->Fill( jet.chargedHadronEnergyFraction()  );
	  hists["hist_pf_good_jetNHF"]->Fill( jet.neutralHadronEnergyFraction()  );


	  if ( useMC && jet.genJet() != 0 ) {
	    hists["hist_pf_good_jetGenEmE"]->Fill( jet.genJet()->emEnergy() );
	    hists["hist_pf_good_jetGenHadE"]->Fill( jet.genJet()->hadEnergy() );
	    hists["hist_pf_good_jetEoverGenE"]->Fill( jet.energy() / jet.genJet()->energy() );

	    hists["hist_pf_good_jetGenEMF"]->Fill( jet.genJet()->emEnergy() / jet.genJet()->energy() );
	  }
 
	} // end loop over good pf jets

	hists["hist_pf_hT"]->Fill( hT );
	pfSummary.push_back( EventSummary(hT, event.id().run(), event.id().event(), event.id().luminosityBlock() ) );
      } // end if N good PF jets
    
    }// end if passed kin cuts


}


void MultijetAnalyzer::endJob()
{

  
  std::cout << "Calo jet selection" << std::endl;
  caloSelector.print(std::cout);
  std::cout << "PF jet selection" << std::endl;
  pfSelector.print(std::cout);

//   sort( caloSummary.begin(), caloSummary.end() );
//   sort( pfSummary.begin(), pfSummary.end() );

//   std::cout << "============= 4-jet CALO events =============" << std::endl;
//   copy( caloSummary.begin(), caloSummary.end(), std::ostream_iterator<EventSummary>(std::cout) );
//   std::cout << std::endl;
//   std::cout << "============= 4-jet PF events =============" << std::endl;
//   copy( pfSummary.begin(), pfSummary.end(), std::ostream_iterator<EventSummary>(std::cout) );
//   std::cout << std::endl;  
}
