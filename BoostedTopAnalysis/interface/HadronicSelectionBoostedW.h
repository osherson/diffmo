#ifndef Analysis_BoostedTopAnalysis_interface_HadronicSelection_h_h
#define Analysis_BoostedTopAnalysis_interface_HadronicSelection_h_h

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"
#include "TLorentzVector.h"

class HadronicSelectionBoostedW : public Selector<fwlite::EventContainer> {
 public:

  typedef std::vector<pat::Jet>::const_iterator    value_type;
  typedef std::vector<value_type>                  collection_type;


  HadronicSelectionBoostedW(
   boost::shared_ptr<WPlusJetsEventSelector> wPlusJets,
   edm::InputTag const & jetTag,
   edm::InputTag const & trigTag,
   double jetPtMin,
   double jetEtaMax,
   double mu,
   double ycut,
   double dPhi
   );

  ~HadronicSelectionBoostedW() {}


  virtual bool operator()( fwlite::EventContainer const & eventConst, pat::strbitset & ret);  

  collection_type      const & selectedJets     () const { return selectedJets_;     } 
  collection_type      const & taggedJets       () const { return taggedJets_;       } 
 
 protected: 
  boost::shared_ptr<WPlusJetsEventSelector> wPlusJets_;
  edm::InputTag               jetTag_;
  edm::InputTag               trigTag_;

  collection_type             selectedJets_;
  collection_type             taggedJets_;

  double jetPtMin_;
  double jetEtaMax_;
  double mu_;         // mass drop
  double ycut_;       // asymmetry veto
  double dPhi_;       // delta phi between leading muon, and leading jet

  
};


#endif
