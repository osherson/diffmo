#ifndef Analysis_JetAnalysis_interface_SimpleSubstructureSelector_h
#define Analysis_JetAnalysis_interface_SimpleSubstructureSelector_h


/**
  \class    SimpleSubstructureSelector
  \brief    Class to do a very simple substructure analysis on dijets

  This selector will select dijet events that have high jet mass
  with "symmetric" subjets. 

   This will make cuts in phases for
     analysis, starting:
      - Inclusive (all)
      - Kinematics (pt > ptMin, |y| < yMax)
      - Subjet mass symmetry for 1st jet (mu0 < muCut)
      - Subjet mass symmetry for 2nd jet (mu1 < muCut)
      - Mass cut for 1st jet (m > mCut)
      - Mass cut for 2nd jet (m > mCut)

  \author   Salvatore Rappoccio
*/



#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"




#include <iostream>

class SimpleSubstructureSelector : public EventSelector {

 public: // interface

  SimpleSubstructureSelector() {}

  SimpleSubstructureSelector( edm::ParameterSet const & parameters );

  using EventSelector::operator();
  virtual bool operator()( edm::EventBase const & iEvent, pat::strbitset & ret);
  
  /// -----------
  /// Accessors
  /// -----------

  /// Jets as coming from the tuple with no corrections
  std::vector<pat::Jet> const &          bareJets() const { return *jetsHandle_; }

  /// Corrected jet p4
  reco::Candidate::LorentzVector const & jetP4( unsigned int i ) const { return jetP4_[i]; }
  /// Mass drop variable
  double                                 jetMu( unsigned int i ) const { return jetMu_[i]; }
  /// Dijet mass candidate
  reco::Candidate::LorentzVector const & jjCand()                const { return jjCand_; }

  /// Selector fast-cache indices
  index_type indexInclusive_;
  index_type indexKin_;
  index_type indexMu1_;
  index_type indexMu2_;
  index_type indexM1_;
  index_type indexM2_;
  
 private: // member variables
  
    edm::InputTag src_;    /// Input tag for jets
    edm::InputTag rhoSrc_; /// mean pt per unit area
    edm::InputTag pvSrc_;  /// primary vertex source
    double ptCut_;         /// pt cut for both jets
    double rapidityCut_;   /// rapidity cut for both jets
    double jetScale_;      /// scale the JEC up or down (in MC)
    double jetSmear_;      /// smear the JER by this amount
    double jetUncertainty_;/// value of any "flat" jet uncertainty
    
    /// Jet energy corrections object
    std::vector<std::string> jecPayloads_;
    std::string jecUncPayload_;
    boost::shared_ptr<JetCorrectionUncertainty> jecUnc_;
    boost::shared_ptr<FactorizedJetCorrector> jec_;


    /// Per event cached jets and resulting 4-vector
    edm::Handle<std::vector<pat::Jet> > jetsHandle_;
    std::vector<reco::Candidate::LorentzVector> jetP4_;
    std::vector<double> jetMu_;
    reco::Candidate::LorentzVector jjCand_;
    
};

#endif
