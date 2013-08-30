#ifndef Analysis_BoostedTopAnalysis_interface_MistagMaker_h
#define Analysis_BoostedTopAnalysis_interface_MistagMaker_h

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoissonQ.h"
#include "Analysis/JetAnalysis/interface/DijetSelector.h"
#include "Analysis/BoostedTopAnalysis/interface/BoostedTopWTagFunctor.h"


#include "TH1F.h"

using namespace std;

class MistagMaker {
  public :
    MistagMaker( const edm::ParameterSet & iConfig, TFileDirectory & iDir );
    virtual ~MistagMaker() {}
    virtual void beginJob() {}
    virtual void analyze( const edm::EventBase& iEvent ) ;
    virtual void endJob() { dijetSelector_.print(cout); }

  private :
    DijetSelector             dijetSelector_;

    TFileDirectory& theDir;
    std::map<std::string, TH1F*>     histograms1d;
    CLHEP::RandFlat *flatDistribution_;
    BoostedTopWTagFunctor     boostedTopWTagFunctor_;
    double      wMassMin_,  wMassMax_ ;
    double      ptMin_;

};


#endif
