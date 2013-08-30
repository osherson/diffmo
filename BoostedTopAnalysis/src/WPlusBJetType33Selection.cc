#include "Analysis/BoostedTopAnalysis/interface/WPlusBJetType33Selection.h"

WPlusBJetType33Selection::WPlusBJetType33Selection ( edm::ParameterSet const & params ) :
  jetTag_               (params.getParameter<edm::InputTag>("jetSrc") ),
  trigSrc_              (params.getParameter<edm::InputTag>("trigSrc") ),
  trig_                 (params.getParameter<std::string>("trig") ),
  leadJetPtCut_         (params.getParameter<double>( "leadJetPtCut" ) ),
  wMassMin_             (params.getParameter<double>("wMassMin") ),
  wMassMax_             (params.getParameter<double>("wMassMax") ),
  topMassMin_           (params.getParameter<double>("topMassMin") ) ,
  topMassMax_           (params.getParameter<double>("topMassMax") ),
  jetPt_                (params.getParameter<double>("jetPtMin") ),
  jetEta_               (params.getParameter<double>("jetEtaMax") ),
  twPlusBJetSelection_  (params),
  owPlusBJetSelection_  (twPlusBJetSelection_)
{
  //make the bitset
  push_back("Inclusive");
  //push_back("Trigger");
  push_back("nJets >= 6");
  //push_back("Leading Jet Pt");
  push_back("has bJet0");
  push_back("hasMinPair0");
  push_back("minPairMassCut0");
  push_back("topMass0");
  push_back("has bJet1");
  push_back("hasMinPair1");
  push_back("minPairMassCut1");
  push_back("topMass1");
  push_back("passType33");

  //turn on bit
  set("Inclusive");
  //set("Trigger");
  set("nJets >= 6");
  //set("Leading Jet Pt");
  set("has bJet0");
  set("hasMinPair0");
  set("minPairMassCut0");
  set("topMass0");
  set("has bJet1");
  set("hasMinPair1");
  set("minPairMassCut1");
  set("topMass1");
  set("passType33");

  if ( params.exists("cutsToIgnore") )
      setIgnoredCuts( params.getParameter<vector<string> >("cutsToIgnore") );

}

bool WPlusBJetType33Selection::operator()( edm::EventBase const & t, pat::strbitset & ret )
{
  //turn off and clear container
  ret.set(false);
  pfJets_.clear();

  p4_top0_ = reco::Candidate::LorentzVector(0,0,0,0);
  p4_top1_ = reco::Candidate::LorentzVector(0,0,0,0);

  passCut( ret, "Inclusive" );

  edm::Handle<vector<pat::Jet>  >   jetHandle;
  t.getByLabel( jetTag_, jetHandle );

  const boost::shared_ptr<PFJetIDSelectionFunctor> & pfJetSel = twPlusBJetSelection_.pfJetSel();
  pat::strbitset retPFJet = pfJetSel->getBitTemplate();

  for( vector<pat::Jet>::const_iterator jetBegin=jetHandle->begin(), jetEnd=jetHandle->end(), ijet=jetBegin ;
    ijet!=jetEnd; ijet++ )
  {
    if( ijet->pt() > jetPt_ && fabs( ijet->eta() ) < jetEta_ ) {
      retPFJet.set(false);
      bool passJetID = (*pfJetSel)( *ijet, retPFJet );
      if( passJetID ) {
        pfJets_.push_back( edm::Ptr<pat::Jet>(jetHandle, ijet-jetBegin )  );
      }
    } // end if jetPt, jetEta
  } // end ijet

  const pat::Jet * theJet;
  if( pfJets_.size() < 1 )   return false;
  theJet = &(*pfJets_.at(0));
/*
  Get the trigger
  edm::Handle<pat::TriggerEvent>  triggerEvent;
  t.getByLabel( trigSrc_, triggerEvent);
  if( !triggerEvent.isValid() )   return (bool)ret;

  // Check the trigger requirement
  pat::TriggerEvent const * trig = &*triggerEvent;

  bool passTrig = false;
  if( trig->wasRun() && trig->wasAccept() ) {
    pat::TriggerPath const * jetPath = trig->path(trig_);
    if( jetPath != 0 && jetPath->wasAccept() )  {
      passTrig = true;
    }
  }
*/
/*
  if( ignoreCut( "Trigger" ) || passTrig )  {
    passCut( ret, "Trigger" );

    if( ignoreCut( "Leading Jet Pt" ) || theJet->pt() > leadJetPtCut_ ) {
      passCut( ret, "Leading Jet Pt" );
*/
  if( ignoreCut( "nJets >= 6" ) || pfJets_.size() >= 6 ) {
      passCut( ret, "nJets >= 6" );
      pat::strbitset tret = twPlusBJetSelection_.getBitTemplate();
      //Analyze the towards direction
      bool tpassWPlusBJet  = twPlusBJetSelection_( t, theJet->p4(), tret, true );

      pat::strbitset oret = owPlusBJetSelection_.getBitTemplate();
      //Analyze the opposite direction
      bool opassWPlusBJet  = owPlusBJetSelection_( t, theJet->p4(), oret, false );

      std::vector<edm::Ptr<pat::Jet> >  const & tbJets = twPlusBJetSelection_.bJets();
      //cout<<"Type33::BJets0 is "<<tbJets.size()<<endl;
      std::vector<edm::Ptr<pat::Jet> >  const & obJets = owPlusBJetSelection_.bJets();
      std::vector<edm::Ptr<pat::Jet> >  const & tMinDrPair = twPlusBJetSelection_.minDrPair();
      std::vector<edm::Ptr<pat::Jet> >  const & oMinDrPair = owPlusBJetSelection_.minDrPair();

      int numBJets = 0;
      bool hasBJet0 = false, hasBJet1 = false;
      if( tbJets.size() >= 1 )  hasBJet0 = true;
      if( obJets.size() >= 1 )  hasBJet1 = true;

      bool passTopMass0 = false, passTopMass1= false;

      if( ignoreCut("has bJet0") || hasBJet0 ) {
        passCut( ret, "has bJet0" );
        bool hasMinPair0 = tMinDrPair.size()==2 ;
        if( ignoreCut("hasMinPair0") || hasMinPair0 ) {
          passCut( ret, "hasMinPair0" );
          p4_top0_  = tbJets.at(0)->p4() + tMinDrPair.at(0)->p4() + tMinDrPair.at(1)->p4();
          double minPairMass0 = (tMinDrPair.at(0)->p4() + tMinDrPair.at(1)->p4()).mass();
          bool passMinPairMass = ( minPairMass0 > wMassMin_ && minPairMass0 < wMassMax_ );
          if( ignoreCut("minPairMassCut0") || passMinPairMass ) {
            passCut( ret, "minPairMassCut0" );
            double topMass0 = p4_top0_.mass();
            passTopMass0 = ( topMass0 > topMassMin_ && topMass0 < topMassMax_ );
            if( ignoreCut("topMass0") || passTopMass0 )
              passCut( ret, "topMass0" );
          }  
        }  // end if hasMinPair0
      } //end if has bJet0

      if( ignoreCut("has bJet1") || hasBJet1 ) {
        passCut(ret, "has bJet1");
        bool hasMinPair1 = oMinDrPair.size()==2 ;
        if( ignoreCut("hasMinPair1") || hasMinPair1 ) {
           passCut( ret, "hasMinPair1" );
           p4_top1_  = obJets.at(0)->p4() + oMinDrPair.at(0)->p4() + oMinDrPair.at(1)->p4();
           double minPairMass1 = (oMinDrPair.at(0)->p4() + oMinDrPair.at(1)->p4()).mass();
           bool passMinPairMass = ( minPairMass1 > wMassMin_ && minPairMass1 < wMassMax_ );
           if( ignoreCut("minPairMassCut1") || passMinPairMass )  {
            passCut( ret, "minPairMassCut1" );
            double topMass1 = p4_top1_.mass();
            passTopMass1 = ( topMass1 > topMassMin_ && topMass1 < topMassMax_ );
            if( ignoreCut("topMass1") || passTopMass1 )
              passCut( ret, "topMass1" );
           }
        } // end if hasMinPair1
      } // end if has bJet1

      if( ignoreCut("passType33") || ( passTopMass0 && passTopMass1 ) )
        passCut( ret, "passType33" );
  } // nJets >= 6
//    } // Leading Jet Pt
//  }  // pass trigger

  return (bool)ret;


}
