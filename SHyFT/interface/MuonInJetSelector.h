#ifndef Analysis_SHyFT_interface_MuonInJetSelector_h
#define Analysis_SHyFT_interface_MuonInJetSelector_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "PhysicsTools/SelectorUtils/interface/MuonVPlusJetsIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PVSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"

class MuonInJetSelector : public EventSelector {
   public:
      MuonInJetSelector() {}
      MuonInJetSelector( edm::ParameterSet const & params );

  
      virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);
      using EventSelector::operator();

      std::vector<reco::ShallowClonePtrCandidate> const & jets () const { return selectedJets_;}
      std::vector<reco::ShallowClonePtrCandidate> const & muons () const { return selectedMuons_;}
 
   protected: 

      edm::InputTag               muonTag_;
      edm::InputTag               jetTag_;

      MuonVPlusJetsIDSelectionFunctor      muonId_;
      PFJetIDSelectionFunctor              pfjetId_;

      double muJetDR_;

      double muPtMin_  ;
      double muEtaMax_ ;

      double jetPtMin_ ;
      double jetEtaMax_;

      std::string btagDisc1_;
      std::string btagDisc2_;
      double btagDiscCut1_;
      double btagDiscCut2_;

      index_type   inclusiveIndex_;
      index_type   mu1_;
      index_type   mu2_;
      index_type   jet1Index_;
      index_type   jet1TagIndex_;
      index_type   jet2Index_;
      index_type   jet2TagIndex_;


      std::vector<reco::ShallowClonePtrCandidate> selectedMuons_;
      std::vector<reco::ShallowClonePtrCandidate> selectedJets_;
};


#endif
