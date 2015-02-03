// leptonic actions

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "Analysis/Ntuplizer/plugins/diffmo_lepHelper.h"

#include <iostream>

using namespace std;
using namespace LEPDF;

// Class Def:
class DiFfMoLepton : public edm::EDFilter 	
{ // open primary class def
	public:
		explicit DiFfMoLepton(const edm::ParameterSet&);
		~DiFfMoLepton();
	private:
		virtual void beginJob() ;
		virtual bool filter(edm::Event&, const edm::EventSetup&); // this is essentially the Ntuplizer code
		virtual void endJob() ;
		// member data
		edm::InputTag 	lepSrc_;
		std::string		lepType_;
		std::string		lepName_;
		edm::InputTag	pvSrc_;
		edm::InputTag	beamSpotSrc_;
}; // close primary class def

DiFfMoLepton::DiFfMoLepton(const edm::ParameterSet& iConfig) :
	lepSrc_		(iConfig.getParameter<edm::InputTag>("lepSrc")),
	lepType_	(iConfig.getParameter<std::string>("lepType")),
	lepName_	(iConfig.getParameter<std::string>("lepName")),
	pvSrc_		(iConfig.getParameter<edm::InputTag>("pvSrc")),
	beamSpotSrc_(iConfig.getParameter<edm::InputTag>("beamSpotSrc"))
{
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (lepName_);
	produces<std::vector<unsigned int>>(lepName_+"istight");
	produces<std::vector<unsigned int>>(lepName_+"isloose");
	produces<std::vector<unsigned int>>(lepName_+"modtight");
	produces<std::vector<double>>(lepName_+"iso");
	produces<std::vector<signed int>>(lepName_+"charge");
}

void DiFfMoLepton::beginJob()
{
	std::cout << "adding lepton... "<<lepName_<<"\n";
}

void DiFfMoLepton::endJob() 
{
}

bool DiFfMoLepton::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	std::auto_ptr<p4_vector> leps( new p4_vector() );
	std::auto_ptr<std::vector<unsigned int> > lepsistight ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<unsigned int> > lepsisloose ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<unsigned int> > lepmodtight ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<signed int> > lepscharge ( new std::vector<signed int>() );
	std::auto_ptr<std::vector<double>> lepsiso(new std::vector<double>());

	bool is_el = (lepType_ == "el" or lepType_ == "ele" or lepType_ == "elec" or lepType_ == "electron" or lepType_ == "electrons");
	bool is_mu = (lepType_ == "mu" or lepType_ == "muon" or lepType_ == "muons");

	edm::Handle< std::vector<reco::Vertex> > hPV;
	iEvent.getByLabel( pvSrc_, hPV );

	if (is_mu)
	{
		edm::Handle<std::vector<pat::Muon> > lepHandle;
		iEvent.getByLabel (lepSrc_, lepHandle);
		for ( std::vector<pat::Muon>::const_iterator muonBegin = lepHandle->begin(), muonEnd = lepHandle->end(), imuon = muonBegin; imuon != muonEnd; ++imuon )
		{
			lepsiso->push_back(LEPDF::lepPFIso(imuon));
			lepscharge->push_back(imuon->charge());
			reco::Candidate::PolarLorentzVector lep_nocuts (imuon->pt(), imuon->eta(), imuon->phi(), imuon->mass());
			leps->push_back(lep_nocuts);
			lepsistight->push_back(LEPDF::lepTight(imuon));
			lepsisloose->push_back(LEPDF::lepLoose(imuon));
			//not currently useful for muons
			lepmodtight->push_back(LEPDF::lepTight(imuon));
		}
	}
	if (is_el)
	{
		edm::Handle<std::vector<pat::Electron> > lepHandle;
		iEvent.getByLabel (lepSrc_, lepHandle);

		edm::Handle<reco::ConversionCollection> hConversions;
		iEvent.getByLabel("allConversions", hConversions);

		edm::Handle<reco::BeamSpot> hBeamSpot;
		iEvent.getByLabel(beamSpotSrc_, hBeamSpot);

		for ( std::vector<pat::Electron>::const_iterator elecBegin = lepHandle->begin(), elecEnd = lepHandle->end(), ielec = elecBegin; ielec != elecEnd; ++ielec )
		{
			lepsiso->push_back(LEPDF::lepPFIso(ielec));
			lepscharge->push_back(ielec->charge());
			reco::Candidate::PolarLorentzVector lep_nocuts (ielec->pt(), ielec->eta(), ielec->phi(), ielec->mass());
			leps->push_back(lep_nocuts);
			lepsistight->push_back(LEPDF::lepTight(ielec));
			lepsisloose->push_back(LEPDF::lepLoose(ielec, hPV, hConversions, hBeamSpot));
			//modified selection for electrons
			lepmodtight->push_back(LEPDF::lepModTight(ielec));
		}
	}
	iEvent.put( leps, lepName_);
	iEvent.put( lepsistight, lepName_+"istight");
	iEvent.put( lepsisloose, lepName_+"isloose");
	iEvent.put( lepmodtight, lepName_+"modtight");
	iEvent.put( lepscharge, lepName_+"charge");
	iEvent.put( lepsiso, lepName_+"iso");
	return true;
}

DiFfMoLepton::~DiFfMoLepton(){}

//define this as a plug-in
DEFINE_FWK_MODULE(DiFfMoLepton);
