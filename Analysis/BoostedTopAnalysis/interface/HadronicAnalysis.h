#ifndef Analysis_BoostedTopAnalysis_interface_HadronicAnalysis_h
#define Analysis_BoostedTopAnalysis_interface_HadronicAnalysis_h

#include "Analysis/BoostedTopAnalysis/interface/HadronicSelection.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "Math/GenVector/PxPyPzM4D.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoissonQ.h"
#include "FWCore/Utilities/interface/Exception.h"

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


class HadronicAnalysis {

  public:


  struct EventSummary {
    double mjj;
    double mu0, mu1, y0, y1;
    unsigned int run;
    unsigned int event;
    unsigned int luminosityBlock;

  EventSummary( double imjj, 
		double imu0, double imu1, double iy0, double iy1,
		unsigned int irun, unsigned int ievent, unsigned int iluminosityBlock) :
    mjj(imjj), mu0(imu0), mu1(imu1), y0(iy0), y1(iy1),
      run(irun), event(ievent), luminosityBlock(iluminosityBlock)
    {
    }
    
    friend std::ostream & operator<<( std::ostream & out, const EventSummary & e ) {
      out << "Run " << e.run << ", lumi " << e.luminosityBlock << ", event " << e.event << ", mjj = " << e.mjj
	  << " mu0 = " << e.mu0 << ", mu1 = " << e.mu1 << ", y0 = " << e.y0 << ", y1 = " << e.y1;
      return out;
    };
    
    bool operator< (  const EventSummary & e) const {
      return this->mjj < e.mjj;
    };
  };
  
  HadronicAnalysis(const edm::ParameterSet& iConfig, TFileDirectory& iDir);
    virtual ~HadronicAnalysis() {}
    virtual void analyze(const edm::EventBase& iEvent);
    virtual void beginJob() {}
    virtual void endJob() {
      hadronicSelection_.print(std::cout);
      sort( summary.begin(), summary.end() );
      int max = (summary.size() < 100) ? summary.size() : 100;
      for ( std::vector<EventSummary>::const_reverse_iterator ibegin = summary.rbegin(),
	      iend = ibegin + max, i = ibegin;
	    i != iend; ++i ) {
	std::cout << *i << std::endl;
      }
    }
    void analyzeWJet(const edm::EventBase&);

  private:
    bool                plotTracks_;
    double              histoWeight_;
    TH1F*               weightHist_;
    HadronicSelection   hadronicSelection_;
    TFileDirectory& theDir;
    // 'registry' for the histograms                                                                                                                                                                    
    BoostedTopWTagFunctor   boostedTopWTagFunctor_;

    std::map<std::string, TH1F*> histograms1d;
    std::map<std::string, TH2F*> histograms2d;
    double jetPtMin_;
    double wMinMass_;
    double wMaxMass_;
    double mu_, y_;
    double bTagMedium_, bTagLoose_;
    int eventCount_;
    std::string mistagFileName_;
    TFile * mistagFile_;
    TH1 * bMistagMO_, * bMistagME_;
    TH1 * wMistagO_, * wMistagE_;
    bool  onePV_;
    std::vector<double>     vMu;
    std::vector<std::string>  vWp;

    std::vector<EventSummary>  summary;
    CLHEP::RandFlat *flatDistribution_;
};

#endif
