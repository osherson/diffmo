#ifndef Analysis_BoostedTopAnalysis_interface_CATopTagFunctor_h
#define Analysis_BoostedTopAnalysis_interface_CATopTagFunctor_h


/**
  \class    CATopTagFunctor CATopTagFunctor.h "PhysicsTools/Utilities/interface/CATopTagFunctor.h"
  \brief    Jet selector for pat::Jets

  Selector functor for pat::Jets that implements top-jet tagging from
  the CATopTag algorithm

  \author Salvatore Rappoccio
  \version  $Id: CATopTagFunctor.h,v 1.3 2010/05/18 16:33:01 srappocc Exp $
*/




#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "PhysicsTools/SelectorUtils/interface/Selector.h"

#include <TMath.h>
class CATopTagFunctor : public Selector<pat::Jet>  {

 public: // interface

  enum Version_t { CALO, PF, N_VERSIONS };
  enum Quality_t { LOOSE, TIGHT, N_QUALITY};
  

 CATopTagFunctor( edm::ParameterSet const & params )
   {
     std::cout << "Instantiated CATopTagFunctor" << std::endl;
     std::string inputVersion = params.getParameter<std::string>("version");
     
     tagName_ = params.getParameter<std::string>("tagName");

     if ( inputVersion == "PF" ) version_ = PF;
     else version_ = CALO;
   
     push_back("Top Mass");
     push_back("Top Mass Min");
     push_back("Top Mass Max" );
     push_back("Min Mass");
     push_back("W Mass");
     push_back("W Mass Min" );
     push_back("W Mass Max"   );


     set("Top Mass",     params.getParameter<bool>  ("useTopMassCuts") );
     set("Top Mass Min", params.getParameter<double>("topMassMin")     );
     set("Top Mass Max", params.getParameter<double>("topMassMax")     );
     set("Min Mass",     params.getParameter<double>("minMass")        );
     set("W Mass",       params.getParameter<bool>  ("useWMassCuts")   );
     set("W Mass Min",   params.getParameter<double>("wMassMin")       );
     set("W Mass Max",   params.getParameter<double>("wMassMax")       );


     if ( params.exists("cutsToIgnore") )
       setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );
      
     retInternal_ = getBitTemplate();
  }

  // Allow for multiple definitions of the cuts. 
  bool operator()( const pat::Jet & jet, pat::strbitset & ret )  
  {
    if ( version_ == CALO || version_ == PF ) 
      return caloTag( jet, ret );
    else {
      return false;
    }
  }

  // cuts based on craft 08 analysis. 
  bool caloTag( const pat::Jet & jet, pat::strbitset & ret) 
  {
    
    const reco::CATopJetTagInfo * catopTag = 
      dynamic_cast<reco::CATopJetTagInfo const *>(jet.tagInfo(tagName_));

    // First check top mass cut
    if ( considerCut("Top Mass") ) {
      if ( ignoreCut("Top Mass Min")    || 
	   (catopTag->properties().topMass >= cut("Top Mass Min", double()) ) ) 
	passCut( ret, "Top Mass Min");
      if ( ignoreCut("Top Mass Max")    || 
	   (catopTag->properties().topMass <= cut("Top Mass Max", double()) ) ) 
	passCut( ret, "Top Mass Max");

      // If it passes the full range, set top mass to "pass"
      if ( ret[std::string("Top Mass Max")] && ret[std::string("Top Mass Min")] )
	passCut( ret, "Top Mass");      
    }else {
      passCut( ret, "Top Mass");
      passCut( ret, "Top Mass Max");
      passCut( ret, "Top Mass Min");
    }
    


    // Next check min mass cut
    if ( ignoreCut("Min Mass")    || 
	 (catopTag->properties().minMass >= cut("Min Mass", double()) ) ) 
      passCut( ret, "Min Mass");


    
    // Next check w mass cut
    if ( considerCut("W Mass") ) {
      if ( ignoreCut("W Mass Min")    || 
	   (catopTag->properties().wMass >= cut("W Mass Min", double()) ) ) 
	passCut( ret, "W Mass Min");
      if ( ignoreCut("W Mass Max")    || 
	   (catopTag->properties().wMass <= cut("W Mass Max", double()) ) ) 
	passCut( ret, "W Mass Max");

      // If it passes the full range, set w mass to "pass"
      if ( ret[std::string("W Mass Max")] && ret[std::string("W Mass Min")] )
	passCut( ret, "W Mass");
    } else {
      passCut( ret, "W Mass");
      passCut( ret, "W Mass Max");
      passCut( ret, "W Mass Min");
    }
    

    
    setIgnored( ret );

    // We're done, return
    return (bool)ret;
  }
  
 private: // member variables
  
  Version_t version_;
  Quality_t quality_;

  std::string tagName_;
  
};

#endif
