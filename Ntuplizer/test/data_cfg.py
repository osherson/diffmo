import FWCore.ParameterSet.Config as cms

# Run:

process = cms.Process("diffmo")
process.load("FWCore.MessageService.MessageLogger_cfi")

isData_=cms.bool(True)	#Set to False to run on MC and use genParticles Information

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('file:/eos/uscms/store/user/lpctlbsm/noreplica/yxin/Jet/Run2012A-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_100_1_hIa.root'))
process.diffmogen = cms.EDFilter('DiFfMoGeneral',
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				metSrc = cms.InputTag('patMETsPFlow'))
process.diffmoleps1 = cms.EDFilter('DiFfMoLepton',
				lepSrc = cms.InputTag('selectedPatMuonsPFlowLoose'),
				lepType = cms.string('muon'),		#options for leptons are electron, muon
				lepName = cms.string('muons'))		#this is the name of your collection
process.diffmoleps2 = cms.EDFilter('DiFfMoLepton',
				lepSrc = cms.InputTag('selectedPatElectronsPFlowLoose'),
				lepType = cms.string('electron'),	#options for leptons are electron, muon
				lepName = cms.string('electrons'))	#this is the name of your collection
process.diffmoca8 =   cms.EDFilter("DiFfMoHadronic",					#If your jet collection is pruned and packed you can use the following:
				jetSrc = cms.InputTag('goodPatJetsCA8PF'),				#btagging(btagType) and corrected subjets (subcorr)
				genSrc = cms.InputTag('ca8GenJetsNoNu'),
				npvSrc = cms.InputTag("diffmogen", "npv"),
				jetName = cms.string('UnprunedCA8'),
				btagType = cms.string(''),
				rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
				useNsub = cms.string('yes'),
				subcorr = cms.string('no'),
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
				isData = isData_)

process.diffmoca8pp =   cms.EDFilter("DiFfMoHadronic",
				jetSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
				genSrc = cms.InputTag('ca8GenJetsNoNu'),
				npvSrc = cms.InputTag("diffmogen", "npv"),
				jetName = cms.string('PrunedCA8'),
				btagType = cms.string('combinedSecondaryVertexBJetTags'),
				rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
				useNsub = cms.string('no'),
				subcorr = cms.string('yes'),
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
				isData = isData_)

# 
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.p = cms.Path(	
			process.diffmogen*
			process.diffmoleps1*
			process.diffmoleps2*
			process.diffmoca8*
			process.diffmoca8pp
			)
process.out = cms.OutputModule("PoolOutputModule",
		   fileName = cms.untracked.string("diffmotester.root"),
		   SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p')),
		   outputCommands = cms.untracked.vstring('drop *','keep *_diffmo*_*_*', 'keep *_*prunedGenParticles*_*_*'))
process.outpath = cms.EndPath(process.out)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
print "-------------------------------------------------------------"
