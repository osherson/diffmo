#include "Analysis/BoostedTopAnalysis/interface/WPlusBJetType23Selection.h"

WPlusBJetType23Selection::WPlusBJetType23Selection ( edm::ParameterSet const & params ) :
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
  //push_back("Leading Jet Pt");
  push_back("nJets >= 5");
  push_back("has WJet");
  push_back("wMassCut");
  push_back("has bJetT2");
  push_back("has Top2");
  push_back("Top2MassCut");
  push_back("has bJetT3");
  push_back("hasMinPair");
  push_back("minPairMassCut");
  push_back("has Top3");
  push_back("Top3MassCut");
  push_back("passType23");

  //turn on bit
  set("Inclusive");
  //set("Trigger");
  //set("Leading Jet Pt");
  set("nJets >= 5");
  set("has WJet");
  set("wMassCut");
  set("has bJetT2");
  set("has Top2");
  set("Top2MassCut");
  set("has bJetT3");
  set("hasMinPair");
  set("minPairMassCut");
  set("has Top3");
  set("Top3MassCut");
  set("passType23");

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<vector<string> >("cutsToIgnore") );


}

bool WPlusBJetType23Selection::operator()( edm::EventBase const & t, pat::strbitset & ret )
{
  //turn off and clear container
  ret.set(false);
  tightTop0_ = false;
  tightTop1_ = false;
  type3Top0_ = false;
  type3Top1_ = false;
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
  //Get the trigger
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

  if( ignoreCut( "Trigger" ) || passTrig )  {
    passCut( ret, "Trigger" );

    if( ignoreCut( "Leading Jet Pt" ) || theJet->pt() > leadJetPtCut_ ) {
      passCut( ret, "Leading Jet Pt" );
*/
  if( ignoreCut("nJets >= 5") || pfJets_.size() >= 5 ) {
      passCut( ret, "nJets >= 5" );
      pat::strbitset tret = twPlusBJetSelection_.getBitTemplate();
      //Analyze the towards direction
      bool tpassWPlusBJet  = twPlusBJetSelection_( t, theJet->p4(), tret, true );

      pat::strbitset oret = owPlusBJetSelection_.getBitTemplate();
      //Analyze the opposite direction
      bool opassWPlusBJet  = owPlusBJetSelection_( t, theJet->p4(), oret, false );

      std::vector<edm::Ptr<pat::Jet> >  const & tWJets = twPlusBJetSelection_.wJets();
      std::vector<edm::Ptr<pat::Jet> >  const & oWJets = owPlusBJetSelection_.wJets();
      std::vector<edm::Ptr<pat::Jet> >  const & tbJets = twPlusBJetSelection_.bJets();
      std::vector<edm::Ptr<pat::Jet> >  const & obJets = owPlusBJetSelection_.bJets();
      std::vector<edm::Ptr<pat::Jet> >  const & tMinDrPair = twPlusBJetSelection_.minDrPair();
      std::vector<edm::Ptr<pat::Jet> >  const & oMinDrPair = owPlusBJetSelection_.minDrPair();

      bool hasW0 = false, hasW1 = false;
      bool hasB0 = false, hasB1 = false;
      bool topMassCut0 = false, topMassCut1 = false;
      double wMass0 = 0., wMass1 = 0. ;
      if( tWJets.size() >= 1 )  { wMass0 = tWJets.at(0)->mass(); }
      if( oWJets.size() >= 1 )  { wMass1 = oWJets.at(0)->mass(); }
      if( tbJets.size() >= 1 )  hasB0 = true;
      if( obJets.size() >= 1 )  hasB1 = true;

      if( wMass0 > wMassMin_ && wMass0 < wMassMax_ )    hasW0 = true;
      if( wMass1 > wMassMin_ && wMass1 < wMassMax_ )    hasW1 = true;
      tightTop0_ = hasW0 && hasB0;
      tightTop1_ = hasW1 && hasB1;

      if( ignoreCut("has WJet") || hasW0 || hasW1 ) {
        passCut( ret, "has WJet");
        passCut( ret, "wMassCut");
        if( ignoreCut("has bJetT2") || tightTop0_ || tightTop1_ )  {
          passCut( ret, "has bJetT2");
          passCut( ret, "has Top2" );
          if( tightTop0_ )  {
            p4_top0_ = tWJets.at(0)->p4() + tbJets.at(0)->p4();
            double topMass0 = p4_top0_.mass();
            topMassCut0 = ( topMass0 > topMassMin_ && topMass0 < topMassMax_ );
            if( ignoreCut("Top2MassCut") || topMassCut0 )
              passCut( ret, "Top2MassCut" );
          } // if tightTop0_
          if( tightTop1_ )  {
            p4_top1_ = oWJets.at(0)->p4() + obJets.at(0)->p4();
            double topMass1 = p4_top1_.mass();
            topMassCut1 = ( topMass1 > topMassMin_ && topMass1 < topMassMax_ );
            if( ignoreCut("Top2MassCut") || topMassCut1 )
              passCut( ret, "Top2MassCut" );
          } // if tightTop1_
        }  // has bJetT2
      } // has WJet

      bool hasBT3 = ( !hasW0 && hasB0 ) || ( !hasW1 && hasB1 );
      if( ignoreCut("has bJetT3") || hasBT3 ) {
        passCut( ret, "has bJetT3");
        if( !hasW0 )  {
          bool hasMinPair = (tMinDrPair.size() == 2);
          if( ignoreCut("hasMinPair") || hasMinPair ) {
            passCut( ret, "hasMinPair" );
            type3Top0_ = true;
            double minPairMass = (tMinDrPair.at(0)->p4() + tMinDrPair.at(1)->p4()).mass();
            p4_top0_  = tbJets.at(0)->p4() + tMinDrPair.at(0)->p4() + tMinDrPair.at(1)->p4();
            double topMass0 = p4_top0_.mass();
            bool minMassCut = ( minPairMass > wMassMin_ && minPairMass < wMassMax_ );
            if( ignoreCut("minPairMassCut") || minMassCut ) {
              passCut( ret, "minPairMassCut" );
              passCut( ret, "has Top3" );
              topMassCut0 = ( topMass0 > topMassMin_ && topMass0 < topMassMax_ );
              if( ignoreCut("Top3MassCut") || topMassCut0 )
                passCut( ret, "Top3MassCut" );
            } // minPairMassCut
          }  // hasMinPair
        }  // end !hasW0
        if( !hasW1 )  {
          bool hasMinPair = (oMinDrPair.size() == 2);
          if( ignoreCut("hasMinPair") || hasMinPair ) {
            passCut( ret, "hasMinPair" );
            type3Top1_  = true;
            double minPairMass = (oMinDrPair.at(0)->p4() + oMinDrPair.at(1)->p4()).mass();
            p4_top1_  = obJets.at(0)->p4() + oMinDrPair.at(0)->p4() + oMinDrPair.at(1)->p4();
            double topMass1 = p4_top1_.mass();
            bool minMassCut = ( minPairMass > wMassMin_ && minPairMass < wMassMax_ );
            if( ignoreCut("minPairMassCut") || minMassCut ) {
              passCut( ret, "minPairMassCut" );
              passCut( ret, "has Top3" );
              topMassCut1 = ( topMass1 > topMassMin_ && topMass1 < topMassMax_ );
              if( ignoreCut("Top3MassCut") || topMassCut1 )
                passCut( ret, "Top3MassCut" );
            } // minPairMassCut
          }  // hasMinPair
        }  // end !hasW1
      }  // end has bJetT3

      if( topMassCut0 && topMassCut1 )
        passCut( ret, "passType23" );
  } // nJets >= 5
//    } // Leading Jet Pt
//  }  // pass trigger

  return (bool)ret;


}
