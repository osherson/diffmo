#include "Analysis/SHyFT/interface/SHyFT.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"

#include "TStopwatch.h"
#include "DataFormats/Provenance/interface/LuminosityBlockRange.h"

#include <iostream>
#include <iomanip>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "Math/GenVector/PxPyPzM4D.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

//Root includes
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"

/// THE ACTUAL PROGRAM
int main ( int argc, char ** argv )
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // Setup a style                                                                                                                                                       
  gROOT->SetStyle ("Plain");

  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  // Get the python configuration
  PythonProcessDesc builder(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> b = builder.processDesc();
  boost::shared_ptr<edm::ParameterSet> parameters = b->getProcessPSet();
  parameters->registerIt(); 
  edm::ParameterSet const& inputs  = parameters->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet const& outputs = parameters->getParameter<edm::ParameterSet>("outputs");

  bool selectLumis = false;
  std::vector<edm::LuminosityBlockRange> lumis;
  if ( inputs.exists("lumisToProcess") ) {
    selectLumis = true;

    std::vector<edm::LuminosityBlockRange> const & lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
    lumis.resize( lumisTemp.size() );
    copy( lumisTemp.begin(), lumisTemp.end(), lumis.begin() );
  }

  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );
  TFileDirectory theDir = fs.mkdir( "histos" ); 


  TH1F * fh = theDir.make<TH1F>("flavorHistory", "Flavor History", 15, 0, 15 );
  

    

  fwlite::ChainEvent ev ( inputs.getParameter<std::vector<std::string> > ("fileNames") );
  int maxEvents = inputs.getParameter<int>("maxEvents");

  unsigned int nEventsAnalyzed(0);

  // some timing
  TStopwatch timer;
  timer.Start();
  //loop through each event
  for( ev.toBegin();
       ! ev.atEnd() && ( (int)nEventsAnalyzed < maxEvents || maxEvents < 0) ;
       ++ev, ++nEventsAnalyzed) 
  {
    
    if ( ev.event()->size() == 0 ) continue; // skip trees with no events


    edm::Handle<unsigned int> flavorHistory_h;
    edm::EventBase const & event = ev;
    
    event.getByLabel( edm::InputTag("flavorHistoryFilter"), flavorHistory_h);

    edm::Handle<std::vector<reco::GenParticle> > gen_h;
    event.getByLabel( edm::InputTag("prunedGenParticles"), gen_h );

    bool isW = false;
    
    if ( flavorHistory_h.isValid() && gen_h.isValid() ) {
      // std::cout << "Flavor history = " << *flavorHistory_h << std::endl;
      for ( std::vector<reco::GenParticle>::const_iterator igen = gen_h->begin(),
	      iend = gen_h->end(); igen != iend && !isW; ++igen ) {
	if ( fabs(igen->pdgId()) == 24 ) isW = true;
      }
      if ( isW )
	fh->Fill( *flavorHistory_h );
    }


    if (nEventsAnalyzed%10000==0) std::cout<<"Events analyzed: "<<nEventsAnalyzed<<std::endl;
  } //end event loop
  

  timer.Stop();

  // print some timing statistics
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("Analyzed events: %d \n",nEventsAnalyzed);
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
  printf("%4.2f events / RealTime second .\n", nEventsAnalyzed/rtime);
  printf("%4.2f events / CpuTime second .\n", nEventsAnalyzed/ctime);
  
  return 0;
}
