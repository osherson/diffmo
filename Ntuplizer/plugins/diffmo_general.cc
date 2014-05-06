#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include "DataFormats/PatCandidates/interface/MET.h"

#include "Analysis/SHyFT/interface/SHyFTPFSelector.h"

#include <iostream>
using namespace std;


// Class Def:
class DiFfMoGeneral : public edm::EDFilter 	
{ // open primary class def
   public:
      explicit DiFfMoGeneral(const edm::ParameterSet&);
     ~DiFfMoGeneral();
   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&); // this is essentially the Ntuplizer code
      virtual void endJob() ;
	// member data
	edm::InputTag	pvSrc_;
	edm::InputTag	metSrc_;
}; // close primary class def

DiFfMoGeneral::DiFfMoGeneral(const edm::ParameterSet& iConfig) :
	pvSrc_ 		(iConfig.getParameter<edm::InputTag>("pvSrc")),
	metSrc_ 	(iConfig.getParameter<edm::InputTag>("metSrc"))
{
	produces<unsigned int>    ("npv");
	produces<double>    ("metpt");
	produces<double>    ("metphi");
}

void DiFfMoGeneral::beginJob()
{
	std::cout << "adding general event info\n";
}

bool DiFfMoGeneral::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) //Make cuts/adjustments/corrections:
{
	// handles and labels start here :
  	edm::Handle< std::vector<reco::Vertex> > h_pv;
  	iEvent.getByLabel( pvSrc_, h_pv );
	edm::Handle<std::vector<pat::MET> > h_met;
	iEvent.getByLabel( metSrc_, h_met );

  	std::auto_ptr<unsigned int> npv( new unsigned int() );
  	std::auto_ptr<double> metpt( new double() );
  	std::auto_ptr<double> metphi( new double() );

	// Number of reconstructed PV's
  	*npv = h_pv->size();
	// MET:
	for ( std::vector<pat::MET>::const_iterator metBegin = h_met->begin(),
          metEnd = h_met->end(), imet = metBegin; imet != metEnd; ++imet )
  	{
   		*metpt = imet->pt();
		*metphi = imet->phi();

  	}

  	iEvent.put( npv, "npv");
  	iEvent.put( metpt, "metpt");
  	iEvent.put( metphi, "metphi");
	return true;
}


void DiFfMoGeneral::endJob() 
{
}


DiFfMoGeneral::~DiFfMoGeneral(){}

//define this as a plug-in
DEFINE_FWK_MODULE(DiFfMoGeneral);
