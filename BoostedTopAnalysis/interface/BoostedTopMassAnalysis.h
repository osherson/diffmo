#ifndef Analysis_BoostedTopAnalysis_interface_BoostedTopMassAnalysis_h
#define Analysis_BoostedTopAnalysis_interface_BoostedTopMassAnalysis_h

#include "Analysis/SHyFT/interface/SHyFTPFSelector.h"
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


class BoostedTopMassAnalysis {

  public:

  
  BoostedTopMassAnalysis(const edm::ParameterSet& iConfig, TFileDirectory& iDir);
    virtual ~BoostedTopMassAnalysis() {}
    virtual void analyze(const edm::EventBase& iEvent);
    virtual void beginJob() {}
    virtual void endJob() {
      shyftSelector_.print(std::cout);
      boostedTopWTagFunctor_.print(std::cout);
    }

  private:
    SHyFTPFSelector         shyftSelector_;
    BoostedTopWTagFunctor   boostedTopWTagFunctor_;
    pat::strbitset          semilepRet_;
    pat::strbitset::index_type passAllSemilep_;
    pat::strbitset          wtagRet_;
    TFileDirectory& theDir;
};

#endif
