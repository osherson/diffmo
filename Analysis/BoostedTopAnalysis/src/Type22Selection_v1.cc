#include "Analysis/BoostedTopAnalysis/interface/Type22Selection_v1.h"

using namespace std;

Type22Selection_v1::Type22Selection_v1 ( edm::ParameterSet const & params ) :
  pfJetIdParams_         (params.getParameter<edm::ParameterSet>("pfJetIDParams") ),
  pfJetSel_              (new PFJetIDSelectionFunctor(pfJetIdParams_)),
  wJetSelector_          (params.getParameter<edm::ParameterSet>("BoostedTopWJetParameters") ),
  jetPt0_                (params.getParameter<double>("jetPt0") ),
  jetPt1_                (params.getParameter<double>("jetPt1") ),
  jetEta_                (params.getParameter<double>("jetEta") ),
  bTagOP_                (params.getParameter<double>("bTagOP") ),
  bTagAlgo_              (params.getParameter<string>("bTagAlgo") ),
  jetTag_               (params.getParameter<edm::InputTag>("jetSrc") )
{
  /*
    push_back("Inclusive");
    push_back("nJets >= 2");
    push_back("nJets >= 4");
    push_back("hasOneWTag");
    push_back("hasOneBTag");

    set("Inclusive");
    set("nJets >= 2");
    set("nJets >= 4");
    set("hasOneWTag");
    set("hasOneBTag");
  */
}

bool Type22Selection_v1::operator() ( edm::EventBase const & t, pat::strbitset & ret )
{
  ret.set(false);
  pfJets_.clear();
  highPtJets_.clear();
  wTags_.clear();
  bTags_.clear();

  //passCut( ret, "Inclusive" );
  bool type22_verbose = false;
  edm::Handle<vector<pat::Jet>  >   jetHandle;
  t.getByLabel( jetTag_, jetHandle );

  if (type22_verbose) std::cout<<"Type22Selection Event "<<t.id()<<endl;
  if (type22_verbose) std::cout<<"  jet must pass pt > "<< jetPt1_ <<" and eta < "<< jetEta_ <<std::endl;
  pat::strbitset retPFJet = pfJetSel_->getBitTemplate();
  // If ALL of the bits are ignored, don't use the PF Jet ID. 
  // The "getBitTemplate" function sets all bits to "false" except those ignored, which are "true"
  bool ignoreJetID = (bool)retPFJet;
  for( vector<pat::Jet>::const_iterator jetBegin=jetHandle->begin(), jetEnd=jetHandle->end(), ijet=jetBegin ;
       ijet!=jetEnd; ijet++ )
    {
      if (type22_verbose) std::cout<<"     Type22Selection ijet->pt() "<<ijet->pt()<<" ijet->eta() "<<ijet->eta()<<" ijet->phi() "<<ijet->phi();

      if( ijet->pt() > jetPt1_ && fabs( ijet->eta() ) < jetEta_ )  {
	retPFJet.set(false);
	if (type22_verbose) std::cout<<" -> passes cuts "<<std::endl;
	bool passJetID = ignoreJetID || (*pfJetSel_)( *ijet, retPFJet );
	if( passJetID ) {
	  pfJets_.push_back( edm::Ptr<pat::Jet>(jetHandle, ijet-jetBegin )  );
	  if( ijet->pt() > jetPt0_ )
	    highPtJets_.push_back( edm::Ptr<pat::Jet>(jetHandle, ijet-jetBegin )  );
	}
      } // end if jetPt, jetEta
      else {if (type22_verbose) std::cout<<" -> fails cuts "<<std::endl;}
    }

  //Search for W, b jets
  for( vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin=pfJets_.begin(), jetEnd=pfJets_.end(), ijet=jetBegin ;
       ijet!=jetEnd; ijet++ )
    {
      pat::Jet const & jet = **ijet;
      if (type22_verbose) 
	std::cout<<"     Type22Selection Loop 2 jet.pt() "
		 <<jet.pt()<<" jet.eta() "<<jet.eta()<<" jet.phi() "<<jet.phi()<<" btag "
		 <<jet.bDiscriminator( bTagAlgo_ )<<"  op "
		 <<bTagOP_<<" bTags_.size() "<<bTags_.size();
      pat::strbitset iret = wJetSelector_.getBitTemplate();
      if( wJetSelector_( jet, iret )  ) {
	if (type22_verbose) std::cout<<" ->W tagged!"<<std::endl;
	wTags_.push_back( *ijet );
      }  else {
	if( jet.bDiscriminator( bTagAlgo_ ) > bTagOP_ ){
	  bTags_.push_back( *ijet );
	  if (type22_verbose) std::cout<<" ->b tagged!"<<std::endl;
	}
	else {if (type22_verbose) std::cout<<" -> not tagged  "<<std::endl;}
      }
    }

  if (type22_verbose)  std::cout<<"   pfJets_.size() "<<pfJets_.size()<<endl;
  if (type22_verbose)  std::cout<<"   highPtJets_.size() "<<highPtJets_.size()<<endl;
  if (type22_verbose)  std::cout<<"   wTags_.size() "<<wTags_.size()<<endl;
  if (type22_verbose)  std::cout<<"   bTags_.size() "<<bTags_.size()<<endl;
  /*
    if( highPtJets_.size() >= 2 )  {
    passCut( ret, "nJets >= 2" );
    if( pfJets_.size() >= 4 ) {
    passCut( ret, "nJets >= 4" );
    if( wTags_.size() >= 1 )  {
    passCut( ret, "hasOneWTag" );
    if( bTags_.size() >= 1 )  {
    passCut( ret, "hasOneBTag" );
    }
    }
    }
    }
  */
  return bool(ret);

}
