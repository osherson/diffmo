/*   A macro for making a histogram of Jet Pt with cuts
     This is a basic way to cut out jets of a certain Pt and Eta using an if statement
     This example creates a histogram of Jet Pt, using Jets with Pt above 30 and ETA above -2.1 and below 2.1
*/

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TLorentzVector.h"

#include "PhysicsTools/SelectorUtils/interface/strbitset.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"

#include "Analysis/JetAnalysis/interface/DijetSelector.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()

using namespace std;

///-------------------------
/// DRIVER FUNCTION
///-------------------------


// Root includes
#include "TROOT.h"

using namespace std;


///////////////////////////
// ///////////////////// //
// // Main Subroutine // //
// ///////////////////// //
///////////////////////////

int main (int argc, char* argv[]) 
{


  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  cout << "Hello from " << argv[0] << "!" << endl;


  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();


  cout << "Getting parameters" << endl;
  // Get the python configuration
  PythonProcessDesc builder(argv[1], argc, argv);
  boost::shared_ptr<edm::ProcessDesc> b = builder.processDesc();
  boost::shared_ptr<edm::ParameterSet> parameters = b->getProcessPSet();
  parameters->registerIt(); 

  edm::ParameterSet const& jetStudiesParams    = parameters->getParameter<edm::ParameterSet>("jetStudies");
  edm::ParameterSet const& pfJetStudiesParams  = parameters->getParameter<edm::ParameterSet>("pfJetStudies");
  edm::ParameterSet const& caloJetIDParameters = parameters->getParameter<edm::ParameterSet>("jetIDSelector");
  edm::ParameterSet const& pfJetIDParameters   = parameters->getParameter<edm::ParameterSet>("pfJetIDSelector");
  edm::ParameterSet const& plotParameters      = parameters->getParameter<edm::ParameterSet>("plotParameters");
  edm::ParameterSet const& inputs              = parameters->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet const& outputs             = parameters->getParameter<edm::ParameterSet>("outputs");
  
  cout << "setting up TFileService" << endl;
  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );
  TFileDirectory theDir = fs.mkdir( "histos" ); 
    
  cout << "Setting up chain event" << endl;
  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::ChainEvent ev ( inputs.getParameter<std::vector<std::string> > ("fileNames") );


  cout << "Booking histograms" << endl;
  // Book histograms

  std::map<std::string, TH1*> hists;

  hists["hist_nJet"                     ] = theDir.make<TH1D>( "hist_nJet"                    ,"Number of Calo Jets", 10, 0, 10 ) ;
  hists["hist_nPFJet"                   ] = theDir.make<TH1D>( "hist_nPFJet"                  ,"Number of PF Jets", 10, 0, 10 ) ;

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


   hists["hist_mjj"                           ] = theDir.make<TH1D>( "hist_mjj"                          , "Dijet mass", 300, 0, 300 ) ;
   hists["hist_dR_jj"                         ] = theDir.make<TH1D>( "hist_dR_jj"                        , "#Delta R_{JJ}", 200, 0, TMath::TwoPi() ) ;
   hists["hist_imbalance_jj"                  ] = theDir.make<TH1D>( "hist_imbalance_jj"                 , "Dijet imbalance", 200, -1.0, 1.0 )  ;
	                                      
   hists["hist_pf_mjj"                        ] = theDir.make<TH1D>( "hist_pf_mjj"                       , "Dijet mass", 300, 0, 300 ) ;
   hists["hist_pf_dR_jj"                      ] = theDir.make<TH1D>( "hist_pf_dR_jj"                     , "#Delta R_{JJ}", 200, 0, TMath::TwoPi() ) ;
   hists["hist_pf_imbalance_jj"               ] = theDir.make<TH1D>( "hist_pf_imbalance_jj"              , "Dijet imbalance", 200, -1.0, 1.0 )  ;

   
   
  cout << "Making functors" << endl;
  DijetSelector caloSelector( caloJetIDParameters,
				 pfJetIDParameters,
				 jetStudiesParams );

  DijetSelector pfSelector( caloJetIDParameters,
			       pfJetIDParameters,
			       pfJetStudiesParams );

  vector<int> const & runs = plotParameters.getParameter<std::vector<int> >("runs");
  bool doTracks = plotParameters.getParameter<bool>("doTracks");
  bool useMC    = plotParameters.getParameter<bool>("useMC");

  cout << "About to begin looping" << endl;

  int nev = 0;
  //loop through each event
  for (ev.toBegin(); ! ev.atEnd(); ++ev, ++nev) {


    edm::EventBase const & event = ev;

    int run = event.id().run();
    if ( runs.size() > 0 && find( runs.begin(), runs.end(), run ) == runs.end() ) continue;
    
    if ( nev % 10000 == 0 ) cout << "Entry " << nev << ", Processing run " << event.id().run() << ", event " << event.id().event() << endl;

    pat::strbitset retCalo = caloSelector.getBitTemplate();
    bool passedCalo = caloSelector( event, retCalo );

    pat::strbitset retPF = pfSelector.getBitTemplate();
    bool passedPF = pfSelector( event, retPF );

    ///------------------
    /// PF JETS
    ///------------------
    if ( retPF.test("PF Kin Cuts") ) {

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
	vector<edm::Ptr<pat::Jet> > const & goodPFJets = pfSelector.pfJets();

	for ( std::vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin = goodPFJets.begin(),
		jetEnd = goodPFJets.end(), ijet = jetBegin;
	      ijet != jetEnd; ++ijet ) {
	
	  const pat::Jet & jet = **ijet;
	
	  double pt = jet.pt();
      
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

	pat::Jet const & jet0 = **(pfSelector.pfJets().begin());
	pat::Jet const & jet1 = **(pfSelector.pfJets().begin() + 1);

	TLorentzVector p4_j0( jet0.px(), jet0.py(), jet0.pz(), jet0.energy() );
	TLorentzVector p4_j1( jet1.px(), jet1.py(), jet1.pz(), jet1.energy() );

	TLorentzVector p4_jj = p4_j0 + p4_j1;

	hists["hist_pf_mjj"]->Fill( p4_jj.M() );
	hists["hist_pf_dR_jj"]->Fill( p4_j0.DeltaR( p4_j1 ) );
	hists["hist_pf_imbalance_jj"]->Fill( (p4_j0.Perp() - p4_j1.Perp() ) /
					     (p4_j0.Perp() + p4_j1.Perp() ) );


      } // end if N good PF jets
    
    }// end if passed kin cuts

    
  } // end loop over events
    
  cout << "Calo jet selection" << endl;
  caloSelector.print(std::cout);
  cout << "PF jet selection" << endl;
  pfSelector.print(std::cout);




  return 0;
}

