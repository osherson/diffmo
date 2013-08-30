#ifndef Analysis_SHyFT_interface_JetStudies2011_h
#define Analysis_SHyFT_interface_JetStudies2011_h

#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "Math/GenVector/PxPyPzM4D.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Record.h"
#include "PhysicsTools/SelectorUtils/interface/PFMuonSelector.h"
/* #include "PhysicsTools/Utilities/interface/LumiWeighting.h" */
/* #include "DataFormats/FWLite/interface/EventSetup.h" */
/* #include "DataFormats/FWLite/interface/ESHandle.h" */
/* #include "CondFormats/PhysicsToolsObjects/interface/BinningPointByMap.h" */
/* #include "RecoBTag/PerformanceDB/interface/BtagPerformance.h" */
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"


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
#include "TH3.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"

class JetStudies2011 : public edm::BasicAnalyzer {

  public:
    JetStudies2011(const edm::ParameterSet& iConfig, TFileDirectory& iDir);
    virtual ~JetStudies2011() {     
    }
    virtual void analyze(const edm::EventBase& iEvent);
    virtual void beginJob() {}
    virtual void endJob() {}

  private:


    TFileDirectory theDir;
    std::vector<TFileDirectory> dirs_;

    edm::InputTag   jetSrc_;          /// jets
    edm::InputTag   rhoSrc_;          /// mean pt per unit area
    edm::InputTag   pvSrc_;           /// primary vertex
    edm::InputTag   trigSrc_;         /// trigger
    edm::InputTag   genJetsSrc_;      /// gen jets to input
    bool            useCA8GenJets_;   /// use CA8 instead of AK5 gen jets
    bool            weightPV_;        /// weight events by the PV
    std::vector<std::string>    jecPayloads_; /// files for JEC payloads
    std::vector<std::string> trigs_;  /// triggers to consider
    bool            binPtTrig_;       /// bin triggers in pt
    std::vector<double>  ptTrigBins_; /// pt bins for triggers
    bool            useBTags_;        /// require btags
    bool            orderByMass_;     /// order jets by mass rather than pt
    // Optional: If using btags, also check a muon-in-jet selector
    // since this comes in on a mu trigger. 
    edm::InputTag   muonSrc_;         /// muons
    double          rCut_;            /// rCut for deltaR(mu,jet) for tagging
    boost::shared_ptr<PFMuonSelector> pfMuonSelector_; /// muon id selector

    boost::shared_ptr<JetCorrectionUncertainty> jecUnc_;
    boost::shared_ptr<FactorizedJetCorrector> jec_;
    

    /* boost::shared_ptr<edm::LumiWeighting> lumiWeighting_; */
};


#endif
