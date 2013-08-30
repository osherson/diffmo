//#include "Analysis/BoostedTopAnalysis/interface/HadronicAnalysis.h"
#include "TSystem.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"
//#include "Analysis/BoostedTopAnalysis/interface/HadronicAnalysis.h"
#include "Analysis/BoostedTopAnalysis/interface/BoostedTopWTagFunctor.h"
#include "Analysis/BoostedTopAnalysis/interface/SemileptonicSelection.h"
#include "TLorentzVector.h"
#include "TH2F.h"

#include "PhysicsTools/SelectorUtils/interface/RunLumiSelector.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

#include "Analysis/BoostedTopAnalysis/interface/SubjetHelper.h"

#include "Analysis/BoostedTopAnalysis/interface/SemileptonicAnalysis.h"

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

  if ( argc < 2)
    {
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
  string outName = outputs.getParameter<std::string>("outputName");
  string dirName = "histos";
  fwlite::TFileService fs = fwlite::TFileService(outName);
  TFileDirectory theDir = fs.mkdir( dirName.c_str() ); 

  SemileptonicAnalysis semilepAna( parameters->getParameter<edm::ParameterSet>("semileptonicAnalysis"), theDir );
    
  cout << "Setting up chain event... ";
  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::ChainEvent ev ( inputs.getParameter<std::vector<std::string> > ("fileNames") );
  cout <<"Done!\n";

  semilepAna.beginJob();

  int nev = 0;
  //loop through each event
  for (ev.toBegin(); ! ev.atEnd(); ++ev, ++nev) 
   {
      //if( nev == 150000) break;
     //if(nev==50000) break;

      edm::EventBase const & event = ev;
      if ( ev.event()->size() == 0 ) continue; // skip trees with no events
      if ( nev % 10000 == 0 ) cout << "Entry " << nev << ", Processing run " << event.id().run() << ", event " << event.id().event() << endl;
      semilepAna.analyze( event );
	
   }// for (events)
  semilepAna.endJob();

  return 0;
}
