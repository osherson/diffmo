// -*- C++ -*-

// CMS includes
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 


#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>

#include "PhysicsTools/FWLite/interface/TFileService.h"

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
   optutl::CommandLineParser parser ("Boosted Top Efficiency Calculation");

   ////////////////////////////////////////////////
   // Change any defaults or add any new command //
   //      line options you would like here.     //
   ////////////////////////////////////////////////
   parser.stringValue ("outputFile") = "ttbsm_catop_eff"; // .root added automatically

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

   // create the TFileService instance and a map for the histograms
   boost::shared_ptr<fwlite::TFileService> fs( new fwlite::TFileService("eff_catoptag.root") );
   std::map<std::string, TH1*> hist;

   // Book those histograms!

   hist["genJetPt"] = fs->make<TH1F>( "genJetPt", "Gen Jet p_{T};Gen Jet p_{T} (GeV/c)", 60, 0, 3000 );
   hist["genJetY"]  = fs->make<TH1F>("genJetY", "Gen Jet Rapidity;Gen Jet Y", 60, -3.0, 3.0);
 
   hist["genJetPtReco"] = fs->make<TH1F>("genJetPtReco", "Gen Jet p_{T}, Matched to RECO Jet;Gen Jet p_{T} (GeV/c)", 60, 0, 3000);
   hist["genJetYReco"]  = fs->make<TH1F>("genJetYReco", "Gen Jet Rapidity, Matched to RECO Jet;Gen Jet Y", 60, -3.0, 3.0);

   hist["genJetPtTag"] = fs->make<TH1F>("genJetPtTag", "Gen Jet p_{T}, Matched to Tagged Jet;Gen Jet p_{T} (GeV/c)", 60, 0, 3000);
   hist["genJetYTag"]  = fs->make<TH1F>("genJetYTag", "Gen Jet Rapidity, Matched to Tagged Jet;Gen Jet Y", 60, -3.0, 3.0);

   hist["jetPtReco"] = fs->make<TH1F>("jetPtReco", "Jet p_{T};Jet p_{T} (GeV/c)", 60, 0, 3000);
   hist["jetYReco"]  = fs->make<TH1F>("jetYReco", "Jet Rapidity;Jet Y", 60, -3.0, 3.0);

   hist["jetPtTag"] = fs->make<TH1F>("jetPtTag", "Tagged Jet p_{T};Jet p_{T} (GeV/c)", 60, 0, 3000);
   hist["jetYTag"]  = fs->make<TH1F>("jetYTag", "Tagged Jet Rapidity;Jet Y", 60, -3.0, 3.0);

   hist["topMass"] = fs->make<TH1F>("topMass", "Top Mass;Top Mass (GeV/c^{2}", 100, 0, 500 );
   hist["minMass"] = fs->make<TH1F>("minMass", "Jet Min Mass;Min Mass (GeV/c^{2})", 100, 0, 200 );
   hist["wMass"]   = fs->make<TH1F>("wMass",   "W Mass;W Mass (GeV/c^{2})", 100, 0, 200 ); 

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
     eventCont.getByLabel (edm::InputTag("selectedLayer1JetsTopTagCalo"), allJets);

     edm::Handle< vector< reco::GenJet > > genJets;
     eventCont.getByLabel (edm::InputTag("ca8GenJets"), genJets);
     
     if ( !genJets.isValid() ) continue;
     if (!allJets.isValid() ) continue;

     // Loop over the gen jets reconstructed with CA D = 0.8
     for( std::vector<reco::GenJet>::const_iterator genJetsBegin = genJets->begin(),
	    genJetsEnd = genJets->end(), igen = genJetsBegin;
	  igen != genJetsEnd; ++igen ) {


       // Make sure we have a jet with pt > 30
       if ( igen->pt() > 100 ) {
	 
	 hist["genJetPt"]->Fill( igen->pt() );
	 hist["genJetY"]->Fill( igen->rapidity() );



	 // Find a matched reco jet
	 std::vector<pat::Jet>::const_iterator found = allJets->end();    
	 double dRMin = 0.8;
	 for ( std::vector<pat::Jet>::const_iterator jetsBegin = allJets->begin(),
		 jetsEnd = allJets->end(),
		 ijet = jetsBegin;
	       ijet != jetsEnd; ++ijet ) {
	   double dR = reco::deltaR<double>( igen->rapidity(), igen->phi(), ijet->rapidity(), ijet->phi() ) ;
	   if ( dR < dRMin ) {
	     dRMin = dR;
	     found = ijet;
	   }
	 }
	 
	 // IF we've found a matched reco jet
	 if ( found != allJets->end() ) {

	   hist["genJetPtReco"]->Fill( igen->pt() );
	   hist["genJetYReco"]->Fill( igen->rapidity() );

	   hist["jetPtReco"]->Fill( found->pt() );
	   hist["jetYReco"]->Fill( found->rapidity() );
	   
	   const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(found->tagInfo("CATopCaloJet"));
	   
	   if ( catopTag !=0 && catopTag->properties().minMass != 999999.0 ) {
	     
	     hist["topMass"]->Fill( catopTag->properties().topMass );
	     hist["minMass"]->Fill( catopTag->properties().minMass );
	     hist["wMass"]->Fill( catopTag->properties().wMass );

	     // If we've found a tagged jet
	     if ( catopTag->properties().topMass >= 100. && catopTag->properties().topMass <= 250. &&
		  catopTag->properties().minMass >= 50.) {
	       hist["genJetPtTag"]->Fill( igen->pt() );
	       hist["genJetYTag"]->Fill( igen->rapidity() );

	       hist["jetPtTag"]->Fill( found->pt() );
	       hist["jetYTag"]->Fill( found->rapidity() );
	       
	     }// end if tagged the jet	     
	   }// end if reconstructed tag
	 } // end if found a jet
       }// end if gen pt > 100
     }// end loop over gen jets
     
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


