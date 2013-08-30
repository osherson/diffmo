#ifndef Analysis_BoostedTopAnalysis_interface_CombinedQCDEstimation_h
#define Analysis_BoostedTopAnalysis_interface_CombinedQCDEstimation_h

#include "Analysis/BoostedTopAnalysis/interface/Type11Selection_v1.h"
#include "Analysis/BoostedTopAnalysis/interface/Type22Selection_v1.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoissonQ.h"
#include "Analysis/BoostedTopAnalysis/interface/PredictedDistribution.h"

#include "TH1F.h"
#include "TFile.h"

class CombinedQCDEstimation {
  public :
    CombinedQCDEstimation( const edm::ParameterSet & iConfig, TFileDirectory & iDir );
    virtual ~CombinedQCDEstimation() { } //delete flatDistribution_; 
    virtual void beginJob() {}
    virtual void analyze( const edm::EventBase& iEvent ) ;
    virtual void endJob() {  
      type22Selection_v1_.print(std::cout);
      theDir.cd(); 
      ttMassPred11    ->    SetCalculatedErrors();
      ttMassPred12    ->    SetCalculatedErrors();
      ttMassPred22    ->    SetCalculatedErrors();
      ttMassPred11    ->    GetPredictedHist()->Write();
      ttMassPred12    ->    GetPredictedHist()->Write();
      ttMassPred22    ->    GetPredictedHist()->Write();
    }

  private :
    TFileDirectory& theDir;
    Type22Selection_v1   type22Selection_v1_;
    Type11Selection_v1   type11Selection_v1_;
    double              bTagOP_;
    std::string         bTagAlgo_;
    BoostedTopWTagFunctor   *        wJetSelector_;
    double              wMassMin_, wMassMax_;
    double              topMassMin_, topMassMax_;
    std::string         mistagFileName_;
    TFile *             mistagFile_;
    TH1F  *             wMistag_;
    TH1F  *             bMistag_;
    CLHEP::RandFlat *flatDistribution_;
    double              prob;
    bool                runOnData_;
    PredictedDistribution * ttMassPred11;
    PredictedDistribution * ttMassPred12;
    PredictedDistribution * ttMassPred22;
    double 				caTopJetMassMin_;
    double 				caTopJetMassMax_;
    double 				caTopMinMassMin_;
    std::string             caTopMistagFileName_;
    TFile *             caTopMistagFile_;
    TH1F  *             topMistag_;
    double caTopJetPtMin_;
    double caTopJetEtaCut_;
    double jetPt0_;
    double jetPt1_;
    double jetEta_;
};


#endif
