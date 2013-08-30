#ifndef Analysis_BoostedTopAnalysis_interface_BoostedTopAnalysis_h
#define Analysis_BoostedTopAnalysis_interface_BoostedTopAnalysis_h

//#include "Analysis/BoostedTopAnalysis/interface/HadronicSelection.h"
#include "Analysis/BoostedTopAnalysis/interface/SemileptonicSelection.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopJetEventSelector.h"
#include "Analysis/BoostedTopAnalysis/interface/WPlusBJetEventSelector.h"
#include "Analysis/BoostedTopObjects/interface/BoostedTopEventHypothesis.h"


#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "Math/GenVector/PxPyPzM4D.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include <iostream>
#include <iomanip>
//#include <cmath>      //necessary for absolute function fabs()
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

//typedef std::vector<reco::ShallowClonePtrCandidate> ShallowCloneCollection;


class BoostedTopAnalysis 
{
 public:
  BoostedTopAnalysis();
  BoostedTopAnalysis(const edm::ParameterSet& iConfig , TFileDirectory & iDir, 
		     SemileptonicSelection& semiLepSel, 
		     CATopJetEventSelector& typeISel,
		     WPlusBJetEventSelector& typeIIandIIISel);
  virtual ~BoostedTopAnalysis() {}
  virtual void analyze(const edm::EventBase& iEvent);
  virtual void beginJob() {}
  virtual void endJob() {}
 private:
  //HadronicSelection   hadronicSelection_;
  SemileptonicSelection& semilepSelector;
  CATopJetEventSelector& CATopJetSelector;
  WPlusBJetEventSelector& WPlusBJetSelector;

  //the bitsets
  pat::strbitset hadType1_Side1_Ret;
  pat::strbitset hadType1_Side2_Ret;
  pat::strbitset hadType2_Side1_Ret;
  pat::strbitset hadType2_Side2_Ret;
  pat::strbitset semilepRet;
  
  // The event hypothesis!
  BoostedTopEventHypothesis evtHypothesis;

  TFileDirectory& theDir;
  // 'registry' for the histograms
  std::map<std::string, TH1F*> histograms;
  std::map<std::string, TH2F*> histograms2D;
  //std::vector<EventSummary>  summary;
};

#endif
