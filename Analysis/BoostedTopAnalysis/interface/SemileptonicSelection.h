#ifndef Analysis_BoostedTopAnalysis_interface_SemileptonicSelection_h_h
#define Analysis_BoostedTopAnalysis_interface_SemileptonicSelection_h_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "Analysis/JetAnalysis/interface/DijetSelector.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"
#include "Analysis/BoostedTopAnalysis/interface/BoostedTopWTagFunctor.h"


class SemileptonicSelection : public EventSelector {
 public:
  typedef  reco::ShallowClonePtrCandidate candidate;
  typedef  std::vector<candidate> candidate_collection;
  SemileptonicSelection( edm::ParameterSet const & params );

  ~SemileptonicSelection() {}
  
  virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);

  using EventSelector::operator();
  candidate_collection const & taggedJets () const { return wPlusJets().selectedJets(); } 
  candidate_collection const & taggedMuons() const { return wPlusJets().selectedMuons();}
  candidate            const & taggedMETs () const { return wPlusJets().selectedMET(); }
  pat::strbitset const & getWPlusJetsBitSet() const {return retSemi; }
  //pat::strbitset const & getHadronicBitSet() const {return retHad; }

  std::vector<int> getNJetsInRegion() const { std::vector<int> vec(3,0); 
    //vec.reserve(3);
    vec[0] = nJetsA;
    vec[1] = nJetsB;
    vec[2] = nJetsC;
    return vec;}
  candidate_collection::const_iterator getWJet() const {return wJet;}
  candidate_collection::const_iterator getClosestJet() const {return closestJet;}

  virtual void print( std::ostream & out )
  {
    std::cout <<"----- SemileptonicSelection ---\n";
    EventSelector::print(out);
    std::cout <<"----- W + Jets ----------------\n";
    wPlusJets_.print(out);
    //std::cout <<"----- boosted w top tagger ----\n";
    //boostedTopWTagFunctor_.print(out);
  }

  WPlusJetsEventSelector const & wPlusJets() const { return wPlusJets_;}
 
 protected: 
  // selected bitsets
  pat::strbitset retSemi;
  //pat::strbitset retHad;
  pat::strbitset retInt;
  //selectors 
  WPlusJetsEventSelector wPlusJets_;
  //BoostedTopWTagFunctor boostedTopWTagFunctor_;

  // selected objets
  candidate_collection taggedMuons_;
  candidate_collection taggedJets_;
  candidate taggedMETs_;
  candidate_collection::const_iterator wJet;
  candidate_collection::const_iterator closestJet;
  // calculated values from objs
  double dRMin;
  int nJetsA, nJetsB, nJetsC;
  //std::vector<int> nJetsInRegion;

  // cutvals
  const edm::InputTag jetSrc;
  const double ptRelMin;
  const double dRMinCut;
  const double anyLeadJetPt;
  const double oppLeadJetPt;
  const double htLepCut;
  const std::string bDiscriminator;
  const double bDiscriminatorCut;

};


#endif
