#ifndef Analysis_BoostedTopAnalysis_interface_Type11Selection_v1_h
#define Analysis_BoostedTopAnalysis_interface_Type11Selection_v1_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

class Type11Selection_v1 : public EventSelector {
  public :
    Type11Selection_v1( edm::ParameterSet const & params );
    virtual ~Type11Selection_v1() { }
    virtual bool operator() ( edm::EventBase const & t, pat::strbitset & ret );
	std::vector<edm::Ptr<pat::Jet> >  const &  caTopJets() const { return caTopJets_ ; }
	std::vector<edm::Ptr<pat::Jet> >  const &  ca8Jets() const { return ca8Jets_ ; }

  private :
    edm::ParameterSet const &                    pfJetIdParams_;
    boost::shared_ptr<PFJetIDSelectionFunctor>   pfJetSel_;
    double caTopJetPtMin_;
    double caTopJetEtaCut_;
    edm::InputTag                               patJetCollectionInputTag_;
    edm::InputTag                               caTopJetCollectionInputTag_;
    std::vector<edm::Ptr<pat::Jet> >            caTopJets_ ;
    std::vector<edm::Ptr<pat::Jet> >            ca8Jets_ ;

};

#endif
