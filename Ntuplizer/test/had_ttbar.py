import FWCore.ParameterSet.Config as cms

########################### good vs selected? ############################

# Run:
process = cms.Process("diffmo")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1000))
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('root://xrootd.unl.edu//store/results/B2G/TT_Mtt-700to1000_CT10_TuneZ2star_8TeV-powheg-tauola/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v3-99bd99199697666ff01397dad5652e9e/TT_Mtt-700to1000_CT10_TuneZ2star_8TeV-powheg-tauola/USER/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v3-99bd99199697666ff01397dad5652e9e/0000/02621A0E-40C2-E211-9F42-002590593902.root'))
process.diffmogen = cms.EDFilter('DiFfMoGeneral',
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				metSrc = cms.InputTag('patMETsPFlow'),
				triggerSrc = cms.InputTag('placeholder'),
				isData = cms.bool(False),
				readTriggers = cms.bool(False),
				triggers = cms.vstring([
					'placeholder1',
					'placeholder2'
					]))
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
				isData = cms.bool(False),
				jetScale = cms.double(0.0),   #
				jetPtSmear = cms.double(0.1), # note these three are fractional
				jetEtaSmear = cms.double(0.1),#
				jecPayloads = cms.vstring([
					'START53_L1FastJet_AK7PFchs.txt',
					'START53_L2Relative_AK7PFchs.txt',
					'START53_L3Absolute_AK7PFchs.txt',
					'START53_Uncertainty_AK7PFchs.txt'
					]),
				mkSubSize = cms.double(0.0),
				addTopTagInfo = cms.bool(False),
				jetName = cms.string('UnprunedCA8'))

process.diffmoca8pp = process.diffmoca8.clone(
				jetSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
				useNsub = cms.string('no'),
				subcorr = cms.string('yes'),
				btagType = cms.string('combinedSecondaryVertexBJetTags'),
				jetName = cms.string('PrunedCA8'))

process.diffmoca8tt = process.diffmoca8pp.clone(
				jetSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
				addTopTagInfo = cms.bool(True),
				jetName = cms.string('TopTaggedPrunedCA8'))

process.diffmoca8ttsub = process.diffmoca8pp.clone(
				jetSrc = cms.InputTag('selectedPatJetsCATopTagSubjetsPF'),
				subcorr = cms.string('no'),
				jetName = cms.string('SelectedTopTaggedSubjetsCA8'))

process.diffmoca8ppsub =  process.diffmoca8pp.clone(
				jetSrc = cms.InputTag('selectedPatJetsCA8PrunedSubjetsPF'),
				subcorr = cms.string('no'),
				jetName = cms.string('SelectedSubjetsPrunedCA8'))

#goodPatJetsCA8PF
#goodPatJetsCA8PrunedPFPacked
#goodPatJetsCATopTagPFPacked
#				jetSrc = cms.InputTag('selectedPatJetsCATopTagSubjetsPF'),
#selectedPatJetsCA8PrunedSubjetsPF
# 
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.p = cms.Path(	process.diffmogen*
			process.diffmoleps1*
			process.diffmoleps2*
			process.diffmoca8*
			process.diffmoca8pp*
			process.diffmoca8tt
			#process.diffmoHadronicTop
			#process.diffmoca8tt
			#process.diffmoca8ppsub*
			#process.diffmoca8ttsub

			)
process.out = cms.OutputModule("PoolOutputModule",
							   fileName = cms.untracked.string("diffmotester.root"),
							   SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p')),
							   outputCommands = cms.untracked.vstring('drop *','keep *_diffmo*_*_*', 'keep *_*prunedGenParticles*_*_*', 'keep *_diffmoHadronic_*_*'))
process.outpath = cms.EndPath(process.out)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
print "-------------------------------------------------------------"
