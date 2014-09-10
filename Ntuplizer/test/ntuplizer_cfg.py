import FWCore.ParameterSet.Config as cms

# Run:


process = cms.Process("diffmo")
process.load("FWCore.MessageService.MessageLogger_cfi")

isData_=cms.bool(False)	#Set to False to run on MC and use genParticles Information

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('file:/eos/uscms/store/user/lpctlbsm/noreplica/knash/TT_Mtt-700to1000_CT10_scaleup_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v3/99bd99199697666ff01397dad5652e9e/tlbsm_53x_v3_mc_33_1_0si.root'))
process.diffmogen = cms.EDFilter('DiFfMoGeneral',
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				metSrc = cms.InputTag('patMETsPFlow'))
process.diffmoleps1 = cms.EDFilter('DiFfMoLepton',
				lepSrc = cms.InputTag('selectedPatMuonsPFlowLoose'),
				lepType = cms.string('muon'),
				lepName = cms.string('muons'))
process.diffmoleps2 = cms.EDFilter('DiFfMoLepton',
				lepSrc = cms.InputTag('selectedPatElectronsPFlowLoose'),
				lepType = cms.string('electron'),
				lepName = cms.string('electrons'))
process.diffmoca8 =   cms.EDFilter("DiFfMoHadronic",
				jetSrc = cms.InputTag('goodPatJetsCA8PF'),
				genSrc = cms.InputTag('ca8GenJetsNoNu'),
				npvSrc = cms.InputTag("diffmogen", "npv"),
				rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
				useNsub = cms.string('yes'),
				subcorr = cms.string('no'),
				btagType = cms.string(''),
				basecorr = cms.int32(1),
				jetScale = cms.double(0.0),   #
				jetPtSmear = cms.double(0.1), # note these three are fractional
				jetEtaSmear = cms.double(0.1),#
				jecPayloads = cms.vstring([
				    'START53_L1FastJet_AK7PFchs.txt',
				    'START53_L2Relative_AK7PFchs.txt',
				    'START53_L3Absolute_AK7PFchs.txt',
				    'START53_Uncertainty_AK7PFchs.txt'
				    ]),
				jetName = cms.string('UnprunedCA8'),
				isData = isData_)

process.diffmoca8pp =   cms.EDFilter("DiFfMoHadronic",
				jetSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
				genSrc = cms.InputTag('ca8GenJetsNoNu'),
				npvSrc = cms.InputTag("diffmogen", "npv"),
				rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
				useNsub = cms.string('no'),
				subcorr = cms.string('yes'),
				btagType = cms.string('combinedSecondaryVertexBJetTags'),
				basecorr = cms.int32(1),
				jetScale = cms.double(0.0),   #
				jetPtSmear = cms.double(0.1), # note these three are fractional
				jetEtaSmear = cms.double(0.1),#
				jecPayloads = cms.vstring([
				    'START53_L1FastJet_AK7PFchs.txt',
				    'START53_L2Relative_AK7PFchs.txt',
				    'START53_L3Absolute_AK7PFchs.txt',
				    'START53_Uncertainty_AK7PFchs.txt'
				    ]),
				jetName = cms.string('PrunedCA8'),
				isData = isData_)

process.diffmoca8toptag =   cms.EDFilter("DiFfMoHadronic",
				jetSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
				genSrc = cms.InputTag('ca8GenJetsNoNu'),
				npvSrc = cms.InputTag("diffmogen", "npv"),
				rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
				useNsub = cms.string('no'),
				subcorr = cms.string('yes'),
				btagType = cms.string('combinedSecondaryVertexBJetTags'),
				basecorr = cms.int32(1),
				jetScale = cms.double(0.0),   #
				jetPtSmear = cms.double(0.1), # note these three are fractional
				jetEtaSmear = cms.double(0.1),#
				jecPayloads = cms.vstring([
				    'START53_L1FastJet_AK7PFchs.txt',
				    'START53_L2Relative_AK7PFchs.txt',
				    'START53_L3Absolute_AK7PFchs.txt',
				    'START53_Uncertainty_AK7PFchs.txt'
				    ]),
				jetName = cms.string('TopTagCA8'),
				isData = isData_)

process.diffmoak5 =   cms.EDFilter("DiFfMoHadronic",
				jetSrc = cms.InputTag('goodPatJetsPFlow'),
				genSrc = cms.InputTag('ca8GenJetsNoNu'),
				npvSrc = cms.InputTag("diffmogen", "npv"),
				rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
				useNsub = cms.string('no'),
				subcorr = cms.string('no'),
				btagType = cms.string('combinedSecondaryVertexBJetTags'),
				basecorr = cms.int32(0),
				jetScale = cms.double(0.0),   #
				jetPtSmear = cms.double(0.1), # note these three are fractional
				jetEtaSmear = cms.double(0.1),#
				jecPayloads = cms.vstring([
				    'START53_L1FastJet_AK7PFchs.txt',
				    'START53_L2Relative_AK7PFchs.txt',
				    'START53_L3Absolute_AK7PFchs.txt',
				    'START53_Uncertainty_AK7PFchs.txt'
				    ]),
				jetName = cms.string('UnprunedAK5'),
				isData = isData_)


# 
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.p = cms.Path(	process.diffmogen*
			process.diffmoleps1*
			process.diffmoleps2*
			process.diffmoca8*
			process.diffmoca8pp*
			process.diffmoca8toptag#*
#			process.diffmoak5
			)
process.out = cms.OutputModule("PoolOutputModule",
			       fileName = cms.untracked.string("diffmo_v3.root"),
			       SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p')),
			       outputCommands = cms.untracked.vstring('drop *','keep *_diffmo*_*_*', 'keep *_*prunedGenParticles*_*_*'))
process.outpath = cms.EndPath(process.out)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
print "-------------------------------------------------------------"
