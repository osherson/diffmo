#ifndef Analysis_SHyFT_interface_SHyFT_h
#define Analysis_SHyFT_interface_SHyFT_h

#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"
#include "Analysis/SHyFT/interface/SHyFTSelector.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "Math/GenVector/PxPyPzM4D.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Record.h"
/* #include "DataFormats/FWLite/interface/EventSetup.h" */
/* #include "DataFormats/FWLite/interface/ESHandle.h" */
/* #include "CondFormats/PhysicsToolsObjects/interface/BinningPointByMap.h" */
/* #include "RecoBTag/PerformanceDB/interface/BtagPerformance.h" */


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

typedef std::vector<reco::ShallowClonePtrCandidate> ShallowCloneCollection;

class SHyFT : public edm::BasicAnalyzer {

  public:

  struct SHyFTSummary {
    SHyFTSummary( unsigned int irun=0,
		  unsigned int ilumiSec=0,
		  unsigned int ievent=0,
		  unsigned int inJets=0,
		  unsigned int inTags=0 ) :
    run(irun), lumiSec(ilumiSec), event(ievent), nJets(inJets), nTags(inTags)
    {
      if ( nJets < nTags ) {
	throw cms::Exception("InvalidCode") << " This says there are more tags than jets."
					    << " This is nonsense." <<std::endl;
      }
    }
    unsigned int run;
    unsigned int lumiSec;
    unsigned int event;
    unsigned int nJets;
    unsigned int nTags;

    friend std::ostream & operator<<(std::ostream & out, SHyFTSummary const & r ) {
      char buff[1000];
      sprintf(buff, "%12d , %12d, %20d, %5d, %5d", r.run, r.lumiSec, r.event, r.nJets, r.nTags );
      out << buff;
      return out;
    }

    bool operator<( SHyFTSummary const & r ) const {
      if ( run < r.run ) return true;
      else if ( run > r.run ) return false;
      else { // equal runs
	if ( lumiSec < r.lumiSec ) return true;
	else if ( lumiSec > r.lumiSec ) return false;
	else { // equal lumi sections
	  if ( event < r.event ) return true;
	  else if ( event > r.event ) return false;
	  else return false;
	}
      }
    }
  };

    SHyFT(const edm::ParameterSet& iConfig, TFileDirectory& iDir);
    virtual ~SHyFT() {     
    }
    virtual void analyze(const edm::EventBase& iEvent);
    virtual void beginJob() {}
    virtual void endJob();

  private:
    bool analyze_electrons(const std::vector<reco::ShallowClonePtrCandidate>& electrons);
    bool analyze_muons(const std::vector<reco::ShallowClonePtrCandidate>& muons);    
    bool analyze_met( const reco::ShallowClonePtrCandidate & met );
    //std::string calcSampleName (const edm::EventBase& iEvent);
    void weightPDF( edm::EventBase const & iEvent );

    bool make_templates(const std::vector<reco::ShallowClonePtrCandidate>& jets,
			const reco::ShallowClonePtrCandidate & met,
			const std::vector<reco::ShallowClonePtrCandidate>& muons,
			const std::vector<reco::ShallowClonePtrCandidate>& electrons
			);

    SHyFTSelector wPlusJets;
    TFileDirectory& theDir;

    TFileDirectory  subdirMU_plus;
    TFileDirectory  subdirMU_minus;

    TFileDirectory  subdirEB_plus;
    TFileDirectory  subdirEE_plus;
    
    TFileDirectory  subdirEB_minus;
    TFileDirectory  subdirEE_minus;

    // the following parameters need to come from the config
    bool muPlusJets_;
    bool ePlusJets_;
    bool useHFcat_;
    int nJetsCut_ ;
    int mode;
    std::string sampleNameInput;
    // used to be a global, what a shit!
    int HFcat_;
    std::string sampleHistName_;
    bool doMC_;
    std::string identifier_;

    int allNumTags_;
    int allNumJets_;
    std::vector<SHyFTSummary> summary_;

    double globalWeight_;  // For reweighting the entire event for, e.g., pdf reweighting
    bool reweightPDF_;
    bool reweightBTagEff_;
    edm::InputTag pdfInputTag_; 
    std::string pdfToUse_;
    int         pdfEigenToUse_;
    int         pdfVariation_;


    std::string btaggerString_;
    double bcEffScale_;
    double lfEffScale_;
    double allDiscrCut_;
    double bDiscrCut_;
    double cDiscrCut_;
    double lDiscrCut_;
    bool useCustomPayload_;
    std::string customTagRootFile_;
    bool simpleSFCalc_; // Use the simple scale factor calculation where you just treat the SF
                        // as a probability to throw away an MC event. Only works for SF < 1.0.
                        // Technically, for systematics, SF < 1-dSF, since then you'd set this 
                        // up and down by dSF. 
    bool weightSFCalc_; 
    std::string jetAlgo_;
    boost::shared_ptr<TFile> customBtagFile_;


    // for closure test of tag rates
    double nExpectedTaggedJets_;
    double nObservedTaggedJets_;
    double nExpectedTaggedEvents_;
    double nObservedTaggedEvents_;
};


#endif
