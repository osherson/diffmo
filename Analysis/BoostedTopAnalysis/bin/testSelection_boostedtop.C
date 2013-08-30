// -*- C++ -*-

// CMS includes
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"

#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 


#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>

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

   // Setup a style
   gROOT->SetStyle ("Plain");

   // Book those histograms!
   eventCont.add( new TH1F( "jetPt", "Jet p_{T};Jet p_{T} (GeV/c)", 60, 0, 3000) );
   eventCont.add( new TH1F( "topMass", "Top Mass;Top Mass (GeV/c^{2}", 100, 0, 500 ) );
   eventCont.add( new TH1F( "minMass", "Jet Min Mass;Min Mass (GeV/c^{2})", 100, 0, 200 ) );
   eventCont.add( new TH1F( "wMass",   "W Mass;W Mass (GeV/c^{2})", 100, 0, 200 ) );

   //////////////////////
   // //////////////// //
   // // Event Loop // //
   // //////////////// //
   //////////////////////

   for (eventCont.toBegin(); ! eventCont.atEnd(); ++eventCont) {
     //////////////////////////////////
     // Take What We Need From Event //
     //////////////////////////////////
     edm::Handle< vector< pat::Jet > > allJets;
     eventCont.getByLabel (edm::InputTag("selectedLayer1JetsTopTagPF"), allJets);
     
    if (!allJets.isValid() ) continue;

    for ( std::vector<pat::Jet>::const_iterator jetsBegin = allJets->begin(),
	    jetsEnd = allJets->end(),
	    ijet = jetsBegin;
	  ijet != jetsEnd; ++ijet ) {


      const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(ijet->tagInfo("CATopPFJet"));
      
      if ( catopTag !=0 && catopTag->properties().minMass != 999999.0 ) {

	eventCont.hist("jetPt")->Fill (ijet->pt());
	eventCont.hist("topMass")->Fill( catopTag->properties().topMass );
	eventCont.hist("minMass")->Fill( catopTag->properties().minMass );
	eventCont.hist("wMass")->Fill( catopTag->properties().wMass );
      }
    }     
     
   } // for eventCont

      
   ////////////////////////
   // ////////////////// //
   // // Clean Up Job // //
   // ////////////////// //
   ////////////////////////

   // Histograms will be automatically written to the root file
   // specificed by command line options.

   // All done!  Bye bye.
   return 0;
}


