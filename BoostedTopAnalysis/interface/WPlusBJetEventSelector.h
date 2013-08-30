#ifndef Analysis_BoostedTopAnalysis_interface_WPlusBJetEventSelector_h
#define Analysis_BoostedTopAnalysis_interface_WPlusBJetEventSelector_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/ShallowClonePtrCandidate.h"
#include "Analysis/BoostedTopAnalysis/interface/BoostedTopWTagFunctor.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "Analysis/BoostedTopAnalysis/interface/WPlusBJetTypes.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoissonQ.h"


#include "TFile.h"
#include "TH1F.h"

using namespace std;

class WPlusBJetEventSelector : public EventSelector {
  public:
    WPlusBJetEventSelector( edm::ParameterSet const & params );
    virtual ~WPlusBJetEventSelector() { }
    virtual bool operator()( edm::EventBase const & t, reco::Candidate::LorentzVector const & v, pat::strbitset & ret, bool towards);

    boost::shared_ptr<PFJetIDSelectionFunctor> const & pfJetSel()   const { return pfJetSel_;}
    std::vector<edm::Ptr<pat::Jet> >  const & wJets ()  const { return wJets_; }
    std::vector<edm::Ptr<pat::Jet> >  const & bJets ()  const { return bJets_; }
    //For type III top reconstruction, a pair of jets with min deltaR
    std::vector<edm::Ptr<pat::Jet> >  const & minDrPair () const { return minDrPair_ ; } 
    //For type II top, when a W jet is found, but no b jet
    edm::Ptr<pat::Jet> const & aJet()  const { return aJet_ ; }
    bool hasWJets ()  const { return wJets_.size() > 0 ; }
    bool hasBJets ()  const { return bJets_.size() > 0 ; }
    bool aJetFound () const { return aJetFound_ ; }
    std::vector<Type2L>     const & looseTops () const { return looseTops_; }
    std::vector<Type2T>     const & tightTops () const { return tightTops_; }
    std::vector<Type3>      const & type3Tops () const { return type3Tops_; }

    //all jets in this hemisphere
    std::vector<edm::Ptr<pat::Jet> > const & allJets() const { return allJets_ ; }

  private:

    //Not for use at all
    virtual bool operator() (edm::EventBase const & t, pat::strbitset & ret ) { return true;}
    edm::ParameterSet const &   pfJetIdParams_;
    boost::shared_ptr<PFJetIDSelectionFunctor>   pfJetSel_;
    edm::InputTag               jetTag_;
    std::vector<edm::Ptr<pat::Jet> >          wJets_;
    std::vector<edm::Ptr<pat::Jet> >          bJets_;
    std::vector<edm::Ptr<pat::Jet> >          minDrPair_;
    edm::Ptr<pat::Jet>          	      aJet_;
    bool aJetFound_ ;
    BoostedTopWTagFunctor  wJetSelector_;
    double  jetPtMin_;
    double  jetEtaMax_;
    string  bTagAlgo_;
    double  bTagOP_;
    reco::Candidate::LorentzVector vtowards;  //towards direction

    std::vector<edm::Ptr<pat::Jet> >           pfJets_;
    std::vector<edm::Ptr<pat::Jet> >           allJets_;

    std::vector<Type2L>             looseTops_;
    std::vector<Type2T>             tightTops_;
    std::vector<Type3>              type3Tops_;
    void      fillLooseTops();
    void      fillTightTops();
    void      fillType3Tops();
    string    mistagFileName_;
    TFile *   mistagFile_;
    TH1F  *   wMistag_;
    TH1F  *   bMistag_;
    CLHEP::RandFlat *flatDistribution_;
};

#endif
