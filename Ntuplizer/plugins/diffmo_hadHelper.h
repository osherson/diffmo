#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "Nsubjettiness.hh"
#include "Njettiness.hh"
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include "fastjet/tools/Filter.hh"
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ClusterSequenceArea.hh>

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"

#include "Analysis/Ntuplizer/plugins/smfaclookup.h"

// helper functions for hadronic process
namespace HADDF
{

	using namespace fastjet;

	typedef std::vector<reco::Candidate::PolarLorentzVector> p4_vector;
	reco::Candidate::PolarLorentzVector blank (0.0, 0.0, 0.0, -100.0);

	void CalculateTaus(std::vector<pat::Jet>::const_iterator jet, std::auto_ptr<std::vector<double> > &tau1, std::auto_ptr<std::vector<double>> &tau2, std::auto_ptr<std::vector<double>> &tau3, std::auto_ptr<std::vector<double>> &tau4)
	{
		// tau vars
		Nsubjettiness t1(1, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness t2(2, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness t3(3, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness t4(4, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		// make fastjets out of daughters:
		std::vector<fastjet::PseudoJet> FJparticles;
		for (unsigned int k = 0; k < jet->numberOfDaughters(); k++) 
		{
			const reco::PFCandidate* this_constituent = dynamic_cast<const reco::PFCandidate*>(jet->daughter(k));
			FJparticles.push_back( fastjet::PseudoJet( this_constituent->px(), this_constituent->py(), this_constituent->pz(), this_constituent->energy() ) );
		}		
		fastjet::PseudoJet combJet = fastjet::join(FJparticles);
		double T1 = t1.result(combJet);
		double T2 = t2.result(combJet);
		double T3 = t3.result(combJet);
		double T4 = t4.result(combJet);
		tau1->push_back(T1);
		tau2->push_back(T2);
		tau3->push_back(T3);
		tau4->push_back(T4);
	}

	void CalculateTaus(std::vector<pat::Jet>::const_iterator jet, std::auto_ptr<std::vector<double>> &tau1, std::auto_ptr<std::vector<double>> &tau2, std::auto_ptr<std::vector<double>> &tau3, std::auto_ptr<std::vector<double>> &tau4, double size, std::auto_ptr<p4_vector> &sub0kt, std::auto_ptr<p4_vector> &sub1kt, std::auto_ptr<p4_vector> &sub2kt, std::auto_ptr<p4_vector> &sub3kt, std::auto_ptr<p4_vector> &sub0ak, std::auto_ptr<p4_vector> &sub1ak, std::auto_ptr<p4_vector> &sub2ak, std::auto_ptr<p4_vector> &sub3ak, std::auto_ptr<p4_vector> &sub0exkt, std::auto_ptr<p4_vector> &sub1exkt, std::auto_ptr<p4_vector> &sub2exkt, std::auto_ptr<p4_vector> &sub3exkt)
	{
		// tau vars
		Nsubjettiness t1(1, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness t2(2, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness t3(3, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		Nsubjettiness t4(4, Njettiness::AxesMode::onepass_kt_axes, 1.0, 0.8);
		// make fastjets out of daughters:
		std::vector<fastjet::PseudoJet> FJparticles;
		for (unsigned int k = 0; k < jet->numberOfDaughters(); k++) 
		{
			const reco::PFCandidate* this_constituent = dynamic_cast<const reco::PFCandidate*>(jet->daughter(k));
			FJparticles.push_back( fastjet::PseudoJet( this_constituent->px(), this_constituent->py(), this_constituent->pz(), this_constituent->energy() ) );
		}		
		fastjet::PseudoJet combJet = fastjet::join(FJparticles);
		double T1 = t1.result(combJet);
		double T2 = t2.result(combJet);
		double T3 = t3.result(combJet);
		double T4 = t4.result(combJet);
		tau1->push_back(T1);
		tau2->push_back(T2);
		tau3->push_back(T3);
		tau4->push_back(T4);
		// make custom jets:
		// AK
		fastjet::JetDefinition jet_def_ak2(fastjet::antikt_algorithm, 0.2);
		fastjet::ClusterSequence clust_seq_ak(FJparticles, jet_def_ak2);
		double ptmin = 5.0;
		std::vector<fastjet::PseudoJet> subjets_ak = sorted_by_pt(clust_seq_ak.inclusive_jets(ptmin));
		int num_cust_subs_ak = subjets_ak.size();
		if (num_cust_subs_ak == 0)
		{
			sub0ak->push_back(blank);
			sub1ak->push_back(blank);
			sub2ak->push_back(blank);
			sub3ak->push_back(blank);
		}
		if (num_cust_subs_ak == 1)
		{
			reco::Candidate::PolarLorentzVector sub0_ak_P4 (subjets_ak[0].pt(),subjets_ak[0].eta(),subjets_ak[0].phi(),subjets_ak[0].m());
			sub0ak->push_back(sub0_ak_P4);
			sub1ak->push_back(blank);
			sub2ak->push_back(blank);
			sub3ak->push_back(blank);
		}
		if (num_cust_subs_ak == 2)
		{
			reco::Candidate::PolarLorentzVector sub0_ak_P4 (subjets_ak[0].pt(),subjets_ak[0].eta(),subjets_ak[0].phi(),subjets_ak[0].m());
			reco::Candidate::PolarLorentzVector sub1_ak_P4 (subjets_ak[1].pt(),subjets_ak[1].eta(),subjets_ak[1].phi(),subjets_ak[1].m());
			sub0ak->push_back(sub0_ak_P4);
			sub1ak->push_back(sub1_ak_P4);
			sub2ak->push_back(blank);
			sub3ak->push_back(blank);
		}
		if (num_cust_subs_ak == 3)
		{
			reco::Candidate::PolarLorentzVector sub0_ak_P4 (subjets_ak[0].pt(),subjets_ak[0].eta(),subjets_ak[0].phi(),subjets_ak[0].m());
			reco::Candidate::PolarLorentzVector sub1_ak_P4 (subjets_ak[1].pt(),subjets_ak[1].eta(),subjets_ak[1].phi(),subjets_ak[1].m());
			reco::Candidate::PolarLorentzVector sub2_ak_P4 (subjets_ak[2].pt(),subjets_ak[2].eta(),subjets_ak[2].phi(),subjets_ak[2].m());
			sub0ak->push_back(sub0_ak_P4);
			sub1ak->push_back(sub1_ak_P4);
			sub2ak->push_back(sub2_ak_P4);
			sub3ak->push_back(blank);
		}
		if (num_cust_subs_ak > 3)
		{
			reco::Candidate::PolarLorentzVector sub0_ak_P4 (subjets_ak[0].pt(),subjets_ak[0].eta(),subjets_ak[0].phi(),subjets_ak[0].m());
			reco::Candidate::PolarLorentzVector sub1_ak_P4 (subjets_ak[1].pt(),subjets_ak[1].eta(),subjets_ak[1].phi(),subjets_ak[1].m());
			reco::Candidate::PolarLorentzVector sub2_ak_P4 (subjets_ak[2].pt(),subjets_ak[2].eta(),subjets_ak[2].phi(),subjets_ak[2].m());
			reco::Candidate::PolarLorentzVector sub3_ak_P4 (subjets_ak[3].pt(),subjets_ak[3].eta(),subjets_ak[3].phi(),subjets_ak[3].m());
			sub0ak->push_back(sub0_ak_P4);
			sub1ak->push_back(sub1_ak_P4);
			sub2ak->push_back(sub2_ak_P4);
			sub3ak->push_back(sub3_ak_P4);
		}
		// KT
		fastjet::JetDefinition jet_def_kt2(fastjet::kt_algorithm, 0.2);
		fastjet::ClusterSequence clust_seq_kt(FJparticles, jet_def_kt2);
		std::vector<fastjet::PseudoJet> subjets_kt = sorted_by_pt(clust_seq_kt.inclusive_jets(ptmin));
		int num_cust_subs_kt = subjets_kt.size();
		if (num_cust_subs_kt == 0)
		{
			sub0kt->push_back(blank);
			sub1kt->push_back(blank);
			sub2kt->push_back(blank);
			sub3kt->push_back(blank);
		}
		if (num_cust_subs_kt == 1)
		{
			reco::Candidate::PolarLorentzVector sub0_kt_P4 (subjets_kt[0].pt(),subjets_kt[0].eta(),subjets_kt[0].phi(),subjets_kt[0].m());
			sub0kt->push_back(sub0_kt_P4);
			sub1kt->push_back(blank);
			sub2kt->push_back(blank);
			sub3kt->push_back(blank);
		}
		if (num_cust_subs_kt == 2)
		{
			reco::Candidate::PolarLorentzVector sub0_kt_P4 (subjets_kt[0].pt(),subjets_kt[0].eta(),subjets_kt[0].phi(),subjets_kt[0].m());
			reco::Candidate::PolarLorentzVector sub1_kt_P4 (subjets_kt[1].pt(),subjets_kt[1].eta(),subjets_kt[1].phi(),subjets_kt[1].m());
			sub0kt->push_back(sub0_kt_P4);
			sub1kt->push_back(sub1_kt_P4);
			sub2kt->push_back(blank);
			sub3kt->push_back(blank);
		}
		if (num_cust_subs_kt == 3)
		{
			reco::Candidate::PolarLorentzVector sub0_kt_P4 (subjets_kt[0].pt(),subjets_kt[0].eta(),subjets_kt[0].phi(),subjets_kt[0].m());
			reco::Candidate::PolarLorentzVector sub1_kt_P4 (subjets_kt[1].pt(),subjets_kt[1].eta(),subjets_kt[1].phi(),subjets_kt[1].m());
			reco::Candidate::PolarLorentzVector sub2_kt_P4 (subjets_kt[2].pt(),subjets_kt[2].eta(),subjets_kt[2].phi(),subjets_kt[2].m());
			sub0kt->push_back(sub0_kt_P4);
			sub1kt->push_back(sub1_kt_P4);
			sub2kt->push_back(sub2_kt_P4);
			sub3kt->push_back(blank);
		}
		if (num_cust_subs_kt > 3)
		{
			reco::Candidate::PolarLorentzVector sub0_kt_P4 (subjets_kt[0].pt(),subjets_kt[0].eta(),subjets_kt[0].phi(),subjets_kt[0].m());
			reco::Candidate::PolarLorentzVector sub1_kt_P4 (subjets_kt[1].pt(),subjets_kt[1].eta(),subjets_kt[1].phi(),subjets_kt[1].m());
			reco::Candidate::PolarLorentzVector sub2_kt_P4 (subjets_kt[2].pt(),subjets_kt[2].eta(),subjets_kt[2].phi(),subjets_kt[2].m());
			reco::Candidate::PolarLorentzVector sub3_kt_P4 (subjets_kt[3].pt(),subjets_kt[3].eta(),subjets_kt[3].phi(),subjets_kt[3].m());
			sub0kt->push_back(sub0_kt_P4);
			sub1kt->push_back(sub1_kt_P4);
			sub2kt->push_back(sub2_kt_P4);
			sub3kt->push_back(sub3_kt_P4);
		}
		// Exclusive KT
		fastjet::JetDefinition jet_def_exkt(fastjet::kt_algorithm, 1.5708);
		fastjet::ClusterSequence clust_seq_exkt(FJparticles, jet_def_exkt);
		int nSubJets = 3;
		std::vector<fastjet::PseudoJet> subjets_exkt = sorted_by_pt(clust_seq_exkt.exclusive_jets_up_to(nSubJets));
		int num_cust_subs_exkt = subjets_exkt.size();
		if (num_cust_subs_exkt == 0)
		{
			sub0exkt->push_back(blank);
			sub1exkt->push_back(blank);
			sub2exkt->push_back(blank);
			sub3exkt->push_back(blank);
		}
		if (num_cust_subs_exkt == 1)
		{
			reco::Candidate::PolarLorentzVector sub0_exkt_P4 (subjets_exkt[0].pt(),subjets_exkt[0].eta(),subjets_exkt[0].phi(),subjets_exkt[0].m());
			sub0exkt->push_back(sub0_exkt_P4);
			sub1exkt->push_back(blank);
			sub2exkt->push_back(blank);
			sub3exkt->push_back(blank);
		}
		if (num_cust_subs_exkt == 2)
		{
			reco::Candidate::PolarLorentzVector sub0_exkt_P4 (subjets_exkt[0].pt(),subjets_exkt[0].eta(),subjets_exkt[0].phi(),subjets_exkt[0].m());
			reco::Candidate::PolarLorentzVector sub1_exkt_P4 (subjets_exkt[1].pt(),subjets_exkt[1].eta(),subjets_exkt[1].phi(),subjets_exkt[1].m());
			sub0exkt->push_back(sub0_exkt_P4);
			sub1exkt->push_back(sub1_exkt_P4);
			sub2exkt->push_back(blank);
			sub3exkt->push_back(blank);
		}
		if (num_cust_subs_exkt == 3)
		{
			reco::Candidate::PolarLorentzVector sub0_exkt_P4 (subjets_exkt[0].pt(),subjets_exkt[0].eta(),subjets_exkt[0].phi(),subjets_exkt[0].m());
			reco::Candidate::PolarLorentzVector sub1_exkt_P4 (subjets_exkt[1].pt(),subjets_exkt[1].eta(),subjets_exkt[1].phi(),subjets_exkt[1].m());
			reco::Candidate::PolarLorentzVector sub2_exkt_P4 (subjets_exkt[2].pt(),subjets_exkt[2].eta(),subjets_exkt[2].phi(),subjets_exkt[2].m());
			sub0exkt->push_back(sub0_exkt_P4);
			sub1exkt->push_back(sub1_exkt_P4);
			sub2exkt->push_back(sub2_exkt_P4);
			sub3exkt->push_back(blank);
		}
		if (num_cust_subs_exkt > 3)
		{
			reco::Candidate::PolarLorentzVector sub0_exkt_P4 (subjets_exkt[0].pt(),subjets_exkt[0].eta(),subjets_exkt[0].phi(),subjets_exkt[0].m());
			reco::Candidate::PolarLorentzVector sub1_exkt_P4 (subjets_exkt[1].pt(),subjets_exkt[1].eta(),subjets_exkt[1].phi(),subjets_exkt[1].m());
			reco::Candidate::PolarLorentzVector sub2_exkt_P4 (subjets_exkt[2].pt(),subjets_exkt[2].eta(),subjets_exkt[2].phi(),subjets_exkt[2].m());
			reco::Candidate::PolarLorentzVector sub3_exkt_P4 (subjets_exkt[3].pt(),subjets_exkt[3].eta(),subjets_exkt[3].phi(),subjets_exkt[3].m());
			sub0exkt->push_back(sub0_exkt_P4);
			sub1exkt->push_back(sub1_exkt_P4);
			sub2exkt->push_back(sub2_exkt_P4);
			sub3exkt->push_back(sub3_exkt_P4);
		}
	}

	void PopulateSubjets(std::vector<pat::Jet>::const_iterator jet, std::auto_ptr<p4_vector> &sub0, std::auto_ptr<p4_vector> &sub1, std::auto_ptr<p4_vector> &sub2, std::auto_ptr<p4_vector> &sub3, unsigned int nsub, std::string btag, std::auto_ptr<std::vector<double>> &sub0csv, std::auto_ptr<std::vector<double>> &sub1csv, std::auto_ptr<std::vector<double>> &sub2csv, std::auto_ptr<std::vector<double>> &sub3csv)
	{
		int startnull = 0;
		if (nsub > 0) 
		{
			startnull++;
			const pat::Jet* d0_b = dynamic_cast<const pat::Jet*>(jet->daughter(0));
			reco::Candidate::LorentzVector d0_m = d0_b->correctedP4(0);
			reco::Candidate::PolarLorentzVector d0 (d0_m.pt(), d0_m.eta(), d0_m.phi(), d0_m.mass());
			sub0->push_back(d0);
			double b0 = d0_b->bDiscriminator(btag);
			sub0csv->push_back(b0);
		}
		if (nsub > 1) 
		{
			startnull++;
			const pat::Jet* d1_b = dynamic_cast<const pat::Jet*>(jet->daughter(1));
			reco::Candidate::LorentzVector d1_m = d1_b->correctedP4(0);
			reco::Candidate::PolarLorentzVector d1 (d1_m.pt(), d1_m.eta(), d1_m.phi(), d1_m.mass());
			sub1->push_back(d1);
			double b1 = d1_b->bDiscriminator(btag);
			sub1csv->push_back(b1);			
		}
		if (nsub > 2) 
		{
			startnull++;
			const pat::Jet* d2_b = dynamic_cast<const pat::Jet*>(jet->daughter(2));
			reco::Candidate::LorentzVector d2_m = d2_b->correctedP4(0);
			reco::Candidate::PolarLorentzVector d2 (d2_m.pt(), d2_m.eta(), d2_m.phi(), d2_m.mass());
			sub2->push_back(d2);
			double b2 = d2_b->bDiscriminator(btag);
			sub2csv->push_back(b2);				
		}
		if (nsub > 3) 
		{
			startnull++;
			const pat::Jet* d3_b = dynamic_cast<const pat::Jet*>(jet->daughter(3));
			reco::Candidate::LorentzVector d3_m = d3_b->correctedP4(0);
			reco::Candidate::PolarLorentzVector d3 (d3_m.pt(), d3_m.eta(), d3_m.phi(), d3_m.mass());
			sub3->push_back(d3);
			double b3 = d3_b->bDiscriminator(btag);
			sub3csv->push_back(b3);				
		}
		if (startnull == 0)
		{
			sub0->push_back(blank);
			sub1->push_back(blank);	
			sub2->push_back(blank);	
			sub3->push_back(blank);
			sub0csv->push_back(-100.0);
			sub1csv->push_back(-100.0);
			sub2csv->push_back(-100.0);
			sub3csv->push_back(-100.0);
		}
		if (startnull == 1)
		{
			sub1->push_back(blank);	
			sub2->push_back(blank);	
			sub3->push_back(blank);
			sub1csv->push_back(-100.0);
			sub2csv->push_back(-100.0);
			sub3csv->push_back(-100.0);
		}
		if (startnull == 2)
		{
			sub2->push_back(blank);	
			sub3->push_back(blank);
			sub2csv->push_back(-100.0);
			sub3csv->push_back(-100.0);
		}
		if (startnull == 3)
		{	
			sub3->push_back(blank);
			sub3csv->push_back(-100.0);
		}
	}

	void  ApplyJec(std::vector<pat::Jet>::const_iterator jet, boost::shared_ptr<FactorizedJetCorrector> jec, boost::shared_ptr<JetCorrectionUncertainty> jecUnc, bool isData, edm::Handle<std::vector<reco::GenJet> > genJ, std::auto_ptr<p4_vector> &jetC, std::auto_ptr<p4_vector> &sub0C, std::auto_ptr<p4_vector> &sub1C, std::auto_ptr<p4_vector> &sub2C, std::auto_ptr<p4_vector> &sub3C, unsigned int nsub, unsigned int npv, double rhoVal, double scale, double smear, double angularSmear, bool dosub, double &JEC)
	{
		reco::Candidate::LorentzVector uncorrJet = jet->correctedP4(0);
		jec->setJetEta( uncorrJet.eta() );
		jec->setJetPt ( uncorrJet.pt() );
		jec->setJetE  ( uncorrJet.energy() );
		jec->setJetA  ( jet->jetArea() );
		jec->setRho   ( rhoVal );
		jec->setNPV   ( npv );
		double corr = jec->getCorrection();
		reco::GenJet theMatchingGenJet;

		float eta1 = uncorrJet.eta();
		float phi1 = uncorrJet.phi();
		float eta2, phi2, deltaR;

		//Find matching genJet for systematic smearing
		if ( !isData ) {
			for ( std::vector<reco::GenJet>::const_iterator genJBegin = genJ->begin(), genJEnd = genJ->end(), igenjet = genJBegin; igenjet != genJEnd; ++igenjet) 
			{
				eta2 = igenjet->eta();
				phi2 = igenjet->phi();
				deltaR  = reco::deltaR(eta1, phi1, eta2, phi2);
				if (deltaR < 0.1) theMatchingGenJet = (*igenjet);
			}
		}		// scale_UP/DOWN
		if ( fabs(scale) > 0.0001 ) 
		{
			jecUnc->setJetEta( uncorrJet.eta() );
			jecUnc->setJetPt( uncorrJet.pt() * corr );
			double unc = fabs(jecUnc->getUncertainty( bool(scale > 0) ));

			// Add the "flat" flavor dependent corrections in quadrature
			unc = sqrt( unc*unc + scale*scale);
			double sign = 1.0;
			if ( scale < 0.0 ) sign = -1.0;

			// Scale up or down by jetScale_
			double ijetscale = (1 + sign * unc);
			corr *= ijetscale;
		}
		// angular_UP/DOWN
		double etaScale = 1.0;
		double phiScale = 1.0;
		if( fabs(angularSmear) > 0.0 && theMatchingGenJet.pt() > 15.0 ) 
		{
			double recoeta = jet->eta();
			double geneta = theMatchingGenJet.eta();
			double deltaeta = (recoeta-geneta)*angularSmear;
			etaScale = std::max((double) 0.0, (recoeta+deltaeta)/recoeta);
		}
		if( fabs(angularSmear) > 0.0 && theMatchingGenJet.pt() > 15.0 ) 
		{
			double recophi = jet->phi();
			double genphi = theMatchingGenJet.phi();
			double deltaphi = reco::deltaPhi(recophi, genphi)*angularSmear;
			phiScale = std::max((double)0.0,(recophi+deltaphi)/recophi);
		}
		// smear_UP/DOWN
		double ptSmear= 1.0;
		if( fabs(smear) > -1.0 && theMatchingGenJet.pt() > 15.0 )  //This is negative because even at smear=0.00 we want to smear the pt down
		{
			double recopt = jet->pt();
			double genpt = theMatchingGenJet.pt();
			float smear_factor = smear_factor_lookup(theMatchingGenJet.eta(), smear);
			double deltapt = (recopt-genpt)*smear_factor;
			ptSmear = std::max((double)0.0, (recopt+deltapt)/recopt);
		}

		reco::Candidate::PolarLorentzVector corrJet (uncorrJet.pt(), uncorrJet.eta(), uncorrJet.phi(), uncorrJet.mass());
		corrJet *=  (corr * ptSmear);
		corrJet.SetEta( corrJet.eta() * etaScale );
		corrJet.SetPhi( corrJet.phi() * phiScale );
	
		jetC->push_back( corrJet );
		JEC = corr * ptSmear;
		if (dosub)
		{
			int startnull = 0;
			if (nsub > 0)
			{
				startnull++;
				const pat::Jet* d0_b = dynamic_cast<const pat::Jet*>(jet->daughter(0));
				reco::Candidate::LorentzVector d0_m = d0_b->correctedP4(0);
				reco::Candidate::PolarLorentzVector d0 (d0_m.pt(), d0_m.eta(), d0_m.phi(), d0_m.mass());
				d0 *=  (corr * ptSmear);
				d0.SetEta( d0.eta() * etaScale );
				d0.SetPhi( d0.phi() * phiScale );
				sub0C->push_back(d0);		
			}
			if (nsub > 1) 
			{
				startnull++;
				const pat::Jet* d1_b = dynamic_cast<const pat::Jet*>(jet->daughter(1));
				reco::Candidate::LorentzVector d1_m = d1_b->correctedP4(0);
				reco::Candidate::PolarLorentzVector d1 (d1_m.pt(), d1_m.eta(), d1_m.phi(), d1_m.mass());
				d1 *=  (corr * ptSmear);
				d1.SetEta( d1.eta() * etaScale );
				d1.SetPhi( d1.phi() * phiScale );
				sub1C->push_back(d1)	;	
			}
			if (nsub > 2) 
			{
				startnull++;
				const pat::Jet* d2_b = dynamic_cast<const pat::Jet*>(jet->daughter(2));
				reco::Candidate::LorentzVector d2_m = d2_b->correctedP4(0);
				reco::Candidate::PolarLorentzVector d2 (d2_m.pt(), d2_m.eta(), d2_m.phi(), d2_m.mass());
				d2 *=  (corr * ptSmear);
				d2.SetEta( d2.eta() * etaScale );
				d2.SetPhi( d2.phi() * phiScale );
				sub2C->push_back(d2);		
			}
			if (nsub > 3) 
			{
				startnull++;
				const pat::Jet* d3_b = dynamic_cast<const pat::Jet*>(jet->daughter(3));
				reco::Candidate::LorentzVector d3_m = d3_b->correctedP4(0);
				reco::Candidate::PolarLorentzVector d3 (d3_m.pt(), d3_m.eta(), d3_m.phi(), d3_m.mass());
				d3 *=  (corr * ptSmear);
				d3.SetEta( d3.eta() * etaScale );
				d3.SetPhi( d3.phi() * phiScale );
				sub3C->push_back(d3)	;	
			}
			if (startnull == 0)
			{
				sub0C->push_back(blank);
				sub1C->push_back(blank);	
				sub2C->push_back(blank);	
				sub3C->push_back(blank);
			}
			if (startnull == 1)
			{
				sub1C->push_back(blank);	
				sub2C->push_back(blank);	
				sub3C->push_back(blank);
			}
			if (startnull == 2)
			{
				sub2C->push_back(blank);	
				sub3C->push_back(blank);
			}
			if (startnull == 3)
			{	
				sub3C->push_back(blank);
			}
		}
	}
	
	void AddTopTagInfo(std::vector<pat::Jet>::const_iterator jet, std::auto_ptr<std::vector<double> > &topTagMinMass, std::auto_ptr<std::vector<double> > &topTagTopMass, double JEC)
	{
		const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(jet->tagInfo("CATop"));
		topTagMinMass->push_back( catopTag->properties().minMass * (JEC) );
		topTagTopMass->push_back( catopTag->properties().topMass * (JEC) );
	}
}
