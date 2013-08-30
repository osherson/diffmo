#ifndef Analysis_JetAnalysis_include_DijetSelector_h
#define Analysis_JetAnalysis_include_DijetSelector_h


#include "PhysicsTools/SelectorUtils/interface/strbitset.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

#include "PhysicsTools/SelectorUtils/interface/strbitset.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/Common/interface/EventBase.h"

#include <vector>
#include <string>


class DijetSelector : public EventSelector {
 public:
 DijetSelector( edm::ParameterSet const & caloJetIdParams,
		edm::ParameterSet const & pfJetIdParams,
		edm::ParameterSet const & params ) :
    jetSel_    (new JetIDSelectionFunctor(caloJetIdParams)), 
    pfJetSel_  (new PFJetIDSelectionFunctor(pfJetIdParams)),
    jetSrc_    (params.getParameter<edm::InputTag>("jetSrc")), 
    pfJetSrc_  (params.getParameter<edm::InputTag>("pfJetSrc")),
    minNJets_  (params.getParameter<unsigned int>("minNJets")),
    ptMin_     (params.getParameter<double>("ptMin")),
    etaMax_    (params.getParameter<double>("etaMax")),
    htMin_     (params.getParameter<double>("htMin")),
    ht2Min_    (params.getParameter<double>("ht2Min"))
      {
	std::cout << "Instantiated DijetSelector" << std::endl;
	bool useCalo = params.getParameter<bool>("useCalo");

	push_back("Calo Cuts");
	push_back("Calo Kin Cuts");
	push_back("Calo Delta Phi");
	push_back("Calo HT");
	push_back("Calo Dijet HT");
	push_back("Calo Jet ID");
	push_back("PF Cuts");
	push_back("PF Kin Cuts");
	push_back("PF Delta Phi");
	push_back("PF HT");
	push_back("PF Dijet HT");
	push_back("PF Jet ID");

	set("Calo Cuts", useCalo);
	set("Calo Kin Cuts", useCalo);
	set("Calo Delta Phi", useCalo);
	set("Calo HT", useCalo);
	set("Calo Dijet HT", useCalo);
	set("Calo Jet ID", useCalo);
	set("PF Cuts", !useCalo);
	set("PF Kin Cuts", !useCalo);
	set("PF Delta Phi", !useCalo);
	set("PF HT", !useCalo);
	set("PF Dijet HT", !useCalo);
	set("PF Jet ID", !useCalo);

	if ( params.exists("cutsToIgnore") )
	  setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );

    
      }

    virtual ~DijetSelector() {}

    // Main selector
    virtual bool operator()( edm::EventBase const & event, pat::strbitset & ret){

      allCaloJets_.clear();
      allPFJets_.clear();
      caloJets_.clear();
      pfJets_.clear();

      pat::strbitset retCaloJet = jetSel_->getBitTemplate();
      pat::strbitset retPFJet = pfJetSel_->getBitTemplate();

      // First check if we're considering calo jet cuts
      if ( considerCut("Calo Cuts") ) {
	passCut(ret, "Calo Cuts");
	event.getByLabel( jetSrc_, h_jets_ );
	// Calo Cuts

	// Check kinematics
	double htCalo = 0.0;
	double htCalo2 = 0.0; // dijet ht
	if ( h_jets_->size() >= minNJets_ ) {
	  for (std::vector<pat::Jet>::const_iterator jetsBegin = h_jets_->begin(),
		 jetsEnd = h_jets_->end(),
		 ijet = jetsBegin;
	       ijet != jetsEnd; ++ijet ) {
	    if ( ijet->pt() > ptMin_ && std::abs(ijet->eta()) < etaMax_ ) {
	      allCaloJets_.push_back( edm::Ptr<pat::Jet>( h_jets_, ijet - jetsBegin ) );
	      htCalo += ijet->pt();
	      if ( ijet - jetsBegin < 2 ) {
		htCalo2 += ijet->pt();
	      }
	    }
	  }
	}
	if ( allCaloJets_.size() >= minNJets_ || ignoreCut("Calo Kin Cuts") ) {
	  passCut(ret, "Calo Kin Cuts");

	  // Check Dijets
	  bool passDijet = true;
	  if ( considerCut("Calo Delta Phi") && minNJets_ >= 2 && allCaloJets_.size() >= 2 ) {
	    pat::Jet const & jet0 = **(allCaloJets_.begin());
	    pat::Jet const & jet1 = **(allCaloJets_.begin() + 1);
	    double dphi = fabs(deltaPhi<double>( jet0.phi(),
						 jet1.phi() ) );
	    bool passDeltaPhi = fabs(dphi - TMath::Pi()) < 1.0;
	    if (passDeltaPhi) passDijet = true;
	    else passDijet = false;
	  }
	  if ( passDijet || ignoreCut("Calo Delta Phi") ) {
	    passCut(ret, "Calo Delta Phi");

	    if ( htCalo > htMin_ || ignoreCut("Calo HT") ) {
	      passCut(ret, "Calo HT");

	      if ( htCalo2 > ht2Min_ || ignoreCut("Calo Dijet HT") ) {
		passCut(ret, "Calo Dijet HT");
	    
		// Check jet ID

		std::vector<edm::Ptr<pat::Jet> >::const_iterator jetsBegin =allCaloJets_.begin();
		std::vector<edm::Ptr<pat::Jet> >::const_iterator jetsEnd = allCaloJets_.end();
		std::vector<edm::Ptr<pat::Jet> >::const_iterator ijet = jetsBegin;
		if ( considerCut("Calo Delta Phi") ) jetsEnd = jetsBegin + 2;
		for (; ijet != jetsEnd; ++ijet ) {
		  pat::Jet const & jet = **ijet;
		  if ( considerCut("Calo Jet ID") ) {
		    retCaloJet.set(false);
		    bool pass0 = (*jetSel_)( jet, retCaloJet );
		    if ( pass0 ) 
		      caloJets_.push_back( *ijet );
		  } else {
		    caloJets_.push_back( *ijet );
		  }
		}// end loop over good jets
		if ( caloJets_.size() >= minNJets_ || ignoreCut("Calo Jet ID") ) {
		  passCut(ret, "Calo Jet ID");
		}// end if N good calo jets
	      }// end if pass dijet ht cut
	    }// end if pass ht cut
	  }// end delta phi cut
	}// end calo kin cuts
      }// end if calo cuts


      // First check if we're considering pf jet cuts
      if ( considerCut("PF Cuts") ) {
	passCut(ret, "PF Cuts");
	event.getByLabel( pfJetSrc_, h_pfjets_ );
	// PF Cuts

	// Check kinematics
	double htPF = 0.0;
	double htPF2 = 0.0;
	if ( h_pfjets_->size() >= minNJets_ ) {
	  for (std::vector<pat::Jet>::const_iterator jetsBegin = h_pfjets_->begin(),
		 jetsEnd = h_pfjets_->end(),
		 ijet = jetsBegin;
	       ijet != jetsEnd; ++ijet ) {
	    if ( ijet->pt() > ptMin_ && std::abs(ijet->eta()) < etaMax_ ) {
	      allPFJets_.push_back( edm::Ptr<pat::Jet>( h_pfjets_, ijet - jetsBegin ) );
	      htPF += ijet->pt();
	      if ( ijet - jetsBegin < 2 ) {
		htPF2 += ijet->pt();
	      }
	    }
	  }
	}
	if ( allPFJets_.size() >= minNJets_ || ignoreCut("PF Kin Cuts") ) {
	  passCut(ret, "PF Kin Cuts");

	  // Check Dijets
	  bool passDijet = true;
	  if ( considerCut("PF Delta Phi") && minNJets_ >= 2 && allPFJets_.size() >= 2 ) {
	    pat::Jet const & jet0 = **(allPFJets_.begin());
	    pat::Jet const & jet1 = **(allPFJets_.begin() + 1);
	    double dphi = fabs(deltaPhi<double>( jet0.phi(),
						 jet1.phi() ) );
	    bool passDeltaPhi = fabs(dphi - TMath::Pi()) < 1.0;
	    if (passDeltaPhi) passDijet = true;
	    else passDijet = false;
	  }
	  if ( passDijet || ignoreCut("PF Delta Phi") ) {
	    passCut(ret, "PF Delta Phi");

	    if ( htPF > htMin_ || ignoreCut("PF HT") ) {
	      passCut(ret, "PF HT");
	    
	      if ( htPF2 > ht2Min_ || ignoreCut("PF Dijet HT") ) {
		passCut(ret, "PF Dijet HT");
	    
		// Check jet ID
		std::vector<edm::Ptr<pat::Jet> >::const_iterator jetsBegin =allPFJets_.begin();
		std::vector<edm::Ptr<pat::Jet> >::const_iterator jetsEnd = allPFJets_.end();
		std::vector<edm::Ptr<pat::Jet> >::const_iterator ijet = jetsBegin;
		//if ( considerCut("PF Delta Phi") ) jetsEnd = jetsBegin + 2;
		for (; ijet != jetsEnd; ++ijet ) {
		  pat::Jet const & jet = **ijet;
		  if ( considerCut("PF Jet ID") ) {
		    retPFJet.set(false);
		    bool pass0 = (*pfJetSel_)( jet, retPFJet );
		    if ( pass0 ) 
		      pfJets_.push_back( *ijet );
		  } else {
		    pfJets_.push_back(*ijet);
		  }
		}// end loop over good jets
		if ( pfJets_.size() >= minNJets_ || ignoreCut("PF Jet ID") ) {
		  passCut(ret, "PF Jet ID");
		}// end if N good pf jets
	      }// end if pass dijet ht cut
	    }// end if pass ht cut
	  }// end delta phi cut
	}// end pf kin cuts
      }// end if pf cuts



      setIgnored(ret);

      return false;
    }// end of method

  boost::shared_ptr<JetIDSelectionFunctor> const &   jetSel()     const { return jetSel_;}
  boost::shared_ptr<PFJetIDSelectionFunctor> const & pfJetSel()   const { return pfJetSel_;}

  std::vector<edm::Ptr<pat::Jet> > const &  allCaloJets () const { return allCaloJets_; }
  std::vector<edm::Ptr<pat::Jet> > const &  allPFJets   () const { return allPFJets_; }

  std::vector<edm::Ptr<pat::Jet> > const &  caloJets() const { return caloJets_; }
  std::vector<edm::Ptr<pat::Jet> > const &  pfJets  () const { return pfJets_; }



protected:
  boost::shared_ptr<JetIDSelectionFunctor>   jetSel_;
  boost::shared_ptr<PFJetIDSelectionFunctor> pfJetSel_;
  edm::InputTag                              jetSrc_;
  edm::InputTag                              pfJetSrc_;
  unsigned int                               minNJets_;
  double                                     ptMin_;
  double                                     etaMax_;
  double                                     htMin_;
  double                                     ht2Min_;

  edm::Handle<std::vector<pat::Jet> >        h_jets_;
  edm::Handle<std::vector<pat::Jet> >        h_pfjets_;
  
  std::vector<edm::Ptr<pat::Jet> >           allCaloJets_;
  std::vector<edm::Ptr<pat::Jet> >           allPFJets_;


  std::vector<edm::Ptr<pat::Jet> >           caloJets_;
  std::vector<edm::Ptr<pat::Jet> >           pfJets_;

};


#endif
