import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')
options.register('runOnData', 1, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "Flag for data (True) or MC (False), used to decide whether to apply b-tagging SF")
options.register('JES', 'nominal', VarParsing.multiplicity.singleton, VarParsing.varType.string, "Flag for Jet Energy Scale. Options are nominal (off), up, and down - forced to nominal for data")
options.register('JER', 'nominal', VarParsing.multiplicity.singleton, VarParsing.varType.string, "Flag for Jet Energy Resolution Smearing. Options are nominal, up, and down - forced to nominal for data")
options.parseArguments()
print options

############### jesFactor ###############
# 0.00 for no jes uncertainty
# +/- n to scale up or down by n% and add in quadrature with uncertainty. 0.03 for CA8 jets from AK7. 

############### jerFactor ###############
# 0.10 for the default resolution smearing for both pT and the angular distributions
# +/- 0.10 to scale up or down the resolution. So, 0.20 or 0.00

if options.runOnData:
	runOnData = cms.bool(True)
	jesFactor = cms.double(0.00)
	jerFactor = cms.double(0.10)
else:
	runOnData = cms.bool(False)
	if options.JES == 'nominal':
		jesFactor = cms.double(0.00)
	if options.JES == 'up':
		jesFactor = cms.double(0.03)
	if options.JES == 'down':
		jesFactor = cms.double(-0.03)
	if options.JER == 'nominal':
		jerFactor = cms.double(0.10)
	if options.JER == 'up':
		jerFactor = cms.double(0.20)
	if options.JER == 'down':
		jerFactor = cms.double(0.00)

if options.runOnData:
	theJecPayloads = cms.vstring([
		'Winter14_V5_DATA_L1FastJet_AK7PFchs.txt',
		'Winter14_V5_DATA_L2Relative_AK7PFchs.txt',
		'Winter14_V5_DATA_L3Absolute_AK7PFchs.txt',
		'Winter14_V5_DATA_L2L3Residual_AK7PFchs.txt',
		'Winter14_V5_DATA_Uncertainty_AK7PFchs.txt'
	])
else:
	theJecPayloads = cms.vstring([
		'START53_V27_L1FastJet_AK7PFchs.txt',
		'START53_V27_L2Relative_AK7PFchs.txt',
		'START53_V27_L3Absolute_AK7PFchs.txt',
		'START53_V27_Uncertainty_AK7PFchs.txt'
	])

# Run:
process = cms.Process("diffmo")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents))
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('file:root://xrootd.unl.edu//store/results/b2g/StoreResults/JetHT/USER/Run2012B_22Jan2013_TLBSM_53x_0161496fccaa0bf55fbb525b618345b5-v1/00000/00F149B6-E46B-E411-841F-0025905A610C.root'))
process.diffmoGen = cms.EDFilter('DiFfMoGeneral',
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				metSrc = cms.InputTag('patMETsPFlow'),
				isData = runOnData)
process.diffmoMuonPFlowLoose = cms.EDFilter('DiFfMoLepton',
				lepSrc = cms.InputTag('selectedPatMuonsPFlowLoose'),
				lepType = cms.string('muon'),
				lepName = cms.string('muon'),
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				beamSpotSrc = cms.InputTag('offlineBeamSpot'))
process.diffmoElePFlowLoose = cms.EDFilter('DiFfMoLepton',
				lepSrc = cms.InputTag('selectedPatElectronsPFlowLoose'),
				lepType = cms.string('electron'),
				lepName = cms.string('electron'),
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				beamSpotSrc = cms.InputTag('offlineBeamSpot'))
process.diffmoCa8 =   cms.EDFilter("DiFfMoHadronic",
				jetSrc = cms.InputTag('goodPatJetsCA8PF'),
				genSrc = cms.InputTag('ca8GenJetsNoNu'),
				npvSrc = cms.InputTag("diffmoGen", "npv"),
				rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
				useNsub = cms.string('yes'),
				subcorr = cms.string('no'),
				btagType = cms.string(''),
				basecorr = cms.int32(1),
				isData = runOnData,
				jetScale = jesFactor,
				jetPtSmear = jerFactor,
				jetAngularSmear = jerFactor,
				jecPayloads = theJecPayloads,
				mkSubSize = cms.double(1.0),
				addTopTagInfo = cms.bool(False),
				jetName = cms.string('UnprunedCA8'))

process.diffmoAk5 = process.diffmoCa8.clone(
				jetSrc = cms.InputTag('goodPatJetsPFlow'),
				genSrc = cms.InputTag('ak5GenJetsNoNu'),
				# rhoSrc = cms.InputTag('ak5GenJetsNoNu', 'rho'), #kt6 is the only one filled in our pat-tuples
				useNsub = cms.string('no'),
				jetName = cms.string('AK5'))

process.diffmoCa8pp = process.diffmoCa8.clone(
				jetSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
				useNsub = cms.string('no'),
				subcorr = cms.string('yes'),
				btagType = cms.string('combinedSecondaryVertexBJetTags'),
				jetName = cms.string('PrunedCA8'))

process.diffmoCa8tt = process.diffmoCa8pp.clone(
				jetSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
				addTopTagInfo = cms.bool(True),
				jetName = cms.string('TopTaggedCA8'))

process.diffmoHep = process.diffmoCa8pp.clone(
				jetSrc = cms.InputTag('goodPatJetsCAHEPTopTagPFPacked'),
				jetName = cms.string('HEPTopTagged'))

process.diffmoCa8ttsub = process.diffmoCa8pp.clone(
				jetSrc = cms.InputTag('selectedPatJetsCATopTagSubjetsPF'),
				subcorr = cms.string('no'),
				jetName = cms.string('SelectedTopTaggedSubjetsCA8'))

process.diffmoCa8ppsub =  process.diffmoCa8pp.clone(
				jetSrc = cms.InputTag('selectedPatJetsCA8PrunedSubjetsPF'),
				subcorr = cms.string('no'),
				jetName = cms.string('SelectedSubjetsPrunedCA8'))

process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.p = cms.Path(	
			process.diffmoGen*
			process.diffmoMuonPFlowLoose*
			process.diffmoElePFlowLoose*
			process.diffmoCa8*
			process.diffmoCa8pp*
			process.diffmoCa8tt*
			process.diffmoHep*
			process.diffmoAk5
			)
process.out = cms.OutputModule("PoolOutputModule",
							   fileName = cms.untracked.string("diffmotester_data.root"),
							   SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p')),
							   outputCommands = cms.untracked.vstring('drop *','keep *_diffmo*_*_*', 'keep *_*prunedGenParticles*_*_*', 'keep *_TriggerResults_*_HLT'))
process.outpath = cms.EndPath(process.out)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
print "-------------------------------------------------------------"
