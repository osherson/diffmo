#ifndef Analysis_BoostedTopAnalysis_interface_BoostedTopWTagFunctor_h
#define Analysis_BoostedTopAnalysis_interface_BoostedTopWTagFunctor_h

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "Analysis/BoostedTopAnalysis/interface/SubjetHelper.h"

#include <TMath.h>

class BoostedTopWTagFunctor : public Selector<pat::Jet> {

  public:

 BoostedTopWTagFunctor( edm::ParameterSet const & params ) 
  {
    std::cout << "Instantiated BoostedTopWTagFunctor" << std::endl;
    initialize( params.getParameter<double>("jetPtMin"),
		params.getParameter<double>("jetEtaMax"),
		params.getParameter<double>("muMax"),
		params.getParameter<double>("ycut"),
		params.getParameter<unsigned int>("numOfDaughters") );
  }


 BoostedTopWTagFunctor(  double jetPtMin = 30.0,
			 double jetEtaMax = 2.4,
			 double muMax = 0.7,
			 double ycut = 0.1,
			 unsigned int numOfDaughters=2 ){
   initialize( jetPtMin, jetEtaMax, muMax, ycut, numOfDaughters );
 }
     
     void initialize (  double jetPtMin = 30.0,
			double jetEtaMax = 2.4,
			double muMax = 0.7,
			double ycut = 0.1,
			unsigned int numOfDaughters=2 )
	{  
	   //cout<<"Construct wTag Functor"<<endl;

	   push_back("jetPtMin");
	   push_back("jetEtaMax");
	   push_back("mu");
	   push_back("ycut");
	   push_back("numOfDaughters");

	   set("jetPtMin",	jetPtMin );
	   set("jetEtaMax",	jetEtaMax );
	   set("mu",		muMax );
	   set("ycut",		ycut );
	   set("numOfDaughters",  (int) numOfDaughters );

	   //std::cout<<cut("jetPtMin",  double())<<" "<<cut("jetEtaMax",  double() )<<" "
	   //<<cut("mu", double() )<<" "<<cut("ycut", double() ) <<" "<<cut("numOfDaughters", int() ) <<std::endl;
	   
	}

     bool operator() ( const pat::Jet & jet , pat::strbitset & ret )  
     {
       //Clear cache
       ret.set(false);
       int numOfDaughters 	= jet.numberOfDaughters();
       double pt		= jet.pt();
       double eta		= jet.eta();
       /* double m_fat            = jet.mass(); */
       if( pt >        cut("jetPtMin", double() ) || ignoreCut("jetPtMin" ) )   passCut(ret, "jetPtMin" );
       if( fabs(eta )  < cut("jetEtaMax", double() ) || ignoreCut("jetEtaMax" ) ) passCut(ret, "jetEtaMax");
       if( numOfDaughters == cut("numOfDaughters", int() ) || ignoreCut("numOfDaughters")  )     passCut(ret, "numOfDaughters" );
       else  { return false; }
       double y = 0.0, mu = 0.0, dR = 0.0;

       pat::subjetHelper( jet, y, mu, dR );
       if( mu            < cut("mu", double() ) || ignoreCut("mu")   )   passCut(ret, "mu" );
       if( y >		  cut("ycut", double() ) || ignoreCut("ycut") ) passCut(ret, "ycut" );
       return (bool) ret;

     }
     using Selector<pat::Jet>::operator();

};

#endif
