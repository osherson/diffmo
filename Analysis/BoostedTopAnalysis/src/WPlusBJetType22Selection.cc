#include "Analysis/BoostedTopAnalysis/interface/WPlusBJetType22Selection.h"

WPlusBJetType22Selection::WPlusBJetType22Selection ( edm::ParameterSet const & params ) :
  jetTag_               (params.getParameter<edm::InputTag>("jetSrc") ),
  trigSrc_              (params.getParameter<edm::InputTag>("trigSrc") ),
  trig_                 (params.getParameter<std::string>("trig") ),
  leadJetPtCut_         (params.getParameter<double>( "leadJetPtCut" ) ),
  secondJetPtCut_       (params.getParameter<double>( "secondJetPtCut" ) ),
  thirdJetPtCut_        (params.getParameter<double>( "thirdJetPtCut" ) ),
  fourthJetPtCut_       (params.getParameter<double>( "fourthJetPtCut") ),
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
  push_back("Trigger");
  push_back("nJets >= 4");
  push_back("Leading Jet Pt");
  push_back("Second Jet Pt");
  push_back("Third Jet Pt");
  push_back("Fourth Jet Pt");
  push_back("Type22");
  push_back("Type23");
  push_back("Type33");
  push_back("has WJet0");
  push_back("wMassCut0");
  push_back("has Top0");
  push_back("TopMassCut0");
  push_back("has WJet1");
  push_back("wMassCut1");
  push_back("has Top1");
  push_back("TopMassCut1");
  push_back("hasTwoTops");
  push_back("has Tight Top");

  //turn on bit
  set("Inclusive");
  set("Trigger");
  set("nJets >= 4");
  set("Leading Jet Pt");
  set("Second Jet Pt");
  set("Third Jet Pt");
  set("Fourth Jet Pt");
  set("Type22");
  set("Type23");
  set("Type33");
  set("has WJet0");
  set("wMassCut0");
  set("has Top0");
  set("TopMassCut0");
  set("has WJet1");
  set("wMassCut1");
  set("has Top1");
  set("TopMassCut1");
  set("hasTwoTops");
  set("has Tight Top");

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<vector<string> >("cutsToIgnore") );

}

bool WPlusBJetType22Selection::operator()( edm::EventBase const & t, pat::strbitset & ret )
{
  //turn off and clear container
  ret.set(false);
  tightTop0_ = false;
  tightTop1_ = false;
  pfJets_.clear();
  Type = Undef;

  p4_top0_ = reco::Candidate::LorentzVector(0,0,0,0);
  p4_top1_ = reco::Candidate::LorentzVector(0,0,0,0);

  passCut( ret, "Inclusive" );

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

  edm::Handle<vector<pat::Jet>  >   jetHandle;
  t.getByLabel( jetTag_, jetHandle );

  const boost::shared_ptr<PFJetIDSelectionFunctor> & pfJetSel = twPlusBJetSelection_.pfJetSel();
  pat::strbitset retPFJet = pfJetSel->getBitTemplate();

  for( vector<pat::Jet>::const_iterator jetBegin=jetHandle->begin(), jetEnd=jetHandle->end(), ijet=jetBegin ;
    ijet!=jetEnd; ijet++ )
  {
    if( ijet->pt() > jetPt_ && fabs( ijet->eta() ) < jetEta_ )  {
      retPFJet.set(false);
      bool passJetID = (*pfJetSel)( *ijet, retPFJet );
      if( passJetID ) {
        pfJets_.push_back( edm::Ptr<pat::Jet>(jetHandle, ijet-jetBegin )  );
      }
    } // end if jetPt, jetEta
  } // end ijet

  if( ignoreCut( "Trigger" ) || passTrig )  {
    passCut( ret, "Trigger" );

    if( ignoreCut( "nJets >= 4" ) || pfJets_.size() >= 4 )  {
      passCut( ret, "nJets >= 4" );
      const pat::Jet * theJet = &(*pfJets_.at(0));

      if( ignoreCut( "Leading Jet Pt" ) || theJet->pt() > leadJetPtCut_ ) {
        passCut( ret, "Leading Jet Pt" );

        if( ignoreCut( "Second Jet Pt" ) || pfJets_.at(1)->pt() > secondJetPtCut_ ) {
          passCut( ret, "Second Jet Pt" );

          if( ignoreCut( "Third Jet Pt" ) || pfJets_.at(2)->pt() > thirdJetPtCut_ ) {
            passCut( ret, "Third Jet Pt" );

            if( ignoreCut( "Fourth Jet Pt" ) || pfJets_.at(3)->pt() > fourthJetPtCut_ ) {
              passCut( ret, "Fourth Jet Pt" );

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
              edm::Ptr<pat::Jet> const & taJet = twPlusBJetSelection_.aJet();
              edm::Ptr<pat::Jet> const & oaJet = owPlusBJetSelection_.aJet();

              bool hasW0 = false, hasW1 = false;
              int numBJets = 0;
              //Get W mass
              double wMass0 = 0.0, wMass1 = 0.0;
              if( tWJets.size() >= 1 )  wMass0 = tWJets.at(0)->mass();
              if( oWJets.size() >= 1 )  wMass1 = oWJets.at(0)->mass();
              if( tbJets.size() >= 1 )  numBJets++;
              if( obJets.size() >= 1 )  numBJets++;

              if( wMass0 > wMassMin_ && wMass0 < wMassMax_ )    hasW0 = true;
              if( wMass1 > wMassMin_ && wMass1 < wMassMax_ )    hasW1 = true;

              bool hasTop0 = false, hasTop1 = false;
              bool passTopMass0 = false, passTopMass1 = false;

              if( ignoreCut("has WJet0") || tWJets.size() >= 1 ) {
                passCut( ret, "has WJet0" );
                if( ignoreCut("wMassCut0")  || hasW0 )  {
                  passCut( ret, "wMassCut0");
                  if( tbJets.size() >= 1 ) {
                    p4_top0_ = tWJets.at(0)->p4() + tbJets.at(0)->p4()  ;
                    tightTop0_ = true;
                    hasTop0 = true;
                  }
                  else if( twPlusBJetSelection_.aJetFound() )  {
                    p4_top0_ = tWJets.at(0)->p4() + taJet->p4();
                    tightTop0_ = false;
                    hasTop0 = true;
                  }
                  if( hasTop0 ) {
                    passCut( ret, "has Top0" );
                    double topMass0 = p4_top0_.mass();
                    passTopMass0 = ( topMass0 > topMassMin_ && topMass0 < topMassMax_ );
                    if( ignoreCut("TopMassCut0") || passTopMass0 )
                      passCut( ret, "TopMassCut0" );
                  }  // end hasTop0
                }  //wMassCut0
              } // has WJet0

              if( ignoreCut("has WJet1") || oWJets.size() >= 1 ) {
                passCut( ret, "has WJet1" );
                if( ignoreCut("wMassCut1") || hasW1 ) {
                  passCut( ret, "wMassCut1" );
                  if( obJets.size() >= 1 ) {
                    p4_top1_ = oWJets.at(0)->p4() + obJets.at(0)->p4()  ;
                    tightTop1_ = true;
                    hasTop1 = true;
                  }
                  else if( owPlusBJetSelection_.aJetFound() ) {
                    p4_top1_ = oWJets.at(0)->p4() + oaJet->p4();
                    tightTop1_ = false;
                    hasTop1 = true;
                  }
                  if( hasTop1 ) {
                    passCut( ret, "has Top1" );
                    double topMass1 = p4_top1_.mass();
                    passTopMass1 = ( topMass1 > topMassMin_ && topMass1 < topMassMax_ );
                    if( ignoreCut("TopMassCut1") || passTopMass1 )
                      passCut( ret, "TopMassCut1" );
                  }  // end hasTop1 
                } // wMassCut1
              }  // has WJet1

              //Event splitting
              if( hasW0 && hasW1 )    { Type = Type22; passCut( ret, "Type22" ); }
              else if ( hasW0 || hasW1 )   {  Type = Type23; passCut( ret, "Type23"); }
              else  { Type = Type33; passCut( ret, "Type33" ); }

              bool hasTwoTops = passTopMass0 && passTopMass1 ;
              if( ignoreCut("hasTwoTops") || hasTwoTops ) {
                passCut( ret, "hasTwoTops" );

                if( ignoreCut("has Tight Top") || numBJets >= 1 ) {
                  passCut( ret, "has Tight Top" );
                } // has Tight Top
              }// hasTwoTops
            }  // fourth Jet Pt
          } // third Jet Pt
        } // second Jet Pt
      } // Leading Jet Pt
    } // nJets >= 4 
  }  // pass trigger

  return ret[string("has Tight Top")] ;


}
