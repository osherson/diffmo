/*
How to use:      Make class instances to your required version, for example as

                 TopElectronSelector patEle70(TopElectronSelector::wp70);
                 for ( std::vector<pat::Electron>::const_iterator electronBegin = electronHandle->begin(),
                     electronEnd = electronHandle->end(), ielectron = electronBegin;
                  ielectron != electronEnd; ++ielectron ) {
                 bool pass70 = patEle70(*ielectron);
                 }

Contact:         Sadia Khalil (skhalil@fnal.gov)
*/
#ifndef PhysicsTools_PatUtils_interface_TopElectronSelector_h
#define PhysicsTools_PatUtils_interface_TopElectronSelector_h

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

//Math
#include "CLHEP/Units/GlobalPhysicalConstants.h"

class TopElectronSelector : public Selector<pat::Electron>  {

   public: // interface  
  
      enum Version_t { wp95, wp90, wp85, wp80, wp70, sigihih80, dphi80, deta80, hoe80, NONE };
      TopElectronSelector() {}
  
      // initialize it by inserting directly the cut values in a parameter set
       TopElectronSelector( edm::ParameterSet const & parameters)
      {
         // get the cuts from the PS
         initialize( parameters.getParameter<Double_t>("sihih_EB"), 
                     parameters.getParameter<Double_t>("dphi_EB"), 
                     parameters.getParameter<Double_t>("deta_EB"), 
                     parameters.getParameter<Double_t>("hoe_EB"),                      
                     parameters.getParameter<Double_t>("sihih_EE"), 
                     parameters.getParameter<Double_t>("dphi_EE"), 
                     parameters.getParameter<Double_t>("deta_EE"), 
                     parameters.getParameter<Double_t>("hoe_EE"));
         if ( parameters.exists("cutsToIgnore") )
            setIgnoredCuts( parameters.getParameter<std::vector<std::string> >("cutsToIgnore") );            
         retInternal_ = getBitTemplate();
      }
      // initialize it by using only the version name
      TopElectronSelector( Version_t  version)                                          
      {
         if (version == NONE) {
            std::cout << "TopElectronSelector: If you want to use version NONE "
                      << "then you also have to provide the selection cuts by yourself " << std::endl;
            std::cout << "TopElectronSelector: ID Version is changed to wp70 " << std::endl;
            version = wp70;
         }
         initialize(version);
         retInternal_ = getBitTemplate();
      }

      void initialize( Version_t version) 
      {
         version_ = version;
         // push back the variables
         
         push_back("sihih_EB"   );
         push_back("dphi_EB"    );
         push_back("deta_EB"    );
         push_back("hoe_EB"     );
         push_back("sihih_EE"   );
         push_back("dphi_EE"    );
         push_back("deta_EE"    );
         push_back("hoe_EE"     );                 
    
         if (version_ == wp95) {
            set("sihih_EB",    1.0e-02);
            set("dphi_EB",     8.0e-01);
            set("deta_EB",     7.0e-03);
            set("hoe_EB",      1.5e-01); 
            set("sihih_EE",    3.0e-02);
            set("dphi_EE",     7.0e-01);
            set("deta_EE",     1.0e-02);
            set("hoe_EE",      7.0e-02);
         }
         else if (version_ == wp90) {
            set("sihih_EB",    1.0e-02);
            set("dphi_EB",     8.0e-01);
            set("deta_EB",     7.0e-03);
            set("hoe_EB",      1.2e-01);
            set("sihih_EE",    3.0e-02);
            set("dphi_EE",     7.0e-01);
            set("deta_EE",     9.0e-03);
            set("hoe_EE",      5.0e-02);
         }
         else if (version_ == wp85) {
            set("sihih_EB",    1.0e-02);
            set("dphi_EB",     6.0e-02);
            set("deta_EB",     6.0e-03);
            set("hoe_EB",      4.0e-02);
            set("sihih_EE",    3.0e-02);
            set("dphi_EE",     4.0e-02);
            set("deta_EE",     7.0e-03);
            set("hoe_EE",      2.5e-02);
         }
         else if (version_ == wp80) {
            set("sihih_EB",    1.0e-02);
            set("dphi_EB",     6.0e-02);
            set("deta_EB",     4.0e-03);
            set("hoe_EB",      4.0e-02);
            set("sihih_EE",    3.0e-02);
            set("dphi_EE",     3.0e-02);
            set("deta_EE",     7.0e-03);
            set("hoe_EE",      2.5e-02);
         }
         else if (version_ == wp70) {
            set("sihih_EB",    1.0e-02);
            set("dphi_EB",     3.0e-02);
            set("deta_EB",     4.0e-03);
            set("hoe_EB",      2.5e-02);
            set("sihih_EE",    3.0e-02);
            set("dphi_EE",     2.0e-02);
            set("deta_EE",     5.0e-03); 
            set("hoe_EE",      2.5e-02);
         }
         else if (version == sigihih80){
            set("sihih_EB",    1.0e-02);
            set("sihih_EE",    3.0e-02);
         }
         else if (version == dphi80){
            set("dphi_EB",     6.0e-02);
            set("dphi_EE",     3.0e-02);
         }
         else if (version == deta80){
            set("deta_EB",     4.0e-03);
            set("deta_EE",     7.0e-03);//
         }
         else if (version == hoe80){
            set("hoe_EB",      4.0e-02);
            set("hoe_EE",      2.5e-02);
         }

         indexSinhih_EB_     = index_type(&bits_, "sihih_EB"     ); 
         indexDphi_EB_       = index_type(&bits_, "dphi_EB"      );
         indexDeta_EB_       = index_type(&bits_, "deta_EB"      ); 
         indexHoE_EB_        = index_type(&bits_, "hoe_EB"       ); 
         indexSinhih_EE_     = index_type(&bits_, "sihih_EE"     ); 
         indexDphi_EE_       = index_type(&bits_, "dphi_EE"      );
         indexDeta_EE_       = index_type(&bits_, "deta_EE"      ); 
         indexHoE_EE_        = index_type(&bits_, "hoe_EE"       ); 
      }

      void initialize(Double_t sihih_EB, Double_t  dphi_EB, Double_t deta_EB, Double_t hoe_EB,
                      Double_t sihih_EE, Double_t  dphi_EE, Double_t deta_EE, Double_t hoe_EE)
      {
         version_ = NONE;
         push_back("sihih_EB"   );
         push_back("dphi_EB"    );
         push_back("deta_EB"    );
         push_back("hoe_EB"     );
         push_back("sihih_EE"   );
         push_back("dphi_EE"    );
         push_back("deta_EE"    );
         push_back("hoe_EE"     );
              
         set("sihih_EB",    sihih_EB);
         set("dphi_EB",     dphi_EB);
         set("deta_EB",     deta_EB);
         set("hoe_EB",      hoe_EB);
         set("sihih_EE",    sihih_EE);
         set("dphi_EE",     dphi_EE);
         set("deta_EE",     deta_EE);
         set("hoe_EE",      hoe_EE);        
      }

      bool operator()( const pat::Electron & electron, pat::strbitset & ret ) 
      {
         // for the time being only Spring11 variable definition
         return spring11Cuts(electron, ret);
      }
      using Selector<pat::Electron>::operator();
      // function with the Spring11 variable definitions
      bool spring11Cuts( const pat::Electron & electron, pat::strbitset & ret) 
      {
         ret.set(false);
        
         Double_t Deta  = electron.deltaEtaSuperClusterTrackAtVtx();
         Double_t Dphi  = electron.deltaPhiSuperClusterTrackAtVtx(); 
         Double_t sihih = electron.sigmaIetaIeta();
         Double_t HoE   = electron.hadronicOverEm();       
         
         // now apply the cuts
         if (electron.isEB()) { // BARREL case
            // check the EB cuts
            if ( sihih      <  cut(indexSinhih_EB_,double()) || ignoreCut(indexSinhih_EB_)) passCut(ret, indexSinhih_EB_);
            if ( fabs(Dphi) <  cut(indexDphi_EB_,  double()) || ignoreCut(indexDphi_EB_)  ) passCut(ret, indexDphi_EB_);
            if ( fabs(Deta) <  cut(indexDeta_EB_,  double()) || ignoreCut(indexDeta_EB_)  ) passCut(ret, indexDeta_EB_);
            if ( HoE        <  cut(indexHoE_EB_,   double()) || ignoreCut(indexHoE_EB_)   ) passCut(ret, indexHoE_EB_);
            // pass all the EE cuts
            passCut(ret, indexSinhih_EE_);	
            passCut(ret, indexDphi_EE_);	
            passCut(ret, indexDeta_EE_);	
            passCut(ret, indexHoE_EE_);	
             
         } else {  // ENDCAPS case
            // check the EE cuts
            if ( sihih      <  cut(indexSinhih_EE_,double()) || ignoreCut(indexSinhih_EE_)) passCut(ret, indexSinhih_EE_);
            if ( fabs(Dphi) <  cut(indexDphi_EE_,  double()) || ignoreCut(indexDphi_EE_)  ) passCut(ret, indexDphi_EE_);
            if ( fabs(Deta) <  cut(indexDeta_EE_,  double()) || ignoreCut(indexDeta_EE_)  ) passCut(ret, indexDeta_EE_);
            if ( HoE        <  cut(indexHoE_EE_,   double()) || ignoreCut(indexHoE_EE_)   ) passCut(ret, indexHoE_EE_);
            // pass all the EB cuts
            passCut(ret, indexSinhih_EB_);	
            passCut(ret, indexDphi_EB_);	
            passCut(ret, indexDeta_EB_);	
            passCut(ret, indexHoE_EB_);
            
         }
        
         setIgnored(ret);   
         return (bool)ret;
      }
    
   private: // member variables
      // version of the cuts  
      Version_t version_;
      index_type indexSinhih_EB_;
      index_type indexDphi_EB_;
      index_type indexDeta_EB_;
      index_type indexHoE_EB_;
      index_type indexSinhih_EE_;
      index_type indexDphi_EE_;
      index_type indexDeta_EE_;
      index_type indexHoE_EE_;
};

#endif
