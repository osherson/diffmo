#include "Analysis/SHyFT/interface/MuonInJetSelector.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"

#include <iostream>

using namespace std;

MuonInJetSelector::MuonInJetSelector( edm::ParameterSet const & params ) :
   EventSelector(),
   muonTag_         (params.getParameter<edm::InputTag>("muonSrc") ),
   jetTag_          (params.getParameter<edm::InputTag>("jetSrc") ),
   muonId_          (params.getParameter<edm::ParameterSet>("muonId") ),
   pfjetId_         (params.getParameter<edm::ParameterSet>("pfjetId") ),
   muJetDR_         (params.getParameter<double>("muJetDR")),
   muPtMin_         (params.getParameter<double>("muPtMin")), 
   muEtaMax_        (params.getParameter<double>("muEtaMax")), 
   jetPtMin_        (params.getParameter<double>("jetPtMin")), 
   jetEtaMax_       (params.getParameter<double>("jetEtaMax")),
   btagDisc1_       (params.getParameter<std::string>("btagDisc1")),
   btagDisc2_       (params.getParameter<std::string>("btagDisc2")),
   btagDiscCut1_    (params.getParameter<double>("btagDiscCut1")),
   btagDiscCut2_    (params.getParameter<double>("btagDiscCut2"))
{
   // make the bitset
   push_back( "Inclusive"      );
   push_back( ">= 1 Muon"      );
   push_back( ">= 2 Muon"      );
   push_back( ">=1 Jets"       );
   push_back( ">=1 Tags"       );
   push_back( ">=2 Jets"       );
   push_back( ">=2 Tags"       );


   // turn (almost) everything on by default
   set( "Inclusive"      );
   set( ">= 1 Muon"      );
   set( ">= 2 Muon"      );
   set( ">=1 Jets"       );
   set( ">=1 Tags"       );
   set( ">=2 Jets"       );
   set( ">=2 Tags"       );

   inclusiveIndex_ = index_type(&bits_, std::string("Inclusive"      ));

   mu1_          = index_type(&bits_, std::string(">= 1 Muon"));
   mu2_          = index_type(&bits_, std::string(">= 2 Muon"));
   jet1Index_    = index_type(&bits_, std::string(">=1 Jets"));
   jet1TagIndex_ = index_type(&bits_, std::string(">=1 Tags"));
   jet2Index_    = index_type(&bits_, std::string(">=2 Jets"));
   jet2TagIndex_ = index_type(&bits_, std::string(">=2 Tags"));

   if ( params.exists("cutsToIgnore") )
      setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );
	

   retInternal_ = getBitTemplate();
}

bool MuonInJetSelector::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{

   ret.set(false);
   selectedJets_.clear();
   selectedMuons_.clear();

   passCut( ret, inclusiveIndex_);

   edm::Handle< vector< pat::Muon > > muonHandle;
   event.getByLabel (muonTag_, muonHandle);


   for ( std::vector<pat::Muon>::const_iterator muonBegin = muonHandle->begin(),
	   muonEnd = muonHandle->end(), imuon = muonBegin;
	 imuon != muonEnd; ++imuon ) {
     if ( !imuon->isGlobalMuon() ) continue;
     
     // Tight cuts
     bool passTight = muonId_(*imuon,event) ;
     if (  imuon->pt() > muPtMin_ && fabs(imuon->eta()) < muEtaMax_ && 
	   passTight ) {       
       selectedMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Muon>( muonHandle, imuon - muonBegin ) ) );
     }
   }

   int nleptons = selectedMuons_.size();
   if ( ignoreCut(mu1_) || 
	( nleptons >=1 && nleptons < 3 ) ){

     passCut( ret, mu1_);
     if ( ignoreCut(mu2_) || 
	  ( nleptons == 2 ) ){
       passCut( ret, mu2_);

       edm::Handle< vector< pat::Jet > > jetHandle;
       event.getByLabel (jetTag_, jetHandle);

       pat::strbitset ret2 = pfjetId_.getBitTemplate();
       for ( std::vector<pat::Jet>::const_iterator jetBegin = jetHandle->begin(),
	       jetEnd = jetHandle->end(), ijet = jetBegin;
	     ijet != jetEnd; ++ijet ) {
	 reco::ShallowClonePtrCandidate scaledJet ( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Jet>( jetHandle, ijet - jetBegin ),
										    ijet->charge(),
										    ijet->p4() ) );    
	 bool passJetID = pfjetId_(*ijet, ret2);

	 if ( scaledJet.pt() > jetPtMin_ && fabs(scaledJet.eta()) < jetEtaMax_ && passJetID ) {
	   //Add only jets associated to a muon with a dR cone
	   bool indeltaR = false;
	   for( std::vector<reco::ShallowClonePtrCandidate>::const_iterator muonBegin = selectedMuons_.begin(),
		  muonEnd = selectedMuons_.end(), imuon = muonBegin;
		imuon != muonEnd; ++imuon ) {
	     double dR = reco::deltaR( imuon->eta(), imuon->phi(), scaledJet.eta(), scaledJet.phi() ) ;
	     if( dR < muJetDR_ )
	       { indeltaR = true; }
	   }
	   if ( ignoreCut(mu2_) || indeltaR ) {
	     selectedJets_.push_back(scaledJet);
	   }
       
       
	 }// end if pass id and kin cuts
       }// end loop over jets

       
       if ( ignoreCut(jet1Index_) ||
	    static_cast<int>(selectedJets_.size()) >=  1 ){
	 passCut(ret,jet1Index_);

	 pat::Jet const * patJet1 = dynamic_cast<pat::Jet const *>(selectedJets_[0].masterClonePtr().get());
	 if ( patJet1 == 0 ) {
	   throw cms::Exception("InvalidType") << "This clone is not pointing to a pat::Jet" << std::endl;
	 }
	 bool tag1 = patJet1->bDiscriminator(btagDisc1_) > btagDiscCut1_;
	 if ( ignoreCut(jet1TagIndex_ ) ||
	      tag1 ) {
	   passCut( ret, jet1TagIndex_ );

	   if ( ignoreCut(jet2Index_) ||
		static_cast<int>(selectedJets_.size()) >=  2 ){
	     passCut(ret,jet2Index_);  

	     pat::Jet const * patJet2 = dynamic_cast<pat::Jet const *>(selectedJets_[1].masterClonePtr().get());
	     bool tag2 = patJet2->bDiscriminator(btagDisc2_) > btagDiscCut2_;
	     if ( ignoreCut(jet2TagIndex_ ) ||
		  tag2 ) {
	       passCut( ret, jet2TagIndex_ );
	     } // end if second jet tagged
	   } // end if >=2 tight jets
	 }// end if first jet tagged
       } // end if >=1 tight jets     
     }// end if == 2 muons
   }// end if 1 or 2 muons
   
   setIgnored(ret);
   return (bool)ret;
}
