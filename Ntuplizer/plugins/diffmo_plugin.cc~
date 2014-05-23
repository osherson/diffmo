/*#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFMuonSelector.h"
#include "Analysis/BoostedTopAnalysis/interface/SubjetHelper.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "Analysis/BoostedTopAnalysis/interface/CATopTagFunctor.h"
#include "Analysis/BoostedTopAnalysis/interface/BoostedTopWTagFunctor.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include "fastjet/tools/Filter.hh"
#include <fastjet/ClusterSequence.hh>
//#include <fastjet/ActiveAreaSpec.hh>
#include <fastjet/ClusterSequenceArea.hh>
#include "Nsubjettiness.hh"
#include "Njettiness.hh"

#include "Analysis/SHyFT/interface/SHyFTPFSelector.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"

#include <iostream>


using namespace fastjet;
using namespace std;

// Les Houche Accord PDF
namespace LHAPDF{
	void initPDFSet(int nset, const std::string& filename, int member = 0);
	int numberPDF(int nset);
	void usePDFMember(int nset, int member);
      	double xfx(int nset, double x, double Q, int fl);
      	double getXmin(int nset, int member);
      	double getXmax(int nset, int member);
      	double getQ2min(int nset, int member);
      	double getQ2max(int nset, int member);
      	void extrapolate(bool extrapolate=true);
		}

// Class Def:
class HEPtuplizer : public edm::EDFilter 	
{ // open primary class def
   public:
      explicit HEPtuplizer(const edm::ParameterSet&);
     ~HEPtuplizer();
   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&); // this is essentially the Ntuplizer code
      virtual void endJob() ;
	// member data
	edm::InputTag 			pvSrc_;
	edm::InputTag 			ca8Src_;
	edm::InputTag 			ca8prunedSrc_;
	edm::InputTag 			ca8toptagSrc_;
	edm::InputTag 			ak5Src_;
	edm::InputTag 			muonSrc_;
	edm::InputTag 			electronSrc_;  
}; // close primary class def

HEPtuplizer::HEPtuplizer(const edm::ParameterSet& iConfig) :
	pvSrc_ 		(iConfig.getParameter<edm::InputTag>("pvSrc")),
	ak5Src_ 	(iConfig.getParameter<edm::InputTag>("ak5Src")),
	ca8Src_ 	(iConfig.getParameter<edm::InputTag>("ca8Src")),
	ca8prunedSrc_ 	(iConfig.getParameter<edm::InputTag>("ca8prunedSrc")),
	ca8toptagSrc_ 	(iConfig.getParameter<edm::InputTag>("ca8toptagSrc")),
	muonSrc_  	(iConfig.getParameter<edm::InputTag>("muonSrc")),
	electronSrc_  	(iConfig.getParameter<edm::InputTag>("electronSrc"))
	{



	// Get the factorized jet corrector. 
	// The payloads contain N elements, 
	// the Nth is the uncertainty, and the first N-1 elements are the
  	// actual correction levels. 

	// List what branches we'll fill:
	// event info:
	produces<unsigned int>    ("npv");
	// lepton info:
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("muons"); // muon
	produces<std::vector<unsigned int>>("muonsistight");
	produces<std::vector<double>>("muonsiso");
	produces<std::vector<signed int>>("muonscharge");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("electrons"); // electron
	produces<std::vector<unsigned int>>("electronsistight");
	produces<std::vector<double>>("electronsiso");
	produces<std::vector<signed int>>("electronscharge");
	// jet collections:
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("CA8jets"); // basic ca8
	produces<std::vector<double>>("CA8jetsCSV");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("prunedCA8jets"); // pruned ca8 (formerly WtagP4)
  	produces<std::vector<unsigned int>> ("prunedCA8nsub");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("prunedCA8sub0");
	produces<std::vector<double>> ("prunedCA8sub0CSV");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("prunedCA8sub1");
	produces<std::vector<double>> ("prunedCA8sub1CSV");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("prunedCA8sub2");
	produces<std::vector<double>> ("prunedCA8sub2CSV");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("prunedCA8sub3");
	produces<std::vector<double>> ("prunedCA8sub3CSV");
	produces<std::vector<double>> ("prunedCA8jetsCSV");
  	produces<std::vector<double>> ("CA8tau1");
  	produces<std::vector<double>> ("CA8tau2");
  	produces<std::vector<double>> ("CA8tau3");
  	produces<std::vector<double>> ("CA8tau4");
	produces<std::vector<double>> ("PCA8tau1");
  	produces<std::vector<double>> ("PCA8tau2");
  	produces<std::vector<double>> ("PCA8tau3");
  	produces<std::vector<double>> ("PCA8tau4");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("Topjets"); // topTag
	produces<std::vector<unsigned int>> ("Topnsub");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("Tsub0");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("Tsub1");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("Tsub2");
	produces<std::vector<reco::Candidate::PolarLorentzVector> > ("Tsub3");
	produces<std::vector<double>> ("TjetsCSV");
	produces<std::vector<double>> ("Tsub0CSV");
	produces<std::vector<double>> ("Tsub1CSV");
	produces<std::vector<double>> ("Tsub2CSV");
	produces<std::vector<double>> ("Tsub3CSV");  	
	produces<std::vector<double>> ("Ttau1");
  	produces<std::vector<double>> ("Ttau2");
  	produces<std::vector<double>> ("Ttau3");
  	produces<std::vector<double>> ("Ttau4");
	}

void HEPtuplizer::beginJob()
	{
	std::cout << "begin job\n";
	}

bool HEPtuplizer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) //Make cuts/adjustments/corrections:
	{
	// shorthand
  	typedef std::vector<reco::Candidate::PolarLorentzVector> p4_vector;

	// handles and labels start here :
  	edm::Handle< std::vector<reco::Vertex> > h_pv;
  	iEvent.getByLabel( pvSrc_, h_pv );

  	edm::Handle<std::vector<pat::Jet> > h_ak5Jets;
	iEvent.getByLabel( ak5Src_, h_ak5Jets);
  	edm::Handle<std::vector<pat::Jet> > h_ca8Jets;
	iEvent.getByLabel( ca8Src_, h_ca8Jets);
  	edm::Handle<std::vector<pat::Jet> > h_Pca8Jets;
	iEvent.getByLabel( ca8prunedSrc_, h_Pca8Jets);
 	edm::Handle<std::vector<pat::Jet> > h_topTag;
	iEvent.getByLabel( ca8toptagSrc_, h_topTag);
    	edm::Handle<std::vector<pat::Muon> > muonHandle;
    	iEvent.getByLabel (muonSrc_, muonHandle);
    	edm::Handle<std::vector<pat::Electron > > electronHandle;
    	iEvent.getByLabel (electronSrc_, electronHandle);
	// handles and labels end here!
	// event
  	std::auto_ptr<unsigned int> npv( new unsigned int() );
	// lepton
        std::auto_ptr<p4_vector> muons( new p4_vector() );
	std::auto_ptr<std::vector<unsigned int> > muonsistight ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<double>> muonsiso(new std::vector<double>());
	std::auto_ptr<std::vector<signed int> > muonscharge ( new std::vector<signed int>() );
        std::auto_ptr<p4_vector> electrons( new p4_vector() );
	std::auto_ptr<std::vector<unsigned int> > electronsistight ( new std::vector<unsigned int>() );
	std::auto_ptr<std::vector<signed int> > electronscharge ( new std::vector<signed int>() );
	std::auto_ptr<std::vector<double>> electronsiso(new std::vector<double>());
	// jets  	
	std::auto_ptr<p4_vector> AK5jets( new p4_vector() );
	std::auto_ptr<std::vector<double>> AK5jetsCSV(new std::vector<double>());  
  	std::auto_ptr<p4_vector> CA8jets( new p4_vector() );
	std::auto_ptr<std::vector<double>> CA8jetsCSV(new std::vector<double>());  
  	std::auto_ptr<p4_vector> prunedCA8jets( new p4_vector() );
	std::auto_ptr<std::vector<double>> prunedCA8jetsCSV(new std::vector<double>());
	std::auto_ptr<std::vector<double>> prunedCA8sub0CSV(new std::vector<double>());
	std::auto_ptr<std::vector<double>> prunedCA8sub1CSV(new std::vector<double>());
	std::auto_ptr<std::vector<double>> prunedCA8sub2CSV(new std::vector<double>());
	std::auto_ptr<std::vector<double>> prunedCA8sub3CSV(new std::vector<double>());
	std::auto_ptr<std::vector<unsigned int> > prunedCA8nsub ( new std::vector<unsigned int>() );
	std::auto_ptr<p4_vector> prunedCA8sub0(new p4_vector());
	std::auto_ptr<p4_vector> prunedCA8sub1(new p4_vector());
	std::auto_ptr<p4_vector> prunedCA8sub2(new p4_vector());
	std::auto_ptr<p4_vector> prunedCA8sub3(new p4_vector());
  	std::auto_ptr<std::vector<double> > CA8tau1 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > CA8tau2 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > CA8tau3 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > CA8tau4 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > PCA8tau1 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > PCA8tau2 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > PCA8tau3 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > PCA8tau4 ( new std::vector<double>() );
  	std::auto_ptr<p4_vector> Topjets( new p4_vector() );
	std::auto_ptr<std::vector<unsigned int> > Topnsub ( new std::vector<unsigned int>() );
	std::auto_ptr<p4_vector> Tsub0(new p4_vector());
	std::auto_ptr<p4_vector> Tsub1(new p4_vector());
	std::auto_ptr<p4_vector> Tsub2(new p4_vector());
	std::auto_ptr<p4_vector> Tsub3(new p4_vector());
  	std::auto_ptr<std::vector<double> > Ttau1 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > Ttau2 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > Ttau3 ( new std::vector<double>() );
  	std::auto_ptr<std::vector<double> > Ttau4 ( new std::vector<double>() );
	std::auto_ptr<std::vector<double>> TjetsCSV(new std::vector<double>());
	std::auto_ptr<std::vector<double>> Tsub0CSV(new std::vector<double>());
	std::auto_ptr<std::vector<double>> Tsub1CSV(new std::vector<double>());
	std::auto_ptr<std::vector<double>> Tsub2CSV(new std::vector<double>());
	std::auto_ptr<std::vector<double>> Tsub3CSV(new std::vector<double>());

	// Number of reconstructed PV's
  	*npv = h_pv->size();
	// MET:
	
	// Leptons:
	std::vector<reco::Candidate::PolarLorentzVector> looselep;
	unsigned int num_looselep = 0;
	// muons	
	for ( std::vector<pat::Muon>::const_iterator muonBegin = muonHandle->begin(), muonEnd = muonHandle->end(), imuon = muonBegin; imuon != muonEnd; ++imuon )
	{
		double chIso = imuon->userIsolation(pat::PfChargedHadronIso);
	  	double nhIso = imuon->userIsolation(pat::PfNeutralHadronIso);
	  	double gIso  = imuon->userIsolation(pat::PfGammaIso);
	  	double pt    = imuon->pt() ;
	  	double pfIso = (chIso + nhIso + gIso) / pt;
		muonsiso->push_back(pfIso);
		signed int charge = imuon->charge();
		muonscharge->push_back(charge);
		reco::Candidate::PolarLorentzVector muon_nocuts (pt, imuon->eta(), imuon->phi(), imuon->mass());
		muons->push_back(muon_nocuts);
		unsigned int is_tight_muon = 0;
		if (imuon->isGlobalMuon()) 
			{
			if (imuon->isTrackerMuon()) 
				{
				if (static_cast<int>( imuon->numberOfValidHits()) > 10) 
					{
					if (imuon->dB() < 0.2) 
						{
						if (imuon->normChi2() < 10) 
							{
							if (imuon->numberOfMatchedStations() > 0) 
								{
								if (imuon->track()->hitPattern().numberOfValidPixelHits() > 0) 
									{
									is_tight_muon = 1;
									} 
								}
							}
						}
					}
				}
			}

		muonsistight->push_back(is_tight_muon);
		if (is_tight_muon == 0) 
			{
			num_looselep++;
			looselep.push_back(muon_nocuts);
			}	
	}

	// electrons
    	for ( std::vector<pat::Electron>::const_iterator electronBegin = electronHandle->begin(), electronEnd = electronHandle->end(), ielectron = electronBegin; ielectron != electronEnd; ++ielectron ) 
	{
		double chIso = ielectron->userIsolation(pat::PfChargedHadronIso);
	  	double nhIso = ielectron->userIsolation(pat::PfNeutralHadronIso);
	  	double gIso  = ielectron->userIsolation(pat::PfGammaIso);
	  	double pt    = ielectron->pt() ;
	  	double pfIso = (chIso + nhIso + gIso) / pt;
		electronsiso->push_back(pfIso);
		signed int charge = ielectron->charge();
		electronscharge->push_back(charge);
		reco::Candidate::PolarLorentzVector electron_nocuts (pt, ielectron->eta(), ielectron->phi(), ielectron->mass());
		electrons->push_back(electron_nocuts);
		unsigned int is_tight_ele = 0;
		if (ielectron->dB() < 0.2) 
			{
			if (not ielectron->gsfTrack()->trackerExpectedHitsInner().numberOfHits() < 1) 
				{
				is_tight_ele = 1;
				}
			}
		electronsistight->push_back(is_tight_ele);
		if (is_tight_ele == 0) 
			{
			num_looselep++;
			looselep.push_back(electron_nocuts);
			}
	}



		// -----  Jet Collections
	// Multi-obj Quantities
	std::vector<double> tau1quant;
	std::vector<double> tau2quant;
	std::vector<double> tau3quant;
	std::vector<double> tau4quant;
	// - CA8
	for ( std::vector<pat::Jet>::const_iterator jetBegin = h_ca8Jets->begin(), jetEnd = h_ca8Jets->end(), ijet = jetBegin; ijet != jetEnd; ++ijet )
	{
    	  	reco::Candidate::LorentzVector uncorrCA8jet = ijet->correctedP4(1);
          	reco::Candidate::PolarLorentzVector corrCA8jet (uncorrCA8jet.pt(), uncorrCA8jet.eta(), uncorrCA8jet.phi(), uncorrCA8jet.mass());
		int matched = 0;
		for ( std::vector<pat::Jet>::const_iterator jjetBegin = h_Pca8Jets->begin(), jjetEnd = h_Pca8Jets->end(), jjet = jjetBegin; jjet != jjetEnd; ++jjet )
		{ // we're matching to the pruned jets (which have b-tag info) to put in this b-tag.
			reco::Candidate::LorentzVector matchingujet = jjet->correctedP4(1);
          		reco::Candidate::PolarLorentzVector matchingjet (matchingujet.pt(), matchingujet.eta(), matchingujet.phi(), matchingujet.mass());
			double deltaR = reco::deltaR<double>( matchingjet.eta(), matchingjet.phi(), corrCA8jet.eta(), corrCA8jet.phi() );
			if (deltaR < 0.4)
			{
				matched = 1;
				CA8jetsCSV->push_back(jjet->bDiscriminator("combinedSecondaryVertexBJetTags"));
			}
		}
		if (matched == 0)
			{
			CA8jetsCSV->push_back(-100.0);
			}
		// tau vars
		Nsubjettiness tau1(1, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau2(2, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau3(3, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau4(4, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		// make fastjets out of daughters:
		std::vector<fastjet::PseudoJet> FJparticles;
		for (unsigned int k = 0; k < ijet->numberOfDaughters(); k++) 
			{
	        	const reco::PFCandidate* this_constituent = dynamic_cast<const reco::PFCandidate*>(ijet->daughter(k));
			
			FJparticles.push_back( fastjet::PseudoJet( this_constituent->px(), this_constituent->py(), this_constituent->pz(), this_constituent->energy() ) );
			}
		fastjet::PseudoJet combJet = fastjet::join(FJparticles);
		double T1 = tau1.result(combJet);
		double T2 = tau2.result(combJet);
		double T3 = tau3.result(combJet);
		double T4 = tau4.result(combJet);
		tau1quant.push_back(T1);
		tau2quant.push_back(T2);
		tau3quant.push_back(T3);
		tau4quant.push_back(T4);
		CA8tau1->push_back(T1);
    		CA8tau2->push_back(T2);
    		CA8tau3->push_back(T3);
    		CA8tau4->push_back(T4);		
		if (num_looselep > 0)
			{
			for (std::vector<reco::Candidate::PolarLorentzVector>::iterator it = looselep.begin() ; it != looselep.end(); ++it)
				{
				
				double dr_lepjet = reco::deltaR<double>( corrCA8jet.eta(), corrCA8jet.phi(), it->eta(), it->phi() );
				if (dr_lepjet < 0.4)
					{
					reco::Candidate::PolarLorentzVector lep (it->pt(), it->eta(), it->phi(), it->mass());
					corrCA8jet = corrCA8jet - lep;
					}
				}
			}
		CA8jets->push_back(corrCA8jet);	
		
		
	}
	// Prunced CA8 (previously Wtag)
	for ( std::vector<pat::Jet>::const_iterator jetBegin = h_Pca8Jets->begin(), jetEnd = h_Pca8Jets->end(), ijet = jetBegin; ijet != jetEnd; ++ijet )
	{
    	  	reco::Candidate::LorentzVector uncorrPCA8jet = ijet->correctedP4(1);
          	reco::Candidate::PolarLorentzVector corrPCA8jet (uncorrPCA8jet.pt(), uncorrPCA8jet.eta(), uncorrPCA8jet.phi(), uncorrPCA8jet.mass());

		prunedCA8jetsCSV->push_back(ijet->bDiscriminator("combinedSecondaryVertexBJetTags"));
		// taus and nsubjettiness:		
		unsigned int nsub = ijet->numberOfDaughters();
		prunedCA8nsub->push_back(nsub);
		// subjet time:
		int startnull = 0;
		reco::Candidate::PolarLorentzVector blank (0.0, 0.0, 0.0, -100.0);
		if (nsub > 0) 
			{
			startnull++;
			const pat::Jet* sub0_base = dynamic_cast<const pat::Jet*>(ijet->daughter(0));
			reco::Candidate::LorentzVector sub0_mid = sub0_base->correctedP4(0);
			reco::Candidate::PolarLorentzVector sub0 (sub0_mid.pt(), sub0_mid.eta(), sub0_mid.phi(), sub0_mid.mass());
			double bd0 = sub0_base->bDiscriminator("combinedSecondaryVertexBJetTags");
			prunedCA8sub0->push_back(sub0);	
			prunedCA8sub0CSV->push_back(bd0);		
			}
		if (nsub > 1) 
			{
			startnull++;
			const pat::Jet* sub1_base = dynamic_cast<const pat::Jet*>(ijet->daughter(1));
			reco::Candidate::LorentzVector sub1_mid = sub1_base->correctedP4(0);
			reco::Candidate::PolarLorentzVector sub1 (sub1_mid.pt(), sub1_mid.eta(), sub1_mid.phi(), sub1_mid.mass());
			double bd1 = sub1_base->bDiscriminator("combinedSecondaryVertexBJetTags");
			prunedCA8sub1->push_back(sub1);	
			prunedCA8sub1CSV->push_back(bd1);		
			}
		if (nsub > 2) 
			{
			startnull++;
			const pat::Jet* sub2_base = dynamic_cast<const pat::Jet*>(ijet->daughter(2));
			reco::Candidate::LorentzVector sub2_mid = sub2_base->correctedP4(0);
			reco::Candidate::PolarLorentzVector sub2 (sub2_mid.pt(), sub2_mid.eta(), sub2_mid.phi(), sub2_mid.mass());
			double bd2 = sub2_base->bDiscriminator("combinedSecondaryVertexBJetTags");
			prunedCA8sub2->push_back(sub2);	
			prunedCA8sub2CSV->push_back(bd2);		
			}
		if (nsub > 3) 
			{
			startnull++;
			const pat::Jet* sub3_base = dynamic_cast<const pat::Jet*>(ijet->daughter(3));
			reco::Candidate::LorentzVector sub3_mid = sub3_base->correctedP4(0);
			reco::Candidate::PolarLorentzVector sub3 (sub3_mid.pt(), sub3_mid.eta(), sub3_mid.phi(), sub3_mid.mass());
			double bd3 = sub3_base->bDiscriminator("combinedSecondaryVertexBJetTags");
			prunedCA8sub3->push_back(sub3);
			prunedCA8sub3CSV->push_back(bd3);			
			}
		if (startnull == 0)
			{
			prunedCA8sub0->push_back(blank);
			prunedCA8sub1->push_back(blank);	
			prunedCA8sub2->push_back(blank);	
			prunedCA8sub3->push_back(blank);
			prunedCA8sub0CSV->push_back(-100.0);
			prunedCA8sub1CSV->push_back(-100.0);
			prunedCA8sub2CSV->push_back(-100.0);
			prunedCA8sub3CSV->push_back(-100.0);	
			}
		if (startnull == 1)
			{
			prunedCA8sub1->push_back(blank);	
			prunedCA8sub2->push_back(blank);	
			prunedCA8sub3->push_back(blank);
			prunedCA8sub1CSV->push_back(-100.0);
			prunedCA8sub2CSV->push_back(-100.0);
			prunedCA8sub3CSV->push_back(-100.0);	
			}
		if (startnull == 2)
			{
			prunedCA8sub2->push_back(blank);	
			prunedCA8sub3->push_back(blank);
			prunedCA8sub2CSV->push_back(-100.0);
			prunedCA8sub3CSV->push_back(-100.0);	
			}
		if (startnull == 3)
			{
			prunedCA8sub3->push_back(blank);
			prunedCA8sub3CSV->push_back(-100.0);	
			}
		//assign tau vars
		Nsubjettiness tau1(1, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau2(2, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau3(3, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau4(4, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		// make fastjets out of daughters:
		std::vector<fastjet::PseudoJet> P_FJparticles;
		for (unsigned int j = 0; j < nsub; j++) 
			{
	        	pat::Jet const * new_constituent = dynamic_cast<pat::Jet const *>(ijet->daughter(j));
			if (new_constituent->energy() > 0.0 && new_constituent->px() > 0.0 && new_constituent->py() > 0.0 && new_constituent->pz() > 0.0)
				{
				P_FJparticles.push_back( fastjet::PseudoJet( new_constituent->px(), new_constituent->py(), new_constituent->pz(), new_constituent->energy() ) );
				}
			}
		fastjet::PseudoJet PcombJet = fastjet::join(P_FJparticles);
		PCA8tau1->push_back(tau1.result(PcombJet));
    		PCA8tau2->push_back(tau2.result(PcombJet));
    		PCA8tau3->push_back(tau3.result(PcombJet));
    		PCA8tau4->push_back(tau4.result(PcombJet));
		if (num_looselep > 0)
			{
			for (std::vector<reco::Candidate::PolarLorentzVector>::const_iterator it = looselep.begin() ; it != looselep.end(); ++it)
				{
				double dr_lepjet = reco::deltaR<double>( corrPCA8jet.eta(), corrPCA8jet.phi(), it->eta(), it->phi() );
				if (dr_lepjet < 0.4)
					{
					reco::Candidate::PolarLorentzVector lep (it->pt(), it->eta(), it->phi(), it->mass());
					corrPCA8jet = corrPCA8jet - lep;
					}
				}
			}
		prunedCA8jets->push_back(corrPCA8jet);

	}

	// TopTagged
	for ( std::vector<pat::Jet>::const_iterator jetBegin = h_topTag->begin(), jetEnd = h_topTag->end(), ijet = jetBegin; ijet != jetEnd; ++ijet ) 
		{
    		reco::Candidate::LorentzVector uncorrJet = ijet->correctedP4(0);

		reco::Candidate::PolarLorentzVector corrJet (uncorrJet.pt(), uncorrJet.eta(), uncorrJet.phi(), uncorrJet.mass());
		TjetsCSV->push_back(ijet->bDiscriminator("combinedSecondaryVertexBJetTags"));
		unsigned int Tnsub = ijet->numberOfDaughters();
		Topnsub->push_back(Tnsub);
		// subjet time:
		int startnull = 0;
		reco::Candidate::PolarLorentzVector blank (0.0, 0.0, 0.0, -100.0);
		if (Tnsub > 0) 
			{
			startnull++;
			const pat::Jet* sub0_base = dynamic_cast<const pat::Jet*>(ijet->daughter(0));
			reco::Candidate::LorentzVector sub0_mid = sub0_base->correctedP4(0);
			reco::Candidate::PolarLorentzVector sub0 (sub0_mid.pt(), sub0_mid.eta(), sub0_mid.phi(), sub0_mid.mass());
			double bd0 = sub0_base->bDiscriminator("combinedSecondaryVertexBJetTags");
			Tsub0->push_back(sub0);	
			Tsub0CSV->push_back(bd0);		
			}
		if (Tnsub > 1)
			{
			startnull++;
			const pat::Jet* sub1_base = dynamic_cast<const pat::Jet*>(ijet->daughter(1));
			reco::Candidate::LorentzVector sub1_mid = sub1_base->correctedP4(0);
			reco::Candidate::PolarLorentzVector sub1 (sub1_mid.pt(), sub1_mid.eta(), sub1_mid.phi(), sub1_mid.mass());
			double bd1 = sub1_base->bDiscriminator("combinedSecondaryVertexBJetTags");
			Tsub1->push_back(sub1);	
			Tsub1CSV->push_back(bd1);		
			}
		if (Tnsub > 2) 
			{
			startnull++;
			const pat::Jet* sub2_base = dynamic_cast<const pat::Jet*>(ijet->daughter(2));
			reco::Candidate::LorentzVector sub2_mid = sub2_base->correctedP4(0);
			reco::Candidate::PolarLorentzVector sub2 (sub2_mid.pt(), sub2_mid.eta(), sub2_mid.phi(), sub2_mid.mass());
			double bd2 = sub2_base->bDiscriminator("combinedSecondaryVertexBJetTags");
			Tsub2->push_back(sub2);	
			Tsub2CSV->push_back(bd2);		
			}
		if (Tnsub > 3) 
			{
			startnull++;
			const pat::Jet* sub3_base = dynamic_cast<const pat::Jet*>(ijet->daughter(3));
			reco::Candidate::LorentzVector sub3_mid = sub3_base->correctedP4(0);
			reco::Candidate::PolarLorentzVector sub3 (sub3_mid.pt(), sub3_mid.eta(), sub3_mid.phi(), sub3_mid.mass());
			double bd3 = sub3_base->bDiscriminator("combinedSecondaryVertexBJetTags");
			Tsub3->push_back(sub3);
			Tsub3CSV->push_back(bd3);			
			}
		if (startnull == 0)
			{
			Tsub0->push_back(blank);
			Tsub1->push_back(blank);	
			Tsub2->push_back(blank);	
			Tsub3->push_back(blank);
			Tsub0CSV->push_back(-100.0);
			Tsub1CSV->push_back(-100.0);
			Tsub2CSV->push_back(-100.0);
			Tsub3CSV->push_back(-100.0);	
			}
		if (startnull == 1)
			{
			Tsub1->push_back(blank);	
			Tsub2->push_back(blank);	
			Tsub3->push_back(blank);
			Tsub1CSV->push_back(-100.0);
			Tsub2CSV->push_back(-100.0);
			Tsub3CSV->push_back(-100.0);	
			}
		if (startnull == 2)
			{
			Tsub2->push_back(blank);	
			Tsub3->push_back(blank);
			Tsub2CSV->push_back(-100.0);
			Tsub3CSV->push_back(-100.0);	
			}
		if (startnull == 3)
			{
			Tsub3->push_back(blank);
			Tsub3CSV->push_back(-100.0);	
			}
		// tau vars
		Nsubjettiness tau1(1, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau2(2, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau3(3, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness tau4(4, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		// make fastjets out of daughters:
		std::vector<fastjet::PseudoJet> Top_FJparticles;
		for (unsigned int k = 0; k < Tnsub; k++) 
			{
	        	pat::Jet const * new_constituent = dynamic_cast<pat::Jet const *>(ijet->daughter(k));
			if (new_constituent->energy() > 0.0 && new_constituent->px() > 0.0 && new_constituent->py() > 0.0 && new_constituent->pz() > 0.0)
				{
				Top_FJparticles.push_back( fastjet::PseudoJet( new_constituent->px(), new_constituent->py(), new_constituent->pz(), new_constituent->energy() ) );
				}
			}
		fastjet::PseudoJet TcombJet = fastjet::join(Top_FJparticles);
		Ttau1->push_back(tau1.result(TcombJet));
    		Ttau2->push_back(tau2.result(TcombJet));
    		Ttau3->push_back(tau3.result(TcombJet));
    		Ttau4->push_back(tau4.result(TcombJet));
		if (num_looselep > 0)
			{
			for (std::vector<reco::Candidate::PolarLorentzVector>::const_iterator it = looselep.begin() ; it != looselep.end(); ++it)
				{
				double dr_lepjet = reco::deltaR<double>( corrJet.eta(), corrJet.phi(), it->eta(), it->phi() );
				if (dr_lepjet < 0.4)
					{
					reco::Candidate::PolarLorentzVector lep (it->pt(), it->eta(), it->phi(), it->mass());
					corrJet = corrJet -lep;
					}
				}
			}
		Topjets->push_back(corrJet);

		}
	// AK5
	for ( std::vector<pat::Jet>::const_iterator jetBegin = h_ak5Jets->begin(), jetEnd = h_ak5Jets->end(), ijet = jetBegin; ijet != jetEnd; ++ijet ) 
	{
		reco::Candidate::LorentzVector uncorrJet = ijet->correctedP4(0);
		reco::Candidate::PolarLorentzVector corrJet (uncorrJet.pt(), uncorrJet.eta(), uncorrJet.phi(), uncorrJet.mass());
		AK5jetsCSV->push_back(ijet->bDiscriminator("combinedSecondaryVertexBJetTags"));
		AK5jets->push_back(corrJet);

	}


	// here we start finishing up
	// all the putting:
  	iEvent.put( npv, "npv");
	iEvent.put( muons, "muons");
	iEvent.put( muonsistight, "muonsistight");
	iEvent.put( muonsiso, "muonsiso");
	iEvent.put( muonscharge, "muonscharge");
	iEvent.put( electrons, "electrons");
	iEvent.put( electronsistight, "electronsistight");
	iEvent.put( electronsiso, "electronsiso");
	iEvent.put( electronscharge, "electronscharge");
	iEvent.put(AK5jets, "AK5jets");
	iEvent.put(AK5jetsCSV, "AK5jetsCSV");
	iEvent.put(CA8jets, "CA8jets");
	iEvent.put(CA8jetsCSV, "CA8jetsCSV");	
	iEvent.put(prunedCA8jets, "prunedCA8jets");
	iEvent.put(prunedCA8jetsCSV, "prunedCA8jetsCSV");
	iEvent.put(CA8tau1, "CA8tau1");
	iEvent.put(CA8tau2, "CA8tau2");
	iEvent.put(CA8tau3, "CA8tau3");
	iEvent.put(CA8tau4, "CA8tau4");
	iEvent.put(prunedCA8nsub, "prunedCA8nsub");
	iEvent.put(prunedCA8sub0, "prunedCA8sub0");
	iEvent.put(prunedCA8sub1, "prunedCA8sub1");
	iEvent.put(prunedCA8sub2, "prunedCA8sub2");
	iEvent.put(prunedCA8sub3, "prunedCA8sub3");
	iEvent.put(prunedCA8sub0CSV, "prunedCA8sub0CSV");
	iEvent.put(prunedCA8sub1CSV, "prunedCA8sub1CSV");
	iEvent.put(prunedCA8sub2CSV, "prunedCA8sub2CSV");
	iEvent.put(prunedCA8sub3CSV, "prunedCA8sub3CSV");
	iEvent.put(PCA8tau1, "PCA8tau1");
	iEvent.put(PCA8tau2, "PCA8tau2");
	iEvent.put(PCA8tau3, "PCA8tau3");
	iEvent.put(PCA8tau4, "PCA8tau4");
	iEvent.put(Topjets, "Topjets");
	iEvent.put(TjetsCSV, "TjetsCSV");
	iEvent.put(Tsub0, "Tsub0");
	iEvent.put(Tsub1, "Tsub1");
	iEvent.put(Tsub2, "Tsub2");
	iEvent.put(Tsub3, "Tsub3");
	iEvent.put(Tsub0CSV, "Tsub0CSV");
	iEvent.put(Tsub1CSV, "Tsub1CSV");
	iEvent.put(Tsub2CSV, "Tsub2CSV");
	iEvent.put(Tsub3CSV, "Tsub3CSV");
	iEvent.put(Ttau1, "Ttau1");
	iEvent.put(Ttau2, "Ttau2");
	iEvent.put(Ttau3, "Ttau3");
	iEvent.put(Ttau4, "Ttau4");

	return true;
}

void HEPtuplizer::endJob() 
	{
	std::cout << "end job\n";
	}


HEPtuplizer::~HEPtuplizer(){}

//define this as a plug-in
DEFINE_FWK_MODULE(HEPtuplizer);



*/





























