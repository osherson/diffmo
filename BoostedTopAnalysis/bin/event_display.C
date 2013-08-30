// -*- C++ -*-

// CMS includes
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "Analysis/BoostedTopAnalysis/interface/HadronicSelection.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"

#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 


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
   optutl::CommandLineParser parser ("Boosted Top All Hadronic Example");

   ////////////////////////////////////////////////
   // Change any defaults or add any new command //
   //      line options you would like here.     //
   ////////////////////////////////////////////////
   parser.stringValue ("outputFile") = "ttbsm_had"; // .root added automatically

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

   ////////////////////////////////////////
   // ////////////////////////////////// //
   // //         Begin Run            // //
   // // (e.g., book histograms, etc) // //
   // ////////////////////////////////// //
   ////////////////////////////////////////

   std::string tagName = "CATopCaloJet";

  // Tight jet id
  boost::shared_ptr<JetIDSelectionFunctor> jetIdTight      
    ( new JetIDSelectionFunctor( JetIDSelectionFunctor::CRAFT08, 
				 JetIDSelectionFunctor::TIGHT) );

   boost::shared_ptr<CATopTagFunctor> caTopTagFunctor
     ( new CATopTagFunctor( CATopTagFunctor::CALO, 
			    CATopTagFunctor::TIGHT,
			    tagName ) );

   HadronicSelection caTopHadronic(
     edm::InputTag("selectedLayer1JetsTopTagCalo"),
     edm::InputTag("patTriggerEvent"),
     jetIdTight,caTopTagFunctor,
     2,
     2,
     250,
     2.5,
     100., 250.,
     50.,
     0., 1000000.
     );

   // Setup a style
   gROOT->SetStyle ("Plain");

   // Book those histograms!
//    eventCont.add( new TH1F( "jetPt", "Jet p_{T};Jet p_{T} (GeV/c)", 60, 0, 3000) );

   //////////////////////
   // //////////////// //
   // // Event Loop // //
   // //////////////// //
   //////////////////////

   for (eventCont.toBegin(); ! eventCont.atEnd(); ++eventCont) {


     cout << "---------------------------" << endl;
     
     pat::strbitset ret = caTopHadronic.getBitTemplate();


    bool passed = caTopHadronic(eventCont, ret);
    vector<pat::Jet>      const & jets      = caTopHadronic.selectedJets();
    vector<pat::Jet>      const & tags      = caTopHadronic.taggedJets();

    if ( ret[std::string(">= 1 Tight Jet")] ) {
      cout << "Passed tight jet cuts" << endl;


      edm::Handle<TtGenEvent> ttGenEvent;
      eventCont.getByLabel( edm::InputTag("genEvt"), ttGenEvent);

      if ( ! ttGenEvent->isFullHadronic() ) continue;
      

      char buff[1000];
      sprintf(buff, "%6d : pt = %6.2f y = %6.2f phi = %6.2f",
	      ttGenEvent->hadronicDecayQuark()->pdgId(),
	      ttGenEvent->hadronicDecayQuark()->pt(),
	      ttGenEvent->hadronicDecayQuark()->rapidity(),
	      ttGenEvent->hadronicDecayQuark()->phi() );
      cout << buff << endl;
      sprintf(buff, "%6d : pt = %6.2f y = %6.2f phi = %6.2f",
	      ttGenEvent->lightQFromTop()->pdgId(),
	      ttGenEvent->lightQFromTop()->pt(),
	      ttGenEvent->lightQFromTop()->rapidity(),
	      ttGenEvent->lightQFromTop()->phi() );
      cout << buff << endl;
      sprintf(buff, "%6d : pt = %6.2f y = %6.2f phi = %6.2f",
	      ttGenEvent->lightQBarFromTop()->pdgId(),
	      ttGenEvent->lightQBarFromTop()->pt(),
	      ttGenEvent->lightQBarFromTop()->rapidity(),
	      ttGenEvent->lightQBarFromTop()->phi() );
      cout << buff << endl;

      sprintf(buff, "%6d : pt = %6.2f y = %6.2f phi = %6.2f",
	      ttGenEvent->hadronicDecayQuarkBar()->pdgId(),
	      ttGenEvent->hadronicDecayQuarkBar()->pt(),
	      ttGenEvent->hadronicDecayQuarkBar()->rapidity(),
	      ttGenEvent->hadronicDecayQuarkBar()->phi() );
      cout << buff << endl;
      sprintf(buff, "%6d : pt = %6.2f y = %6.2f phi = %6.2f",
	      ttGenEvent->lightQFromTopBar()->pdgId(),
	      ttGenEvent->lightQFromTopBar()->pt(),
	      ttGenEvent->lightQFromTopBar()->rapidity(),
	      ttGenEvent->lightQFromTopBar()->phi() );
      cout << buff << endl;
      sprintf(buff, "%6d : pt = %6.2f y = %6.2f phi = %6.2f",
	      ttGenEvent->lightQBarFromTopBar()->pdgId(),
	      ttGenEvent->lightQBarFromTopBar()->pt(),
	      ttGenEvent->lightQBarFromTopBar()->rapidity(),
	      ttGenEvent->lightQBarFromTopBar()->phi() );
      cout << buff << endl;
      
    }


   } // for eventCont

      
   ////////////////////////
   // ////////////////// //
   // // Clean Up Job // //
   // ////////////////// //
   ////////////////////////

   caTopHadronic.print(cout);

   // Histograms will be automatically written to the root file
   // specificed by command line options.

   // All done!  Bye bye.
   return 0;
}


