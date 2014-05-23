// -*- C++ -*-
//
// Package:    BoostedTopTrigger
// Class:      BoostedTopTrigger
// 
/**\class BoostedTopTrigger BoostedTopTrigger.cc Analysis/BoostedTopTrigger/src/BoostedTopTrigger.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Benedikt Hegner
//         Created:  Sat July 15 11:39:14 CDT 2011
// $Id: BoostedTopTrigger.cc,v 1.2 2011/10/07 18:19:25 srappocc Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "TPRegexp.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH2F.h"
#include "TH1F.h"
//
// class declaration
//

using namespace std;
using trigger::Keys;
using trigger::TriggerObject;
using edm::InputTag;



class BoostedTopTrigger : public edm::EDAnalyzer {
   public:
      explicit BoostedTopTrigger(const edm::ParameterSet&);
      ~BoostedTopTrigger();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      virtual void getAodTriggerObjectsForModule (edm::InputTag collectionTag,
                                                  edm::Handle<trigger::TriggerEvent> aodTriggerEvent,
                                                  trigger::TriggerObjectCollection trigObjs,
                                                  std::vector<TriggerObject> & foundObjects  );

      // ----------member data ---------------------------

      std::string targetTrigger;
      double threshold_;
      std::string theHltProcessName; 
      bool verbose_;
      InputTag nameOfHLTFinalFilterModule;
      InputTag jetTag1_;     
      InputTag jetTag2_;

  TH1D * jetPtType11Num;
  TH1D * jetPtType11Den;
  TH1D * jetPtType12Num;
  TH1D * jetPtType12Den;

  TH1D * HThistType11;
  TH1D * HThistType12;
  TH1D * HThistAll;

  TH1D * MassType11_low;
  TH1D * MassType12_low;

  TH1D * MassType11_high;
  TH1D * MassType11_all;
  TH1D * MassType12_high;
  TH1D * MassType12_all;
  
int nPassed11_;
  int nTotal11_;
  int nPassed12_;
  int nTotal12_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
BoostedTopTrigger::BoostedTopTrigger(const edm::ParameterSet& iConfig)

{
  verbose_ = iConfig.getUntrackedParameter<bool>("Verbose", false);
  theHltProcessName = iConfig.getUntrackedParameter<std::string>("HltProcessName", "HLT");
  targetTrigger = iConfig.getUntrackedParameter<std::string>("TargetTrigger", "HLT_Jet300_v1");
  threshold_ = iConfig.getUntrackedParameter<double>("threshold", 320.0);
  jetTag1_ = iConfig.getUntrackedParameter<edm::InputTag>("jetTag1");
  jetTag2_ = iConfig.getUntrackedParameter<edm::InputTag>("jetTag2");

  edm::Service<TFileService> fs;
  nPassed11_ = nTotal11_ = nPassed12_ = nTotal12_ = 0;

  HThistType11 = fs->make<TH1D>("HThistType11", "HT, Type 1+1", 200, 0, 2000);
  HThistType12 = fs->make<TH1D>("HThistType12", "HT, Type 1+2", 200, 0, 2000);
  HThistAll = fs->make<TH1D>("HThistAll", "HT, All Events", 200, 0, 2000);

  MassType11_low = fs->make<TH1D>("MassType11_low", "Inv. Mass Type 1+1", 500, 0, 5000);
  MassType12_low = fs->make<TH1D>("MassType12_low", "Inv. Mass Type 1+2", 500, 0, 5000);
  
  MassType11_high = fs->make<TH1D>("MassType11_high", "Inv. Mass Type 1+1", 500, 0, 5000);
  MassType12_high = fs->make<TH1D>("MassType12_high", "Inv. Mass Type 1+2", 500, 0, 5000);

  MassType11_all = fs->make<TH1D>("MassType11_all", "Inv. Mass Type 1+1", 500, 0, 5000);
  MassType12_all = fs->make<TH1D>("MassType12_all", "Inv. Mass Type 1+2", 500, 0, 5000);
  
  jetPtType11Num = fs->make<TH1D>("jetPtType11Num",  "Jet p_{T}, Type 1 + 1, Numerator",   150, 0., 1500.);
  jetPtType11Den = fs->make<TH1D>("jetPtType11Den",  "Jet p_{T}, Type 1 + 1, Denominator", 150, 0., 1500.);


  jetPtType12Num = fs->make<TH1D>("jetPtType12Num",  "Jet p_{T}, Type 1 + 1, Numerator",   150, 0., 1500.);
  jetPtType12Den = fs->make<TH1D>("jetPtType12Den",  "Jet p_{T}, Type 1 + 1, Denominator", 150, 0., 1500.);
}



BoostedTopTrigger::~BoostedTopTrigger()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
BoostedTopTrigger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle<trigger::TriggerEvent>         aodTriggerEvent;
   InputTag HltAodInputTag("hltTriggerSummaryAOD","","");
   iEvent.getByLabel(HltAodInputTag, aodTriggerEvent);

   edm::Handle<vector<pat::Jet>  >   jetHandle1;
   iEvent.getByLabel( jetTag1_,   jetHandle1 );
   edm::Handle<vector<pat::Jet>  >   jetHandle2;
   iEvent.getByLabel( jetTag2_,   jetHandle2 );

   edm::Handle< std::vector<reco::Vertex> > h_pv;
   iEvent.getByLabel( "goodOfflinePrimaryVertices", h_pv );
 
   unsigned int npv = h_pv->size();
   const trigger::TriggerObjectCollection trigObjs = aodTriggerEvent->getObjects();
   std::vector<TriggerObject> foundHLTTrigObjs; 

   getAodTriggerObjectsForModule ( nameOfHLTFinalFilterModule,
				   aodTriggerEvent,
				   trigObjs,
				   foundHLTTrigObjs );


   const vector<pat::Jet> *ca8Jets = jetHandle1.product();

   float HTsum = 0.0;
   int Nbtag = 0;
   for (unsigned int i=0; i < ca8Jets->size(); i++){
	
	pat::Jet thisJet = (*ca8Jets)[i];
	HTsum += thisJet.pt();
	if (thisJet.bDiscriminator("combinedSecondaryVertexBJetTags") > 0.679) Nbtag++;
   }



   /// Type 1+1

   bool pass11 = false;
   bool pass12 = false;
   if ( jetHandle1->size() >= 2 ) {
     /// ...


     pat::Jet const & jet0 = (*jetHandle1)[0];
     pat::Jet const & jet1 = (*jetHandle1)[1];
     
     reco::BaseTagInfo const * tagInfo0 = jet0.tagInfo("CATop");
     reco::BaseTagInfo const * tagInfo1 = jet1.tagInfo("CATop");

     reco::CATopJetTagInfo const * caTopTagInfo0 = dynamic_cast<reco::CATopJetTagInfo const *>( tagInfo0 );
     reco::CATopJetTagInfo const * caTopTagInfo1 = dynamic_cast<reco::CATopJetTagInfo const *>( tagInfo1 );

     bool mass0 = caTopTagInfo0->properties().topMass > 140.0 && caTopTagInfo0->properties().topMass < 250.0;
     bool substructure0 = jet0.numberOfDaughters() >= 3 && caTopTagInfo0->properties().minMass > 50.0;
     bool mass1 = caTopTagInfo1->properties().topMass > 140.0 && caTopTagInfo1->properties().topMass < 250.0;
     bool substructure1 = jet1.numberOfDaughters() >= 3 && caTopTagInfo1->properties().minMass > 50.0;


     if ( jet0.pt() > 350 &&
	  mass0 &&
	  jet1.pt() > 350 &&
	  mass1
	  ) {
       
       jetPtType11Den->Fill( jet0.pt() );
       HThistType11->Fill(HTsum );
       pass11 = true;
       ++nTotal11_;
	float invMass = (jet0.p4() + jet1.p4()).mass();     

	MassType11_all->Fill( invMass );
	if (npv >= 15) MassType11_high->Fill( invMass );
        else if (npv < 15) MassType11_low->Fill( invMass);

   
       HThistAll->Fill(HTsum);
       bool passedTrig = false;
       if ( foundHLTTrigObjs.size() != 0 ){
	 double trigPt = foundHLTTrigObjs[0].pt();
       if ( Nbtag >= threshold_ ) {
	   passedTrig = true;
	 }
       }
       if ( passedTrig ) {
	 jetPtType11Num->Fill( jet0.pt() );
	 ++nPassed11_;
       }
     }
   }
   if ( !pass11 ) {

     if ( jetHandle1->size() >= 1 && jetHandle2->size() >= 2 ) {
       /// ...


       pat::Jet const & jet0 = (*jetHandle1)[0];
       pat::Jet const & jet1 = (*jetHandle2)[0];
       pat::Jet const & jet2 = (*jetHandle2)[1];
     
       reco::BaseTagInfo const * tagInfo0 = jet0.tagInfo("CATop");

       reco::CATopJetTagInfo const * caTopTagInfo0 = dynamic_cast<reco::CATopJetTagInfo const *>( tagInfo0 );

       bool mass0 = caTopTagInfo0->properties().topMass > 140.0 && caTopTagInfo0->properties().topMass < 250.0;
       bool substructure0 = jet0.numberOfDaughters() >= 3 && caTopTagInfo0->properties().minMass > 50.0;

       bool mass1 = jet1.mass() > 50.0;
       

       if ( jet0.pt() > 350 &&
	    mass0 &&
	    jet1.pt() > 200 &&
	    mass1 &&
	    jet2.pt() > 30
	    ) {

	 jetPtType12Den->Fill( jet0.pt() );
	HThistType12->Fill(HTsum );
       
        HThistAll->Fill(HTsum);
	 pass12 = true;
	 ++nTotal12_;
       
	float invMass = (jet0.p4() + jet1.p4() + jet2.p4()).mass();
	MassType12_all->Fill( invMass );
	if (npv >= 15) MassType12_high->Fill( invMass );
        else if (npv < 15) MassType12_low->Fill( invMass);
	

	 bool passedTrig = false;
	 if ( foundHLTTrigObjs.size() != 0 ){
	   double trigPt = foundHLTTrigObjs[0].pt();
	   if ( Nbtag >= threshold_ ) {
	     passedTrig = true;
	   }
	 }
	 if ( passedTrig ) {
	   jetPtType12Num->Fill( jet0.pt() );
	   ++nPassed12_;
	 }
       }
     }


   }
}



// ------------ method called once each job just before starting event loop  ------------
void 
BoostedTopTrigger::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BoostedTopTrigger::endJob() 
{
  std::cout << " -------------- Efficiency --------------" << std::endl;
  char buff[1000];
  sprintf(buff, "Type 1 + 1 = %8d / %8d = %6.2f", nPassed11_, nTotal11_, (double)nPassed11_/(double(nTotal11_) ) );
  std::cout << buff << std::endl;
  sprintf(buff, "Type 1 + 2 = %8d / %8d = %6.2f", nPassed12_, nTotal12_, (double)nPassed12_/(double(nTotal12_) ) );
  std::cout << buff << std::endl;
}

// ------------ method called when starting to processes a run  ------------
void 
BoostedTopTrigger::beginRun(edm::Run const& run , edm::EventSetup const& eventSetup)
{

  HLTConfigProvider hltConfig;
    
  bool hltConfigChanged;
  bool hltConfigInitSuccess = hltConfig.init(run, eventSetup, theHltProcessName, hltConfigChanged);

  std::vector<std::string> validTriggerNames;

  if (hltConfigInitSuccess)
    validTriggerNames = hltConfig.triggerNames();

  if (validTriggerNames.size() < 1) {
    cout  << endl << endl << endl
          << "---> WARNING: The HLT Config Provider gave you an empty list of valid trigger names" << endl
          << "Could be a problem with the HLT Process Name (you provided  " << theHltProcessName <<")" << endl
          << "W/o valid triggers we can't produce plots, exiting..."
          << endl << endl << endl;

    int noValidHLTConfig = 0;
    assert (noValidHLTConfig);
    
  }

  vector<string>::const_iterator iDumpName;
  unsigned int numTriggers = 0;
  
  bool foundMatchingTrigger = false;
  
  for ( iDumpName = validTriggerNames.begin();
        iDumpName != validTriggerNames.end();
        iDumpName++) {

    if (verbose_) 
      cout << "Trigger   "  << numTriggers << "   =   "
           << (*iDumpName)
           << endl;
    size_t found;
    found= (*iDumpName).find(targetTrigger);

    if ( found!=string::npos ) {
      foundMatchingTrigger = true;
      targetTrigger = (*iDumpName);
      if (verbose_)
        cout << "---------> Trigger " << (*iDumpName)
             << " matches your TargetTrigger ( "
             << targetTrigger
             << ") " << endl;
    }

        
    ++numTriggers;
  }


  if (!foundMatchingTrigger) {
    cout << "Oops, the trigger you want to study "
         << "is not in this table :( "
         << endl;
    assert (foundMatchingTrigger);
  }


  ////////////////////////////////////////////////////////
  //
  //  Parse the list of modules for your trigger
  //  Look for
  //    1. The L1 Seed 
  //    2. The Final Filter
  /////////////////////////////////////////////////////////

  std::vector<std::string> moduleNames = hltConfig.moduleLabels (targetTrigger);

  string finalFilterString;
  for ( size_t i = 0; i < moduleNames.size(); i++ ) {
    string module = moduleNames[i];

    if ( TString(module).Contains ("Regional") ) {
      finalFilterString = module;
    }
                                  
    
  }

  nameOfHLTFinalFilterModule = InputTag(finalFilterString, "", theHltProcessName);
  //  cout << "Name is" << nameOfHLTFinalFilterModule << endl;  


}

// ------------ method called when ending the processing of a run  ------------
void 
BoostedTopTrigger::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
BoostedTopTrigger::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
BoostedTopTrigger::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BoostedTopTrigger::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void 
BoostedTopTrigger::getAodTriggerObjectsForModule (edm::InputTag collectionTag,
                                                  edm::Handle<trigger::TriggerEvent> aodTriggerEvent,
                                                  trigger::TriggerObjectCollection trigObjs,
                                                  std::vector<TriggerObject> & foundObjects  ) {

   collectionTag = InputTag("hltL1sL1HTT150OrHTT175", "", "HLT"); 


  if (verbose_)
    cout  << "Getting trigger objects for module label = " << collectionTag << endl;
  
  size_t filterIndex   = aodTriggerEvent->filterIndex( collectionTag );
    
  if (verbose_)
    cout << "\n\n filterIndex is "
         << filterIndex;
    
  if ( filterIndex < aodTriggerEvent->sizeFilters() ) {
    const Keys & keys = aodTriggerEvent->filterKeys( filterIndex );

    if (verbose_)
      cout << "\n\nGot keys"
           << "Key size is " << keys.size() << std::endl;;
                              
    // The keys are apparently pointers into the trigger
    // trigObjs collections
    // Use the key's to look up the particles for the
    // filter module that you're using 
      
    for ( size_t j = 0; j < keys.size(); j++ ){
      TriggerObject foundObject = trigObjs[keys[j]];

      // This is the trigger object. Apply your filter to it!
      if (verbose_)
        cout << "Found an objects with pt = "
             << foundObject.pt()
             << ", eta = " << foundObject.eta()
             << endl ;
        

      foundObjects.push_back( foundObject );
     
    }
  }

  
  
}


//define this as a plug-in
DEFINE_FWK_MODULE(BoostedTopTrigger);
