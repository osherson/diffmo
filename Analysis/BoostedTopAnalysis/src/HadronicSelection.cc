
#include "Analysis/BoostedTopAnalysis/interface/HadronicSelection.h"

#include <iostream>

using namespace std;

HadronicSelection::HadronicSelection( edm::ParameterSet const & params ) :
  dijetSelector_         (params.getParameter<edm::ParameterSet>("jetIDParams"), 
			  params.getParameter<edm::ParameterSet>("pfJetIDParams"),
			  params.getParameter<edm::ParameterSet>("dijetSelectorParams")),
  caTopTagFunctor_       (params.getParameter<edm::ParameterSet>("caTopTagParams") ),
  boostedTopWTagFunctor_ (params.getParameter<edm::ParameterSet>("boostedTopWTagParams") ),
  pvSrc_                 (params.getParameter<edm::InputTag>("pvSrc")),
  trigSrc_               (params.getParameter<edm::InputTag>("trigSrc") ),
  trig_                  (params.getParameter<std::string>("trig") ),
  minTags_               (params.getParameter<unsigned int>("minTags") ),
  usePF_                 (params.getParameter<bool>("usePF")),
  useWTag_               (params.getParameter<bool>("useWTag"))
{
  std::cout << "Instantiated HadronicSelection" << std::endl;
  // make the bitset
  push_back( "Inclusive"       );
  push_back( "Trigger"         );
  push_back( "Jet Preselection");
  push_back( "Min nPV"         );
  push_back( "Max nPV"         );
  push_back( ">= 1 Tag"        );
  push_back( ">= N Tags"       );

  // all on by default
  set( "Inclusive"        );
  set( "Trigger"          );
  set( "Jet Preselection" );

  if ( params.exists("minNPV")) 
    set( "Min nPV"          , params.getParameter<int>("minNPV") );
  else
    set( "Min nPV"          , 0);

  if ( params.exists("maxNPV")) 
    set( "Max nPV"          , params.getParameter<int>("maxNPV") );
  else
    set( "Max nPV"          , 10000);

  set( ">= 1 Tag"         );
  set( ">= N Tags",  minTags_ );

  
  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );

  retInternal_ = getBitTemplate();
}

bool HadronicSelection::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{
  taggedJets_.clear();

  passCut( ret, "Inclusive");


  // Get the trigger
  edm::Handle<pat::TriggerEvent> triggerEvent;
  event.getByLabel( trigSrc_, triggerEvent);
  if (!triggerEvent.isValid() ) return (bool)ret;  


  // Check the trigger requirement
  pat::TriggerEvent const * trig = &*triggerEvent;

  bool passTrig = false;
  if ( trig->wasRun() && trig->wasAccept() ) {
    pat::TriggerPath const * jetPath = trig->path(trig_);
    if ( jetPath != 0 && jetPath->wasAccept() ) {
      passTrig = true;    
    }
  }

  // Now check trigger requirement
  if ( ignoreCut("Trigger") || 
       passTrig ) {
    passCut(ret, "Trigger");
    
    edm::Handle< std::vector<reco::Vertex>  > pvHandle;
    event.getByLabel( pvSrc_, pvHandle);

    int npv = pvHandle->size();

    if ( ignoreCut("Min nPV") || npv >= cut("Min nPV", int()) ) {
      passCut( ret, "Min nPV");

      if ( ignoreCut("Max nPV") || npv <= cut("Max nPV", int()) ) {
	passCut( ret, "Max nPV");




	// Get the good inclusive jets
	pat::strbitset jetRet = dijetSelector_.getBitTemplate();
	bool passDijet = dijetSelector_( event, jetRet );
	std::vector<edm::Ptr<pat::Jet> > const & inclusiveJets = (!usePF_) ? dijetSelector_.caloJets() : dijetSelector_.pfJets();

	// Get a list of the jets that pass our tag requirements
	for ( std::vector<edm::Ptr<pat::Jet> >::const_iterator jetBegin = inclusiveJets.begin(),
		jetEnd = inclusiveJets.end(), ijet = jetBegin;
	      ijet != jetEnd; ++ijet ) {
	  if ( !useWTag_ ) {
	    pat::strbitset ret = caTopTagFunctor_.getBitTemplate();
	    if ( caTopTagFunctor_(**ijet, ret ) ) {
	      taggedJets_.push_back( *ijet );
	    }
	  } else {
	    pat::strbitset ret = boostedTopWTagFunctor_.getBitTemplate();
	    if ( boostedTopWTagFunctor_(**ijet, ret ) ) {
	      taggedJets_.push_back( *ijet );
	    }
	  }
	  
	}
	
    
	// Now check if there is at least one jet with pt,Y cuts
	if ( ignoreCut("Jet Preselection") ||
	     inclusiveJets.size() >= 1 ){
	  passCut(ret,"Jet Preselection");


	  // Now look for >= 1 tags
	  if ( ignoreCut(">= 1 Tag") ||
	       static_cast<int>(taggedJets_.size()) > 0 ){
	    passCut(ret,">= 1 Tag");

	    // Next require at least N (configurable) tags
	    if ( ignoreCut(">= N Tags") ||
		 static_cast<int>(taggedJets_.size()) >=  this->cut(">= N Tags", int()) ){
	      passCut(ret,">= N Tags");
	    }// end if >= N Tags
	    
	  }// end if >= 1 Tag
	  
	}// End if jet preselection
	
      }// max npv
      
    } // min npv
    
  } // end if trigger
  

  setIgnored(ret);
  
  return (bool)ret;
}
