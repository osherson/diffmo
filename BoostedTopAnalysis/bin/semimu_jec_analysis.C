// -*- C++ -*-

// CMS includes
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "Analysis/BoostedTopAnalysis/interface/HadronicSelectionBoostedW.h"
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 

#include "TH2.h"
#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>

// Root includes
#include "TROOT.h"
#include "TLorentzVector.h"

using namespace std;

///////////////////////////
// ///////////////////// //
// // Main Subroutine // //
// ///////////////////// //
///////////////////////////

int main (int argc, char* argv[]) 
{
   ////////////////////////////////
   // ////////////////////////// //
   // // Command Line Options // //
   // ////////////////////////// //
   ////////////////////////////////

   // Tell people what this analysis code does and setup default options.
   optutl::CommandLineParser parser ("Boosted Top Semi-Muonic JEC Example");

   ////////////////////////////////////////////////
   // Change any defaults or add any new command //
   //      line options you would like here.     //
   ////////////////////////////////////////////////
   parser.stringValue ("outputFile") = "ttbsm_semimu_jec"; // .root added automatically



   parser.addOption ("muPlusJets",   optutl::CommandLineParser::kBool,
		     "Run mu+Jets",
                     true );
   parser.addOption ("ePlusJets",   optutl::CommandLineParser::kBool,
		     "Run e+Jets",
                     true );
   parser.addOption ("minNJets",   optutl::CommandLineParser::kInteger,
		     "Min number of anti-kT 0.5 jets",
                     2 );
   parser.addOption ("tightMuMinPt",   optutl::CommandLineParser::kDouble,
		     "Min tight mu pt",
                     20.0 );
   parser.addOption ("tightMuMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max tight mu eta",
                     2.1 );
   parser.addOption ("tightEleMinPt",   optutl::CommandLineParser::kDouble,
		     "Min tight e pt",
		     30.0 );
   parser.addOption ("tightEleMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max tight e eta",
                     2.4 );
   parser.addOption ("looseMuMinPt",   optutl::CommandLineParser::kDouble,
		     "Min loose mu pt",
                     20.0 );
   parser.addOption ("looseMuMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max loose mu eta",
                     2.1 );
   parser.addOption ("looseEleMinPt",   optutl::CommandLineParser::kDouble,
		     "Min loose e pt",
		     30.0 );
   parser.addOption ("looseEleMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max loose e eta",
                     2.4 );
   parser.addOption ("jetMinPt",   optutl::CommandLineParser::kDouble,
		     "Min jet pt, anti-kT 0.5 jets",
		     30.0 );
   parser.addOption ("jetMaxEta",   optutl::CommandLineParser::kDouble,
		     "Max jet eta, anti-kT 0.5 jets",
                     2.4 );
   parser.addOption ("mu", optutl::CommandLineParser::kDouble,
		     "Maximum mass drop of hardest subjet",
		     0.7 );
   parser.addOption ("ycut", optutl::CommandLineParser::kDouble,
		     "Minimum symmetry of two subjets",
		     0.1 );
   parser.addOption ("leadJetPt", optutl::CommandLineParser::kDouble,
		     "Minimum jet pt of hadronic side",
		     150.0 );
   parser.addOption ("deltaPhi", optutl::CommandLineParser::kDouble,
		     "Minimum delta phi between leading jet and leading muon",
		     TMath::Pi() / 2.0 );



   // Parse the command line arguments
   parser.parseArguments (argc, argv);

   //////////////////////////////////
   // //////////////////////////// //
   // // Create Event Container // //
   // //////////////////////////// //
   //////////////////////////////////

   // This object 'event' is used both to get all information from the
   // event as well as to store histograms, etc.
   fwlite::EventContainer eventCont (parser);
  
  cout << "About to allocate functors" << endl;

  // Tight muon id
  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdTight     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08,
					  10.0,
					  999.0,
					  999.0,
					  3.0,
					  11,
					  4.0,
					  6.0,
					  0.05  ) );
  muonIdTight->set( "D0", 0.02 );
  muonIdTight->set( "RelIso", false);
  muonIdTight->set( "HCalVeto", false );
  muonIdTight->set( "ECalVeto", false );
  // Tight electron id
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdTight     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08,
					      999.0,
					      999.0,
					      3.0,
					      0.1 ) );
  electronIdTight->set( "D0", 0.02 );
  electronIdTight->set( "RelIso", false);

  // Tight jet id
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdTight      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::TIGHT) );

  
  // Loose muon id
  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdLoose     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08,
					  10.0,
					  999.0,
					  999.0,
					  3.0,
					  11,
					  4.0,
					  6.0,
					  0.05 ) );
  muonIdLoose->set( "Chi2",    false);
  muonIdLoose->set( "D0",      false);
  muonIdLoose->set( "NHits",   false);
  muonIdLoose->set( "ECalVeto",false);
  muonIdLoose->set( "HCalVeto",false);
  muonIdLoose->set( "RelIso",  false );

  // Loose electron id
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdLoose     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08,
					      999.0,
					      999.0,
					      3.0,
					      0.1) );
  electronIdLoose->set( "D0",  false);
  electronIdLoose->set( "RelIso", false);
  // Loose jet id
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdLoose      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::LOOSE) );

  cout << "Making event selector" << endl;
  boost::shared_ptr<WPlusJetsEventSelector> wPlusJets(
    new WPlusJetsEventSelector (
     edm::InputTag("selectedLayer1Muons"),
     edm::InputTag("selectedLayer1Electrons"),
     edm::InputTag("selectedLayer1Jets"),
     edm::InputTag("layer1METs"),
     edm::InputTag("patTriggerEvent"),
     muonIdTight,
     electronIdTight,
     jetIdTight,
     muonIdLoose,
     electronIdLoose,
     jetIdLoose,
     parser.integerValue ("minNJets")      ,
     parser.boolValue    ("muPlusJets")    ,
     parser.boolValue    ("ePlusJets")     ,
     parser.doubleValue  ("tightMuMinPt")  ,
     parser.doubleValue  ("tightMuMaxEta") ,
     parser.doubleValue  ("tightEleMinPt") ,
     parser.doubleValue  ("tightEleMaxEta"),
     parser.doubleValue  ("looseMuMinPt")  ,
     parser.doubleValue  ("looseMuMaxEta") ,
     parser.doubleValue  ("looseEleMinPt") ,
     parser.doubleValue  ("looseEleMaxEta"),
     parser.doubleValue  ("jetMinPt")      ,
     parser.doubleValue  ("jetMaxEta")
     )
   );


  HadronicSelectionBoostedW boostedWHadronic(
     wPlusJets,
     edm::InputTag("selectedLayer1JetsPrunedPF"),
     edm::InputTag("patTriggerEvent"),
     parser.doubleValue("jetMinPt"), parser.doubleValue("jetMaxEta"),
     parser.doubleValue("mu"), parser.doubleValue("ycut"), parser.doubleValue("deltaPhi")
   );

  // turn off trigger check on the muonic side for now
   wPlusJets->set("Trigger", false);

   // Book those histograms!
   eventCont.add( new TH1F( "jetPt", "Jet p_{T};Jet p_{T} (GeV/c)", 60, 0, 3000) );
   eventCont.add( new TH1F( "jetMass", "Jet Mass;Jet Mass (GeV/c^{2}", 100, 0, 500 ) );

   eventCont.add( new TH1F( "muPt", "Muon p_{T};Muon p_{T} (GeV/c)", 50, 0, 500 ) );

   eventCont.add( new TH1F( "muPtRel", "Muon p_{T} Relative to Closest Jet;p_{T} (GeV/c)", 40, 0, 200.0) );
   eventCont.add( new TH1F( "muDRMin", "Muon #Delta R Relative to Closest Jet;#Delta R", 50, 0, 5.0) );
   
   eventCont.add( new TH2F( "mu2D", "Muon 2D Cut;#Delta R_{min};p_{T}^{REL} (GeV/c)",
			    50, 0, 5.0, 40, 0, 200.0 ) );

   eventCont.add( new TH1F( "deltaPhi", "#Delta #phi",
			    50, 0, TMath::Pi()) );
   
   eventCont.add( new TH1F( "m0", "m0", 50, 0, 200) );
   eventCont.add( new TH1F( "m1", "m1", 50, 0, 200) );
   eventCont.add( new TH1F( "pt0", "pt0", 50, 0, 200) );
   eventCont.add( new TH1F( "pt1", "pt1", 50, 0, 200) );
   eventCont.add( new TH1F( "m", "m", 50, 0, 200 ) );
   eventCont.add( new TH1F( "deltaR", "#Delta R",
			    50, 0, TMath::TwoPi() ) );
   eventCont.add( new TH1F( "y", "y", 
			    50, 0, 1 ) );
   eventCont.add( new TH1F( "m0_over_m", "m_{0} / m",
			    50, 0, 1 ) );

   //////////////////////
   // //////////////// //
   // // Event Loop // //
   // //////////////// //
   //////////////////////

   for (eventCont.toBegin(); ! eventCont.atEnd(); ++eventCont) {

     if ( eventCont.id().event() <= 0 ) {
       cout << "Skipping run " << eventCont.id().run() << ", event " << eventCont.id().event() << endl;
       continue;
     }

     // Leptonic side
    pat::strbitset retSemi = wPlusJets->getBitTemplate();


    bool passedSemi = (*wPlusJets)(eventCont, retSemi);
    vector<pat::Muon>     const & muons     = wPlusJets->selectedMuons();
    vector<pat::Jet>      const & looseJets = wPlusJets->selectedJets();

    if ( passedSemi && muons.size() > 0 ) {
      eventCont.hist("muPt")->Fill( muons[0].pt() );
    
      double dRMin = 5.0;
      vector<pat::Jet>::const_iterator closestJet = looseJets.end();
      for ( vector<pat::Jet>::const_iterator ijet = looseJets.begin(),
	      looseJetsBegin = looseJets.begin(), looseJetsEnd = looseJets.end();
	    ijet != looseJetsEnd; ++ijet ) {
	double dR = reco::deltaR<double>( ijet->eta(), ijet->phi(),
					  muons[0].eta(), muons[0].phi() );
	if ( dR < dRMin ) {
	  dRMin = dR;
	  closestJet = ijet;
	}
      }
      if ( closestJet != looseJets.end() )  {
	TLorentzVector muP ( muons[0].px(),
			     muons[0].py(),
			     muons[0].pz(),
			     muons[0].energy() );

	TLorentzVector bjetP ( closestJet->px(),
			       closestJet->py(),
			       closestJet->pz(),
			       closestJet->energy() );

	double ptRel = TMath::Abs( muP.Perp( bjetP.Vect() ) );

	eventCont.hist("muPtRel")->Fill( ptRel );
	eventCont.hist("muDRMin")->Fill( dRMin );
	eventCont.hist("mu2D")->Fill( dRMin, ptRel );

	// Hadronic side
	pat::strbitset ret = boostedWHadronic.getBitTemplate();
	

	// bool passed = boostedWHadronic(eventCont, ret);
	HadronicSelectionBoostedW::collection_type const & jets = boostedWHadronic.selectedJets();
	// HadronicSelectionBoostedW::collection_type const & tags = boostedWHadronic.taggedJets();
	
	if ( ret[std::string(">= 1 Tag")] ) {
	  for ( HadronicSelectionBoostedW::collection_type::const_iterator jetsBegin = jets.begin(),
		  jetsEnd = jets.end(),
		  ijet = jetsBegin;
		ijet != jetsEnd;
		++ijet ) {
	    eventCont.hist("jetPt")->Fill( (*ijet)->pt() );
	    eventCont.hist("jetMass")->Fill( (*ijet)->mass() );
	  }
	}
      }
    }
    
   } // for eventCont



   ////////////////////////
   // ////////////////// //
   // // Clean Up Job // //
   // ////////////////// //
   ////////////////////////


   wPlusJets->print(cout);
   boostedWHadronic.print(cout);

   // Histograms will be automatically written to the root file
   // specificed by command line options.

   // All done!  Bye bye.
   return 0;
}


