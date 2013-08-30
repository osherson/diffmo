#include "Analysis/BoostedTopAnalysis/interface/Type11Selection_v1.h"

using namespace std;

Type11Selection_v1::Type11Selection_v1 ( edm::ParameterSet const & params ) :
  pfJetIdParams_         (params.getParameter<edm::ParameterSet>("pfJetIDParams") ),
  pfJetSel_              (new PFJetIDSelectionFunctor(pfJetIdParams_)),
  caTopJetPtMin_                (params.getParameter<double>("caTopJetPtMin") ),
  caTopJetEtaCut_                (params.getParameter<double>("caTopJetEtaCut") ),
  patJetCollectionInputTag_ (params.getParameter<edm::InputTag>("patJetCollectionInputTag") ),
  caTopJetCollectionInputTag_ (params.getParameter<edm::InputTag>("caTopJetCollectionInputTag") )
{
  std::cout<< "Instantiated Type11Selection_v1" <<std::endl;

}

bool Type11Selection_v1::operator() ( edm::EventBase const & t, pat::strbitset & ret )
{
  ret.set(false);
  caTopJets_.clear();
  ca8Jets_.clear();

  bool type11_verbose = false;

  edm::Handle<vector<pat::Jet>  >   caTopJetHandle;
  t.getByLabel( caTopJetCollectionInputTag_, caTopJetHandle );

  edm::Handle<vector<pat::Jet>  >   patJetHandle;
  t.getByLabel( patJetCollectionInputTag_, patJetHandle );
	 
  if (type11_verbose) std::cout<<"Type11Selection Event "<<t.id()<<std::endl;
  if (type11_verbose) std::cout<<"  jet must pass pt > "<< caTopJetPtMin_ <<" and eta < "<< caTopJetEtaCut_ <<std::endl;

  pat::strbitset retPFJet = pfJetSel_->getBitTemplate();
  // If ALL of the bits are ignored, don't use the PF Jet ID. 
  // The "getBitTemplate" function sets all bits to "false" except those ignored, which are "true"
  bool ignoreJetID = (bool)retPFJet;
  for( vector<pat::Jet>::const_iterator jetBegin=patJetHandle->begin(), jetEnd=patJetHandle->end(), ijet=jetBegin ; ijet!=jetEnd; ijet++ )
    {		
      if (type11_verbose) std::cout<<"     Type11Selection ijet->pt() "<<ijet->pt()<<" ijet->eta() "<<ijet->eta()<<" ijet->phi() "<<ijet->phi();
      if ( ijet->pt() > caTopJetPtMin_ && fabs( ijet->eta() ) < caTopJetEtaCut_)
	{
	  if (type11_verbose) std::cout<<" -> passes cuts ";

	  //Jet ID is applied to the patJet. The caTop jet is then matched to the patJet
	  // You can't apply jet ID to the caTop jet because
	  // PF Jet ID removes all jets with 0 or 1 constituents. 
	  // the constituents of caTop jets are subjets, not particles, so this requirement does not work
	  retPFJet.set(false);
	  bool passJetID = ignoreJetID || (*pfJetSel_)( *ijet, retPFJet );
	  if( passJetID ) 
	    {
	      ca8Jets_.push_back( edm::Ptr<pat::Jet>(patJetHandle, ijet-jetBegin )  );
	      if (type11_verbose) std::cout<<" -> passes jetID "<<std::endl;
	      //match a caTopJet
	      for( vector<pat::Jet>::const_iterator caJetBegin=caTopJetHandle->begin(), caJetEnd=caTopJetHandle->end(), jjet=caJetBegin ; jjet!=caJetEnd; jjet++ )
		{
		  if (type11_verbose) std::cout<<"         Loop2 to match: jjet->pt() "<<jjet->pt()<<" jjet->eta() "<<jjet->eta()<<" jjet->phi() "<<jjet->phi();
		  if ( ijet->pt() == jjet->pt() &&  ijet->eta() == jjet->eta() )
		    {
		      caTopJets_.push_back( edm::Ptr<pat::Jet>(caTopJetHandle, jjet-caJetBegin )  );
		      if (type11_verbose) std::cout<<"  ->matched!  caTopJets_.size()"<<caTopJets_.size()<<std::endl;
		    }
		  else {if (type11_verbose) std::cout<<" "<<std::endl;}
		}
	    }
	  else {if (type11_verbose) std::cout<<" -> does not pass jet ID "<<std::endl;}
	}
      else {if (type11_verbose) std::cout<<" -> fails cuts "<<std::endl;}
    }
  if (type11_verbose) std::cout<<"   ca8Jets_.size() "<<ca8Jets_.size()<<endl;
  if (type11_verbose) std::cout<<"   caTopJets_.size() "<<caTopJets_.size()<<endl;

  return bool(ret);

}
