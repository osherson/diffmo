#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/MultiChainEvent.h"
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"

#include "Math/GenVector/PxPyPzM4D.h"

#include "TStopwatch.h"
#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

//Root includes
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"

using namespace std;

int main ( int argc, char ** argv )
{

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  // Get the python configuration
  PythonProcessDesc builder(argv[1], argc, argv);
  edm::ParameterSet const& shyftParameters = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("shyftAnalysis");
  edm::ParameterSet const& inputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet const& outputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("outputs");

  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );
  TFileDirectory theDir = fs.mkdir( "histos" ); 
    
  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::ChainEvent ev ( inputs.getParameter<std::vector<std::string> > ("fileNames") );

  //cout << "Making event selector" << endl;
  WPlusJetsEventSelector wPlusJets( shyftParameters );
  pat::strbitset ret = wPlusJets.getBitTemplate();
  

  unsigned int nEventsAnalyzed(0);

  // some timing
  TStopwatch timer;
  timer.Start();

  std::cout << "About to loop" << std::endl;
  //loop through each event
  for( ev.toBegin();
       ! ev.atEnd();
       ++ev) {
    ret.set(false);
    bool passed = wPlusJets(ev, ret);

    ++nEventsAnalyzed;

  } //end event loop

  timer.Stop();

  // print some timing statistics
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("Analyzed events: %d \n",nEventsAnalyzed);
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
  printf("%4.2f events / RealTime second .\n", nEventsAnalyzed/rtime);
  printf("%4.2f events / CpuTime second .\n", nEventsAnalyzed/ctime);


  
  //cout << "Printing" << endl;
  wPlusJets.print(std::cout);
  //cout << "We're done!" << endl;

  
  return 0;
}
