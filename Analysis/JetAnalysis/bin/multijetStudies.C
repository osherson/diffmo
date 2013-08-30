/*   A macro for making a histogram of Jet Pt with cuts
     This is a basic way to cut out jets of a certain Pt and Eta using an if statement
     This example creates a histogram of Jet Pt, using Jets with Pt above 30 and ETA above -2.1 and below 2.1
*/

#include "Analysis/JetAnalysis/interface/MultijetAnalyzer.h"

#include "PhysicsTools/SelectorUtils/interface/RunLumiSelector.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()

using namespace std;


///-------------------------
/// DRIVER FUNCTION
///-------------------------

// -*- C++ -*-

// CMS includes
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "Analysis/JetAnalysis/interface/DijetSelector.h"

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


  edm::ParameterSet inputs              = parameters->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet outputs             = parameters->getParameter<edm::ParameterSet>("outputs");

  cout << "setting up TFileService" << endl;
  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );
  TFileDirectory theDir = fs.mkdir( "histos" ); 
    
  cout << "Setting up chain event" << endl;
  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::ChainEvent ev ( inputs.getParameter<std::vector<std::string> > ("fileNames") );


  RunLumiSelector runLumiSel( inputs );

  MultijetAnalyzer multijetAna( *parameters, theDir );




  cout << "About to begin looping" << endl;

  int nev = 0;
  //loop through each event
  for (ev.toBegin(); ! ev.atEnd(); ++ev, ++nev) {
    edm::EventBase const & event = ev;


    if ( ev.event()->size() == 0 ) continue; // skip trees with no events

    // Check good run list
    bool goodLumi = runLumiSel( event );
    if ( !goodLumi ) continue;

    int run = event.id().run();
    if ( nev % 10000 == 0 ) cout << "Entry " << nev << ", Processing run " << event.id().run() << ", event " << event.id().event() << endl;

    // Analyze it
    multijetAna.analyze( event );
  } // end loop over events
    


  multijetAna.endJob();

  return 0;
}

