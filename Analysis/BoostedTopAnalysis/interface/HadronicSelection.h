#ifndef Analysis_BoostedTopAnalysis_interface_HadronicSelection_h_h
#define Analysis_BoostedTopAnalysis_interface_HadronicSelection_h_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "Analysis/JetAnalysis/interface/DijetSelector.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"
#include "Analysis/BoostedTopAnalysis/interface/BoostedTopWTagFunctor.h"

class HadronicSelection : public EventSelector {
 public:
  HadronicSelection( edm::ParameterSet const & params );

  ~HadronicSelection() {}
  
  virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);

  using EventSelector::operator();

  std::vector<edm::Ptr<pat::Jet> >      const & pretaggedJets     () const { 
    return  !usePF_ ? dijetSelector_.caloJets() : dijetSelector_.pfJets() ;     
  } 

  std::vector<edm::Ptr<pat::Jet> >      const & taggedJets       () const { return taggedJets_;       } 

  virtual void print( std::ostream & out ){
    EventSelector::print(out);
    dijetSelector_.print(out);
    caTopTagFunctor_.print(out);
    boostedTopWTagFunctor_.print(out);
  }
 
 protected: 
  DijetSelector             dijetSelector_;
  CATopTagFunctor           caTopTagFunctor_;
  BoostedTopWTagFunctor     boostedTopWTagFunctor_;
  edm::InputTag             pvSrc_;
  edm::InputTag             trigSrc_;
  std::string               trig_;
  int                       minTags_;
  bool                      usePF_; 
  bool                      useWTag_;
  std::vector<edm::Ptr<pat::Jet> >             taggedJets_;
};


#endif
