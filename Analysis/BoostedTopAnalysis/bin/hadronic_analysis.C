#include "Analysis/BoostedTopAnalysis/interface/HadronicAnalysis.h"

#include "PhysicsTools/SelectorUtils/interface/RunLumiSelector.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

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


  std::cout << "Making run/lumi selector" << std::endl;
  RunLumiSelector runLumiSel( inputs );

  std::cout << "Making hadronic analysis object" << std::endl;
  HadronicAnalysis hadronicAnalysis( *parameters, theDir );

  std::vector<int> runs;


  cout << "About to begin looping" << endl;

  int nev = 0;
  int nev_passed = 0;
  //loop through each event
  for (ev.toBegin(); ! ev.atEnd(); ++ev, ++nev) {
    edm::EventBase const & event = ev;


    if ( ev.event()->size() == 0 ) continue; // skip trees with no events

    // Check good run list
    bool goodLumi = runLumiSel( event );
    if ( !goodLumi ) continue;

    ++nev_passed;

    int run = event.id().run();
    if ( find( runs.begin(), runs.end(), run ) == runs.end() )
      runs.push_back(run);

    if ( nev % 10000 == 0 ) cout << "Entry " << nev << ", Processing run " << event.id().run() << ", event " << event.id().event() << endl;

    // Analyze it
    hadronicAnalysis.analyze( event );
  } // end loop over events
    
  sort( runs.begin(), runs.end() );

  std::cout << "Processed " << nev_passed << " / " << nev << " events" << std::endl;
  std::cout << "List of runs processed: " << std::endl;

  for ( std::vector<int>::const_iterator ibegin = runs.begin(),
	  iend = runs.end(), i = ibegin;
	i != iend; ++i )
    std::cout << *i << std::endl;

  hadronicAnalysis.endJob();

  return 0;
}

