#ifndef Analysis_BoostedTopAnalysis_interface_CATopJetEventSelector_h
#define Analysis_BoostedTopAnalysis_interface_CATopJetEventSelector_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"
#include "DataFormats/Candidate/interface/Candidate.h"

using namespace std;

class CATopJetEventSelector : public EventSelector {
  public:
    CATopJetEventSelector( edm::ParameterSet const & params );
    virtual bool operator()( edm::EventBase const & t, reco::Candidate::LorentzVector const & v, pat::strbitset & ret, bool towards);

    vector<reco::ShallowClonePtrCandidate> const & topJets ()  const { return topJets_; }
    bool hasTopJets ()  const { return topJets_.size() > 0 ; }

  private:

    edm::InputTag               jetTag_;
    std::vector<reco::ShallowClonePtrCandidate>  topJets_;
    CATopTagFunctor  caTopJetSelector_;
    double  jetPtMin_;
    double  jetEtaMax_;
    double  dR_;  //hemisphere cone size

};

#endif
