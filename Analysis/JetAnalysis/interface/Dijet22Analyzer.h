#ifndef Analysis_JetAnalysis_interface_Dijet22Analyzer_h
#define Analysis_JetAnalysis_interface_Dijet22Analyzer_h

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TLorentzVector.h"


#include "Analysis/JetAnalysis/interface/SimpleSubstructureSelector.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "DataFormats/Math/interface/deltaPhi.h"

class Dijet22Analyzer {

  public:

  struct EventSummary {
  EventSummary( double ihT, unsigned int irun, unsigned int ievent, unsigned int iluminosityBlock) :
    hT(ihT), run(irun), event(ievent), luminosityBlock(iluminosityBlock)
    {
    }
    double hT;
    unsigned int run;
    unsigned int event;
    unsigned int luminosityBlock;

    friend std::ostream & operator<<( std::ostream & out, const EventSummary & e ) {
      out << "Run " << e.run << ", lumi " << e.luminosityBlock << ", event " << e.event << ", HT = " << e.hT << std::endl;
      return out;
    };

    bool operator< (  const EventSummary & e) const {
      return this->hT < e.hT;
    };
  };

    Dijet22Analyzer(const edm::ParameterSet& iConfig, TFileDirectory& iDir);
    virtual ~Dijet22Analyzer() {}
    virtual void beginJob() {}
    virtual void analyze(const edm::EventBase& iEvent);
    virtual void endJob();

  private:


    // Selectors : we're interested in 
    // a dijet "2 + 2" subjet topology
    SimpleSubstructureSelector dijet22Selector_;

    // Directory for histograms
    TFileDirectory& theDir;
    TFileDirectory dir2;
    TFileDirectory dir3;
    TFileDirectory dir4;
    TFileDirectory dir5;
    TFileDirectory dir6;
    std::vector<EventSummary>   summary;

    // Helper to fill histograms
    void fillHistos( TFileDirectory & dir );
};


#endif
