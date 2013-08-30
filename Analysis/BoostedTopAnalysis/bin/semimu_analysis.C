// -*- C++ -*-

// CMS includes
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "Analysis/BoostedTopAnalysis/interface/HadronicSelection.h"
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
   optutl::CommandLineParser parser ("Boosted Top Semi-Muonic Example");

   ////////////////////////////////////////////////
   // Change any defaults or add any new command //
   //      line options you would like here.     //
   ////////////////////////////////////////////////
   parser.stringValue ("outputFile") = "ttbsm_semimu"; // .root added automatically



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
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  muonIdTight->set( "D0", 0.02 );
  muonIdTight->set( "RelIso", false);
  muonIdTight->set( "HCalVeto", false );
  muonIdTight->set( "ECalVeto", false );
  // Tight electron id
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdTight     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  electronIdTight->set( "D0", 0.02 );
  electronIdTight->set( "RelIso", false);

  // Tight jet id
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdTight      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::TIGHT) );

  
  // Loose muon id
  boost::shared_ptr<MuonVPlusJetsIDSelectionFunctor>      muonIdLoose     
    (new MuonVPlusJetsIDSelectionFunctor( MuonVPlusJetsIDSelectionFunctor::SUMMER08 ) );
  muonIdLoose->set( "Chi2",    false);
  muonIdLoose->set( "D0",      false);
  muonIdLoose->set( "NHits",   false);
  muonIdLoose->set( "ECalVeto",false);
  muonIdLoose->set( "HCalVeto",false);
  muonIdLoose->set( "RelIso",  false );

  // Loose electron id
  boost::shared_ptr<ElectronVPlusJetsIDSelectionFunctor>  electronIdLoose     
    (new ElectronVPlusJetsIDSelectionFunctor( ElectronVPlusJetsIDSelectionFunctor::SUMMER08) );
  electronIdLoose->set( "D0",  false);
  electronIdLoose->set( "RelIso", false);
  // Loose jet id
  boost::shared_ptr<JetIDSelectionFunctor>                jetIdLoose      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, JetIDSelectionFunctor::LOOSE) );

  cout << "Making event selector" << endl;
  WPlusJetsEventSelector wPlusJets(
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
     );



   std::string tagName = "CATopCaloJet";

  // top tagging
   boost::shared_ptr<CATopTagFunctor> caTopTagFunctor
     ( new CATopTagFunctor( CATopTagFunctor::CALO, 
			    CATopTagFunctor::TIGHT,
			    tagName ) );

   HadronicSelection caTopHadronic(
     edm::InputTag("selectedLayer1JetsTopTagCalo"),
     edm::InputTag("patTriggerEvent"),
     jetIdTight,caTopTagFunctor,
     1,
     1,
     250,
     2.5,
     100., 250.,
     50.,
     0., 1000000.
     );

  // turn off trigger check on the muonic side for now
   wPlusJets.set("Trigger", false);

   // Setup a style
   gROOT->SetStyle ("Plain");

   // Book those histograms!
   eventCont.add( new TH1F( "jetPt", "Jet p_{T};Jet p_{T} (GeV/c)", 60, 0, 3000) );
   eventCont.add( new TH1F( "topMass", "Top Mass;Top Mass (GeV/c^{2}", 100, 0, 500 ) );
   eventCont.add( new TH1F( "minMass", "Jet Min Mass;Min Mass (GeV/c^{2})", 100, 0, 200 ) );
   eventCont.add( new TH1F( "wMass",   "W Mass;W Mass (GeV/c^{2})", 100, 0, 200 ) );

   eventCont.add( new TH1F( "dijetMass", "Dijet Mass;Mass (GeV/c^{2})", 350, 0, 3500 ) );


   eventCont.add( new TH1F( "muPt", "Muon p_{T};Muon p_{T} (GeV/c)", 50, 0, 500 ) );

   eventCont.add( new TH1F( "muPtRel", "Muon p_{T} Relative to Closest Jet;p_{T} (GeV/c)", 40, 0, 200.0) );
   eventCont.add( new TH1F( "muDRMin", "Muon #Delta R Relative to Closest Jet;#Delta R", 50, 0, 5.0) );
   
   eventCont.add( new TH2F( "mu2D", "Muon 2D Cut;#Delta R_{min};p_{T}^{REL} (GeV/c)",
			    50, 0, 5.0, 40, 0, 200.0 ) );

   //////////////////////
   // //////////////// //
   // // Event Loop // //
   // //////////////// //
   //////////////////////

   for (eventCont.toBegin(); ! eventCont.atEnd(); ++eventCont) {

     // Leptonic side
    pat::strbitset retSemi = wPlusJets.getBitTemplate();


    bool passedSemi = wPlusJets(eventCont, retSemi);
    vector<pat::Muon>     const & muons     = wPlusJets.selectedMuons();
    vector<pat::Jet>      const & looseJets = wPlusJets.selectedJets();

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
	pat::strbitset ret = caTopHadronic.getBitTemplate();
	

	// bool passed = caTopHadronic(eventCont, ret);
	vector<pat::Jet>      const & jets      = caTopHadronic.selectedJets();
	// vector<pat::Jet>      const & tags      = caTopHadronic.taggedJets();
	
	if ( ret[std::string(">= 1 Tight Jet")] ) {
	  for ( vector<pat::Jet>::const_iterator jetsBegin = jets.begin(),
		  jetsEnd = jets.end(),
		  ijet = jetsBegin;
		ijet != jetsEnd;
		++ijet ) {
	    
	    
	    const reco::CATopJetTagInfo * catopTag = 
	      dynamic_cast<reco::CATopJetTagInfo const *>(ijet->tagInfo(tagName));
	    eventCont.hist("jetPt")->Fill( ijet->pt() );
	    if ( catopTag != 0 && catopTag->properties().minMass < 999999.) {
	      eventCont.hist("topMass")->Fill( catopTag->properties().topMass );
	      eventCont.hist("minMass")->Fill( catopTag->properties().minMass );
	      eventCont.hist("wMass")  ->Fill( catopTag->properties().wMass );
	    }
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

		  
   wPlusJets.print(cout);
   caTopHadronic.print(cout);

   // Histograms will be automatically written to the root file
   // specificed by command line options.

   // All done!  Bye bye.
   return 0;
}


