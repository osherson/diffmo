#ifndef Analysis_BoostedTopAnalysis_interface_Type12Analysis_h
#define Analysis_BoostedTopAnalysis_interface_Type12Analysis_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "TH1F.h"
#include "TFile.h"

using namespace std;

class Type12Analysis  {
  public :
    Type12Analysis( const edm::ParameterSet & iConfig,  TFileDirectory & iDir );
    virtual ~Type12Analysis() {}
    virtual void beginJob() {}
    virtual void analyze( const edm::EventBase& iEvent ) ;
    virtual void endJob() {}

  private :
    TFileDirectory& theDir;
    std::map<std::string, TH1F*> histograms1d;
    edm::ParameterSet const &   pfJetIdParams_;
    boost::shared_ptr<PFJetIDSelectionFunctor>   pfJetSel_;
    double    bTagOP_;
    TFile   *   mistagFile_;
    TH1F    *   bMistag_;

};

#endif 
