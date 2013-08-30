
#include "Analysis/BoostedTopAnalysis/interface/HadronicSelectionBoostedW.h"

#include <iostream>

using namespace std;

HadronicSelectionBoostedW::HadronicSelectionBoostedW( 
    boost::shared_ptr<WPlusJetsEventSelector> wPlusJets,
    edm::InputTag const & jetTag,
    edm::InputTag const & trigTag,
    double jetPtMin,
    double jetEtaMax,
    double mu,
    double ycut,
    double dPhi
    ) :
  wPlusJets_(wPlusJets),
  jetTag_(jetTag),
  trigTag_(trigTag),
  jetPtMin_(jetPtMin),
  jetEtaMax_(jetEtaMax),
  mu_(mu),
  ycut_(ycut),
  dPhi_(dPhi)
{
  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 Jet"       );
  push_back( ">= 1 Tight Jet" );
  push_back( ">= 1 Tag"       );


  // all on by default
  set( "Inclusive"      );
  set( "Trigger"        );
  set( ">= 1 Jet"       );
  set( ">= 1 Tight Jet" );
  set( ">= 1 Tag"       );


}

bool HadronicSelectionBoostedW::operator() ( fwlite::EventContainer const & eventConst, pat::strbitset & ret)
{
  selectedJets_.clear();
  taggedJets_.clear();

  fwlite::EventContainer & event = const_cast<fwlite::EventContainer &>(eventConst);

  passCut( ret, "Inclusive");

  // Get all the jets
  edm::Handle< vector< pat::Jet > > jetHandle;
  eventConst.getByLabel (jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) return (bool)ret;

  // Get the trigger
  edm::Handle<pat::TriggerEvent> triggerEvent;
  eventConst.getByLabel(edm::InputTag("patTriggerEvent"), triggerEvent);
  if (!triggerEvent.isValid() ) return (bool)ret;  

  std::vector<reco::ShallowClonePtrCandidate> const & selectedMuons = wPlusJets_->selectedMuons();
  
  pat::Muon const * leadingMuon = dynamic_cast<pat::Muon const *> (&(selectedMuons.front()));


  // Get a list of the jets that pass our tight event selection
  for ( std::vector<pat::Jet>::const_iterator jetBegin = jetHandle->begin(),
	  jetEnd = jetHandle->end(), ijet = jetBegin;
	ijet != jetEnd; ++ijet ) {

    double dPhi = deltaPhi<double>( ijet->phi(), leadingMuon->phi() );

    event.hist("deltaPhi")->Fill( dPhi );

    if ( ijet->pt() > jetPtMin_ && 
	 fabs(ijet->eta()) < jetEtaMax_ &&
	 dPhi > dPhi_ ) {

      // calculate the mass drop:
      double m_fat = ijet->mass();
      if ( ijet->numberOfDaughters() == 2 && m_fat > 0 ) {

	event.hist("m")->Fill( m_fat );

	selectedJets_.push_back( ijet );

	double m0 = ijet->daughter(0)->mass();
	double m1 = ijet->daughter(1)->mass();
	double pt0 = ijet->daughter(0)->pt();
	double pt1 = ijet->daughter(1)->pt();

	if ( m1 > m0 ) {
	  double temp = m1;
	  m1 = m0;
	  m0 = temp;

	  temp = pt1;
	  pt1 = pt0;
	  pt0 = temp;
	}

	double dR = reco::deltaR<double>( ijet->daughter(0)->eta(),
					  ijet->daughter(0)->phi(),
					  ijet->daughter(1)->eta(),
					  ijet->daughter(1)->phi() );


	// Note that y ~ min(pt0^2,pt1^2) / max(pt0^2,pt1^2)
	double y = std::min( pt0*pt0, pt1*pt1) * dR*dR / (m_fat*m_fat);

	event.hist("m0")->Fill( m0 );
	event.hist("m1")->Fill( m1 );
	event.hist("pt0")->Fill( pt0 );
	event.hist("pt1")->Fill( pt1 );
	event.hist("deltaR")->Fill( dR );
	event.hist("y")->Fill( y );
	event.hist("m0_over_m")->Fill( m0 / m_fat );
	
	  
	if ( m0 / m_fat < mu_ &&
	     y > ycut_
	     ) {
	  taggedJets_.push_back( ijet );
	}
      }
    }
  }

  // Check the trigger requirement
  pat::TriggerEvent const * trig = &*triggerEvent;

  bool passTrig = false;
  if ( trig->wasRun() && trig->wasAccept() ) {
    pat::TriggerPath const * jetPath = trig->path("HLT_Jet110");
    if ( jetPath != 0 && jetPath->wasAccept() ) {
      passTrig = true;    
    }
  }

  // Now check trigger requirement
  if ( ignoreCut("Trigger") || 
       passTrig ) {
    passCut(ret, "Trigger");

    // Now check if there is at least one jet with pt,Y cuts
    if ( ignoreCut(">= 1 Jet") ||
	 static_cast<int>(jetHandle->size()) > 0 ){
      passCut(ret,">= 1 Jet");

      // Now check if there is at least one jet that passes the jet ID cuts
      if ( ignoreCut(">= 1 Tight Jet") ||
	   static_cast<int>(selectedJets_.size()) > 0 ){
	passCut(ret,">= 1 Tight Jet");
      

	// Now look for >= 1 tags
	if ( ignoreCut(">= 1 Tag") ||
	     static_cast<int>(taggedJets_.size()) > 0 ){
	  passCut(ret,">= 1 Tag");
	  
	}// end if >= 1 Tag	
      
      }// End if >= 1 Tight Jet
      
    }// End if >= 1 Jet	  
    
  } // end if trigger
  
  
  return (bool)ret;
}
