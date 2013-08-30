#ifndef Analysis_SHyFT_interface_SHyFTPFSelector_h
#define Analysis_SHyFT_interface_SHyFTPFSelector_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/SelectorUtils/interface/PFElectronSelector.h"
#include "PhysicsTools/SelectorUtils/interface/PFMuonSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

class SHyFTPFSelector : public EventSelector {
   public:


     typedef reco::ShallowClonePtrCandidate               value_type;
     typedef std::vector<value_type>                      collection_type;
     typedef collection_type::const_iterator              const_iterator;
     typedef collection_type::iterator                    iterator;

      SHyFTPFSelector() {}
      SHyFTPFSelector( edm::ParameterSet const & params );

      virtual void scaleJets(double scale) {jetScale_ = scale;}
  
      virtual bool operator()( edm::EventBase const & t, pat::strbitset & ret);
      using EventSelector::operator();

      std::vector<reco::ShallowClonePtrCandidate> const & selectedJets      () const { return selectedJets_;     }
      std::vector<reco::ShallowClonePtrCandidate> const & selectedTightMuons     () const { return selectedTightMuons_;    }
      std::vector<reco::ShallowClonePtrCandidate> const & selectedLooseMuons() const { return selectedLooseMuons_;}
      reco::ShallowClonePtrCandidate const &              selectedMET       () const { return met_; }
      std::vector<reco::ShallowClonePtrCandidate> const & selectedTightElectrons () const { return selectedTightElectrons_;}
      std::vector<reco::ShallowClonePtrCandidate> const & selectedLooseElectrons () const { return selectedLooseElectrons_; }
      
      void printSelectors(std::ostream & out) const {
         out << "Muon ID Tight Selector: " << std::endl;
         muonIdPFTight_.print(out);
	 out << "Muon ID Loose Selector: " << std::endl;
	 muonIdPFLoose_.print(out);
         out << "Electron ID Tight Selector: " << std::endl;
         electronIdPFTight_.print(out);
         out << "Electron ID Loose Selector: " << std::endl;
         electronIdPFLoose_.print(out);
      }
 
   protected: 

      edm::InputTag               muonTag_;
      edm::InputTag               electronTag_;
      edm::InputTag               jetTag_;
      edm::InputTag               metTag_;
      edm::InputTag               pvTag_;
      edm::InputTag               trigTag_;
      std::string                 trig_;

      std::vector<reco::ShallowClonePtrCandidate> selectedJets_;
      std::vector<reco::ShallowClonePtrCandidate> allMuons_;
      std::vector<reco::ShallowClonePtrCandidate> selectedTightMuons_;
      std::vector<reco::ShallowClonePtrCandidate> selectedLooseMuons_;
      std::vector<reco::ShallowClonePtrCandidate> allElectrons_;
      std::vector<reco::ShallowClonePtrCandidate> selectedTightElectrons_;
      std::vector<reco::ShallowClonePtrCandidate> selectedLooseElectrons_;
      std::vector<reco::ShallowClonePtrCandidate> selectedMETs_;
      reco::ShallowClonePtrCandidate              met_;

      PFMuonSelector      muonIdPFTight_;
      PFMuonSelector      muonIdPFLoose_;
      PFElectronSelector  electronIdPFTight_;
      PFElectronSelector  electronIdPFLoose_;

      int minJets_;

      double muPtMin_  ;
      double muEtaMax_ ;
      double eleEtMin_ ;
      double eleEtaMax_;

      double muPtMinLoose_  ;
      double muEtaMaxLoose_ ;
      double eleEtMinLoose_ ;
      double eleEtaMaxLoose_;

      double jetPtMin_ ;
      double jetEtaMax_;

      double jetScale_;
      double jetUncertainty_; // "flat" uncertainty after the L2L3 uncertainty
      double jetSmear_;
      double metMin_;
      double metMax_;
      double unclMetScale_; 
      double ePtScale_;        
      double ePtUncertaintyEE_;

      index_type   inclusiveIndex_; 
      index_type   triggerIndex_;   
      index_type   lep1Index_;      
      index_type   lep2Index_;      
      index_type   lep3Index_;      
      index_type   metLowIndex_;  
      index_type   metHighIndex_;       
      index_type   jet1Index_;      
      index_type   jet2Index_;      
      index_type   jet3Index_;      
      index_type   jet4Index_;      
      index_type   jet5Index_;      

      /// Use data :
      ///   - L2L3Residual correction
      bool           useData_;
      /// Remove leptons from the jet list that are "loose but not tight"
      ///    to avoid double-counting in the sideband regions
      bool           removeLooseLeptons_;
      /// Use L1 corrections
      bool           useL1Corr_;

      // Jet energy corrections object
      std::string    jecPayload_;
      boost::shared_ptr<JetCorrectionUncertainty> jecUnc_;
      boost::shared_ptr<FactorizedJetCorrector> jec_;
};


#endif
