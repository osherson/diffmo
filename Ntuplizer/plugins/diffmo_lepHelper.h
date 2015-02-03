#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

// Helper functions for diffmo's lepton module
namespace LEPDF
{

	typedef std::vector<reco::Candidate::PolarLorentzVector> p4_vector;

	double lepPFIso(std::vector<pat::Muon>::const_iterator muon)
	{
		double chIso = muon->userIsolation(pat::PfChargedHadronIso);
		double nhIso = muon->userIsolation(pat::PfNeutralHadronIso);
		double gIso  = muon->userIsolation(pat::PfGammaIso);
		double pt    = muon->pt() ;
		double pfIso = (chIso + nhIso + gIso) / pt;
		return pfIso;
	}

	double lepPFIso(std::vector<pat::Electron>::const_iterator electron)
	{
		double chIso = electron->userIsolation(pat::PfChargedHadronIso);
		double nhIso = electron->userIsolation(pat::PfNeutralHadronIso);
		double gIso  = electron->userIsolation(pat::PfGammaIso);
		double pt    = electron->pt() ;
		double pfIso = (chIso + nhIso + gIso) / pt;
		return pfIso;
	}

	unsigned int lepTight(std::vector<pat::Muon>::const_iterator muon)
	{
		unsigned int is_tight_muon = 0;
		if (muon->isGlobalMuon()) 
			{
				if (muon->isTrackerMuon()) 
				{
					if (static_cast<int>( muon->numberOfValidHits()) > 10) 
					{
						if (muon->dB() < 0.2) 
						{
							if (muon->normChi2() < 10) 
							{
								if (muon->numberOfMatchedStations() > 0) 
								{
									if (muon->track()->hitPattern().numberOfValidPixelHits() > 0) 
									{
										is_tight_muon = 1;
									} 
								}
							}
						}
					}
				}
			}
		return is_tight_muon;
	}

	unsigned int lepTight(std::vector<pat::Electron>::const_iterator electron)
	{
		unsigned int is_tight_ele = 0;
		if (electron->dB() < 0.2) 
		{
			if (not electron->gsfTrack()->trackerExpectedHitsInner().numberOfHits() < 1) 
			{
				is_tight_ele = 1;
			}
		}
		return is_tight_ele;
	}

	unsigned int lepModTight(std::vector<pat::Electron>::const_iterator electron)
	{
		double abseta = std::abs(electron->superCluster()->eta());
		if (abseta <= 1.4442 || abseta >= 1.5660) 
		{
			return 0;
		}
		else
		{
			if (electron->dB() >= 0.02) return 0;
			else if (electron->gsfTrack()->dz() >= 0.01) return 0;
			else return 1;
		}
	}

	unsigned int lepTight2(std::vector<pat::Muon>::const_iterator muon, const edm::Handle< std::vector<reco::Vertex> > hVtx)
	{
		reco::Vertex vtx = *(hVtx->begin()); // I THINK we only want the first PV. It is supposed to be the most likely PV.
		unsigned int is_tight_muon = 0;
		if (muon->isTightMuon(vtx)) is_tight_muon = 1;
		return is_tight_muon;
	}

	unsigned int lepTight2(std::vector<pat::Electron>::const_iterator electron, const edm::Handle< std::vector<reco::Vertex> > hVtx, const edm::Handle<reco::ConversionCollection> hConCol, const edm::Handle<reco::BeamSpot> hBeamspot)
	{
		//Selection taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#New_instructions_GitHub 02/02/2015
		reco::Vertex vtx = *(hVtx->begin()); // I THINK we only want the first PV. It is supposed to be the most likely PV.

		unsigned int is_tight_ele = 0;
		if (electron->isEB()) 
		{
			if (fabs(electron->deltaEtaSuperClusterTrackAtVtx()) < 0.004)
			{
				if (fabs(electron->deltaPhiSuperClusterTrackAtVtx()) < 0.03)
				{
					if (electron->scSigmaIEtaIEta() < 0.01)
					{
						if (electron->hadronicOverEm() < 0.12)
						{
							// float d0vtx = electron->gsfTrack()->d0() - vtx.x() * math::sin(electron->gsfTrack()->phi()) + vtx.y() * math::cos(electron->gsfTrack()->phi());
							float d0vtx = electron->gsfTrack()->dxy(vtx.position());
							if (fabs(d0vtx) < 0.02)
							{
								// float dzvtx = (electron->vz() - vtx.z()) - ((electron->vx() - vtx.x()) * electron->px() + (electron->vy() - vtx.y() * electron->py()) / electron->pt() * electron->pz() / electron->pt();
								float dzvtx = electron->gsfTrack()->dz(vtx.position());
								if (fabs(dzvtx) < 0.1)
								{
									float e = electron->superCluster()->energy();
									// float p = electron->eSuperClusterOverP();
									float p = electron->trackMomentumAtVtx().R();
									float ep = 0;
									// if (e!=0) p = p / e;
									if (e!=0 and p!=0) ep = (1.0/e) - (1.0/p);
									else return is_tight_ele;
									if (fabs(ep) < 0.05)
									{
										if (lepPFIso(electron) < 0.10)
										{
											// if(hConCol.isValid() && beamspot.isValid())
											if(hConCol.isValid() && hBeamspot.isValid())
											{
												// if(electron->passConversionVeto())
												if(!ConversionTools::hasMatchedConversion( *electron, hConCol, hBeamspot.product()->position()))
												{
													if(electron->gsfTrack()->trackerExpectedHitsInner().numberOfHits() <= 0)
													{
														is_tight_ele=1;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (electron->isEE())
		{
			if (fabs(electron->deltaEtaSuperClusterTrackAtVtx()) < 0.005)
			{
				if (fabs(electron->deltaPhiSuperClusterTrackAtVtx()) < 0.02)
				{
					if (electron->scSigmaIEtaIEta() < 0.03)
					{
						if (electron->hadronicOverEm() < 0.10)
						{
							// float d0vtx = electron->gsfTrack()->d0() - vtx.x() * math::sin(electron->gsfTrack()->phi()) + vtx.y() * math::cos(electron->gsfTrack()->phi());
							float d0vtx = electron->gsfTrack()->dxy(vtx.position());
							if (fabs(d0vtx) < 0.02)
							{
								// float dzvtx = (electron->vz() - vtx.z()) - ((electron->vx() - vtx.x()) * electron->px() + (electron->vy() - vtx.y() * electron->py()) / electron->pt() * electron->pz() / electron->pt();
								float dzvtx = electron->gsfTrack()->dz(vtx.position());
								if (fabs(dzvtx) < 0.1)
								{
									float e = electron->superCluster()->energy();
									// float p = electron->eSuperClusterOverP();
									float p = electron->trackMomentumAtVtx().R();
									float ep = 0;
									// if (e!=0) p = p / e;
									if (e!=0 and p!=0) ep = (1.0/e) - (1.0/p);
									else return is_tight_ele;
									if (fabs(ep) < 0.05)
									{
										if ((electron->pt() >= 20 && lepPFIso(electron) < 0.10) || (electron->pt() < 20 && lepPFIso(electron) < 0.07))
										{
											if(hConCol.isValid() && hBeamspot.isValid())
											{
												// if(electron->passConversionVeto())
												if(!ConversionTools::hasMatchedConversion( *electron, hConCol, hBeamspot.product()->position()))
												{
													if(electron->gsfTrack()->trackerExpectedHitsInner().numberOfHits() <= 0)
													{
														is_tight_ele=1;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return is_tight_ele;
	}

	unsigned int lepLoose(std::vector<pat::Muon>::const_iterator muon)
	{
		unsigned int is_loose_muon = 0;
		if(muon->isLooseMuon()) is_loose_muon = 1;
		return is_loose_muon;
	}

	unsigned int lepLoose(std::vector<pat::Electron>::const_iterator electron, const edm::Handle< std::vector<reco::Vertex> > hVtx, const edm::Handle<reco::ConversionCollection> hConCol, const edm::Handle<reco::BeamSpot> hBeamspot)
	{
		//Selection taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#New_instructions_GitHub 02/02/2015
		reco::Vertex vtx = *(hVtx->begin()); // I THINK we only want the first PV. It is supposed to be the most likely PV.
		unsigned int is_loose_ele = 0;
		if (electron->isEB()) 
		{
			if (fabs(electron->deltaEtaSuperClusterTrackAtVtx()) < 0.007)
			{
				if (fabs(electron->deltaPhiSuperClusterTrackAtVtx()) < 0.15)
				{
					if (electron->scSigmaIEtaIEta() < 0.01)
					{
						if (electron->hadronicOverEm() < 0.12)
						{
							// float d0vtx = electron->gsfTrack()->d0() - vtx.x() * math::sin(electron->gsfTrack()->phi()) + vtx.y() * math::cos(electron->gsfTrack()->phi());
							float d0vtx = electron->gsfTrack()->dxy(vtx.position());
							if (fabs(d0vtx) < 0.02)
							{
								// float dzvtx = (electron->vz() - vtx.z()) - ((electron->vx() - vtx.x()) * electron->px() + (electron->vy() - vtx.y() * electron->py()) / electron->pt() * electron->pz() / electron->pt();
								float dzvtx = electron->gsfTrack()->dz(vtx.position());
								if (fabs(dzvtx) < 0.2)
								{
									float e = electron->superCluster()->energy();
									// float p = electron->eSuperClusterOverP();
									float p = electron->trackMomentumAtVtx().R();
									float ep = 0;
									// if (e!=0) p = p / e;
									if (e!=0 and p!=0) ep = (1.0/e) - (1.0/p);
									else return is_loose_ele;
									if (fabs(ep) < 0.05)
									{
										if (lepPFIso(electron) < 0.15)
										{
											if(hConCol.isValid() && hBeamspot.isValid())
											{
												// if(electron->passConversionVeto())
												if(!ConversionTools::hasMatchedConversion( *electron, hConCol, hBeamspot.product()->position()))
												{
													if(electron->gsfTrack()->trackerExpectedHitsInner().numberOfHits() <= 1)
													{
														is_loose_ele=1;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else if (electron->isEE())
		{
			if (fabs(electron->deltaEtaSuperClusterTrackAtVtx()) < 0.009)
			{
				if (fabs(electron->deltaPhiSuperClusterTrackAtVtx()) < 0.10)
				{
					if (electron->scSigmaIEtaIEta() < 0.03)
					{
						if (electron->hadronicOverEm() < 0.10)
						{
							// float d0vtx = electron->gsfTrack()->d0() - vtx.x() * math::sin(electron->gsfTrack()->phi()) + vtx.y() * math::cos(electron->gsfTrack()->phi());
							float d0vtx = electron->gsfTrack()->dxy(vtx.position());
							if (fabs(d0vtx) < 0.02)
							{
								// float dzvtx = (electron->vz() - vtx.z()) - ((electron->vx() - vtx.x()) * electron->px() + (electron->vy() - vtx.y() * electron->py()) / electron->pt() * electron->pz() / electron->pt();
								float dzvtx = electron->gsfTrack()->dz(vtx.position());
								if (fabs(dzvtx) < 0.2)
								{
									float e = electron->superCluster()->energy();
									// float p = electron->eSuperClusterOverP();
									float p = electron->trackMomentumAtVtx().R();
									float ep = 0;
									// if (e!=0) p = p / e;
									if (e!=0 and p!=0) ep = (1.0/e) - (1.0/p);
									else return is_loose_ele;
									if (fabs(ep) < 0.05)
									{
										if ((electron->pt() >= 20 && lepPFIso(electron) < 0.15) || (electron->pt() < 20 && lepPFIso(electron) < 0.10))
										{
											if(hConCol.isValid() && hBeamspot.isValid())
											{
												// if(electron->passConversionVeto())
												if(!ConversionTools::hasMatchedConversion( *electron, hConCol, hBeamspot.product()->position()))
												{
													if(electron->gsfTrack()->trackerExpectedHitsInner().numberOfHits() <= 1)
													{
														is_loose_ele=1;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return is_loose_ele;
	}
}
