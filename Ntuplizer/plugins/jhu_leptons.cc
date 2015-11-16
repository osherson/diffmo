#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "Analysis/Ntuplizer/plugins/jhu_lepHelper.h"

#include <iostream>

using namespace std;
using namespace LEPDF;

// Class Def:
class jhuLepton : public edm::EDFilter 	
{ // open primary class def
	public:
		explicit jhuLepton(const edm::ParameterSet&);
		~jhuLepton();
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

jhuLepton::jhuLepton(const edm::ParameterSet& iConfig) :
	lepSrc_		(iConfig.getParameter<edm::InputTag>("lepSrc")),
	lepType_	(iConfig.getParameter<std::string>("lepType")),
	lepName_	(iConfig.getParameter<std::string>("lepName")),
	pvSrc_		(iConfig.getParameter<edm::InputTag>("pvSrc")),
	beamSpotSrc_(iConfig.getParameter<edm::InputTag>("beamSpotSrc"))
{
	produces<std::vector<reco::Candidate::PolarLorentzVector> > (lepName_);
	produces<std::vector<unsigned int>>(lepName_+"istight");
	produces<std::vector<unsigned int>>(lepName_+"isloose");
	produces<std::vector<unsigned int>>(lepName_+"ispseudotight");
	produces<std::vector<unsigned int>>(lepName_+"ispseudoloose");
	produces<std::vector<unsigned int>>(lepName_+"modtight");
	produces<std::vector<double>>(lepName_+"iso");
	produces<std::vector<signed int>>(lepName_+"charge");
	//electron only
	bool is_el = (lepType_ == "el" or lepType_ == "ele" or lepType_ == "elec" or lepType_ == "electron" or lepType_ == "electrons");
	if (is_el)
	{
		produces<std::vector<unsigned int>>(lepName_+"isEBEEGap");
		produces<std::vector<unsigned int>>(lepName_+"passConversionVeto");
		produces<std::vector<double>>(lepName_+"TransverseIP");
		produces<std::vector<unsigned int>>(lepName_+"numberOfHits");
		produces<std::vector<double>>(lepName_+"MVA");
	}
}

void jhuLepton::beginJob()
{
	std::cout << "adding lepton... "<<lepName_<<"\n";
}

void jhuLepton::endJob() 
{
}

bool jhuLepton::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	std::auto_ptr<p4_vector> leps( new p4_vector() );
	std::auto_ptr<std::vector<unsigned int> > lepsistight ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<unsigned int> > lepsisloose ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<unsigned int> > lepsispseudotight ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<unsigned int> > lepsispseudoloose ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<unsigned int> > lepmodtight ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<signed int> > lepscharge ( new std::vector<signed int>() );
	std::auto_ptr<std::vector<double>> lepsiso(new std::vector<double>());
	// for MVA based electron ID
	// https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopEGMRun1#Signal
	std::auto_ptr<std::vector<unsigned int>> ele_isEBEEGap(new std::vector<unsigned int>());	
	std::auto_ptr<std::vector<double>> ele_TransverseIP(new std::vector<double>());
	std::auto_ptr<std::vector<unsigned int>> ele_passConversionVeto(new std::vector<unsigned int>());
	std::auto_ptr<std::vector<double>> ele_MVA(new std::vector<double>());
	std::auto_ptr<std::vector<unsigned int>> ele_numberOfHits(new std::vector<unsigned int>());

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
			lepsistight->push_back(LEPDF::lepTight(imuon, hPV));
			lepsisloose->push_back(LEPDF::lepLoose(imuon));
			//not currently useful for muons
			lepsispseudotight->push_back(LEPDF::lepTight(imuon, hPV));
			lepsispseudoloose->push_back(LEPDF::lepLoose(imuon));
			lepmodtight->push_back(LEPDF::lepTight(imuon, hPV));
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
			lepsistight->push_back(LEPDF::lepTight(ielec, hPV, hConversions, hBeamSpot));
			lepsisloose->push_back(LEPDF::lepLoose(ielec, hPV, hConversions, hBeamSpot));
			lepsispseudotight->push_back(LEPDF::lepPseudoTight(ielec, hPV, hConversions, hBeamSpot));
			lepsispseudoloose->push_back(LEPDF::lepPseudoLoose(ielec, hPV, hConversions, hBeamSpot));
			//modified selection for electrons
			lepmodtight->push_back(LEPDF::lepModTight(ielec));

			////////////////////////////////////////////////  
			//       	electron MVA cuts				  //
			////////////////////////////////////////////////
			std::vector<pat::Electron>::const_iterator electron = ielec;
			// EB-EE transition region 1.4442 < fabs(superCluster.eta) < 1.5660
			unsigned int is_EBEEGap = 0;
			double abseta = std::abs(electron->superCluster()->eta());
			if (abseta <= 1.4442 || abseta >= 1.5660) {is_EBEEGap = 0;}
			else {is_EBEEGap = 1;}
                        ele_isEBEEGap->push_back(is_EBEEGap);
			// Transverse IP of the elecrtron (GSF track)
			reco::Vertex vertex_=  *(hPV->begin());	
			ele_TransverseIP->push_back(fabs(electron->gsfTrack()->dxy(vertex_.position())));
			// Conversion rejection 
			ele_passConversionVeto->push_back(electron->passConversionVeto());
			// MVA
			ele_MVA->push_back(electron->electronID("mvaTrigV0"));
			// mHits
			ele_numberOfHits->push_back(electron->gsfTrack()->trackerExpectedHitsInner().numberOfHits());
		}
		iEvent.put( ele_isEBEEGap,lepName_+ "isEBEEGap");
		iEvent.put( ele_TransverseIP,lepName_+ "TransverseIP");
		iEvent.put( ele_passConversionVeto,lepName_+ "passConversionVeto");
		iEvent.put( ele_MVA, lepName_+ "MVA");
		iEvent.put( ele_numberOfHits,lepName_+ "numberOfHits");
	}
	iEvent.put( leps, lepName_);
	iEvent.put( lepsistight, lepName_+"istight");
	iEvent.put( lepsisloose, lepName_+"isloose");
	iEvent.put( lepsispseudotight, lepName_+"ispseudotight");
	iEvent.put( lepsispseudoloose, lepName_+"ispseudoloose");
	iEvent.put( lepmodtight, lepName_+"modtight");
	iEvent.put( lepscharge, lepName_+"charge");
	iEvent.put( lepsiso, lepName_+"iso");
	return true;
}

jhuLepton::~jhuLepton(){}

//define this as a plug-in
DEFINE_FWK_MODULE(jhuLepton);
