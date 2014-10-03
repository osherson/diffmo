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
#include "DataFormats/VertexReco/interface/Vertex.h"

//#include "Analysis/SHyFT/interface/SHyFTPFSelector.h"

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
		edm::InputTag triggerSrc_;
		bool isData_;
		bool readTriggers_;
		std::vector<std::string> trigs_;
}; // close primary class def

DiFfMoGeneral::DiFfMoGeneral(const edm::ParameterSet& iConfig) :
	pvSrc_ 		(iConfig.getParameter<edm::InputTag>("pvSrc")),
	metSrc_ 	(iConfig.getParameter<edm::InputTag>("metSrc")),
	triggerSrc_(iConfig.getParameter<edm::InputTag>("triggerSrc")),
	isData_    (iConfig.getParameter<bool>("isData")),
	readTriggers_(iConfig.getParameter<bool>("readTriggers")),
	trigs_       (iConfig.getParameter<std::vector<std::string> > ("triggers"))
{
	produces<unsigned int>    			("npv");
	produces<double>    				("metpt");
	produces<double>    				("metphi");
	produces<std::vector<int> >			("prescales");
	produces<std::vector<int> >			("trigIndex");
	produces<std::vector<int> >			("myTrigIndex");
	produces<std::vector<std::string> > ("trigNames");
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
	std::auto_ptr<std::vector<int> > prescales ( new std::vector<int>() );
	std::auto_ptr<std::vector<int> > trigIndex ( new std::vector<int>() );
	std::auto_ptr<std::vector<int> > myTrigIndex ( new std::vector<int>() );
	std::auto_ptr<std::vector<std::string> > trigNames ( new std::vector<std::string>() );

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

	// For real data, get trigger path
	if(isData_)	{
		if(readTriggers_) {
			//edm::Handle<trigger::TriggerEvent> h_trig;
			edm::Handle<pat::TriggerEvent> h_trig;
			iEvent.getByLabel( triggerSrc_, h_trig );
			for ( std::vector<std::string>::const_iterator itrigBegin = trigs_.begin(),
			itrigEnd = trigs_.end(), itrig = itrigBegin;  itrig != itrigEnd; ++itrig ) {
				if ( h_trig->wasRun() && h_trig->wasAccept() && h_trig->paths() > 0 ) {
					int indexPath = h_trig->indexPath( *itrig );
					if ( indexPath > 0 ) {
						pat::TriggerPath const * path = h_trig->path( *itrig );
						if ( path != 0 && path->wasRun() && path->wasAccept() ) {
							trigIndex->push_back( path->index() );
							myTrigIndex->push_back( static_cast<int>(itrig - itrigBegin) );
							prescales->push_back( path->prescale() );
							trigNames->push_back( path->name() );
						}
					}
				}
			iEvent.put(prescales     ,"prescales");
			iEvent.put(trigIndex     ,"trigIndex");
			iEvent.put(myTrigIndex   ,"myTrigIndex");
			iEvent.put(trigNames     ,"trigNames");
			}
		}
	}
	return true;
}

void DiFfMoGeneral::endJob() 
{
}


DiFfMoGeneral::~DiFfMoGeneral(){}

//define this as a plug-in
DEFINE_FWK_MODULE(DiFfMoGeneral);
