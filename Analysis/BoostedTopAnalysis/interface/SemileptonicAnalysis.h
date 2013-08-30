#ifndef Analysis_BoostedTopAnalysis_interface_SemileptonicAnalysis_h
#define Analysis_BoostedTopAnalysis_interface_SemileptonicAnalysis_h

#include "Analysis/BoostedTopAnalysis/interface/SemileptonicSelection.h"
#include "Analysis/BoostedTopAnalysis/interface/BoostedTopWTagFunctor.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "Math/GenVector/PxPyPzM4D.h"
#include "DataFormats/FWLite/interface/Handle.h"


#include <iostream>
#include <iomanip>
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

typedef std::vector<reco::ShallowClonePtrCandidate> ShallowCloneCollection;


class SemileptonicAnalysis {

  public:

  
  SemileptonicAnalysis(const edm::ParameterSet& iConfig, TFileDirectory& iDir);
    virtual ~SemileptonicAnalysis() {}
    virtual void analyze(const edm::EventBase& iEvent);
    virtual void beginJob() {}
    virtual void endJob() {
      semileptonicSelection_.print(std::cout);
      boostedTopWTagFunctor_.print(std::cout);
    }

  private:
    SemileptonicSelection   semileptonicSelection_;
    BoostedTopWTagFunctor   boostedTopWTagFunctor_;
    TFileDirectory& theDir;
    // 'registry' for the histograms                                                                                                                                                                    
    std::map<std::string, TH1F*> histograms1d;
    std::map<std::string, TH2F*> histograms2d;

};

#endif
