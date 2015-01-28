import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')
options.register('runOnData', 0, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "Flag for data (True) or MC (False), used to decide whether to apply b-tagging SF")
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
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('file:root://xrootd.unl.edu//store/results/B2G/TT_Mtt-700to1000_CT10_TuneZ2star_8TeV-powheg-tauola/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v3-99bd99199697666ff01397dad5652e9e/TT_Mtt-700to1000_CT10_TuneZ2star_8TeV-powheg-tauola/USER/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v3-99bd99199697666ff01397dad5652e9e/0000/02621A0E-40C2-E211-9F42-002590593902.root'))
process.diffmogen = cms.EDFilter('DiFfMoGeneral',
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				metSrc = cms.InputTag('patMETsPFlow'),
				isData = runOnData)
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
				isData = runOnData,
				jetScale = jesFactor,
				jetPtSmear = jerFactor,
				jetAngularSmear = jerFactor,
				jecPayloads = theJecPayloads,
				mkSubSize = cms.double(1.0),
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
				jetName = cms.string('TopTaggedCA8'))

process.diffmohep = process.diffmoca8pp.clone(
				jetSrc = cms.InputTag('goodPatJetsCAHEPTopTagPFPacked'),
				jetName = cms.string('HEPTopTagged'))

process.diffmoca8ttsub = process.diffmoca8pp.clone(
				jetSrc = cms.InputTag('selectedPatJetsCATopTagSubjetsPF'),
				subcorr = cms.string('no'),
				jetName = cms.string('SelectedTopTaggedSubjetsCA8'))

process.diffmoca8ppsub =  process.diffmoca8pp.clone(
				jetSrc = cms.InputTag('selectedPatJetsCA8PrunedSubjetsPF'),
				subcorr = cms.string('no'),
				jetName = cms.string('SelectedSubjetsPrunedCA8'))

process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.p = cms.Path(	process.diffmogen*
			process.diffmoleps1*
			process.diffmoleps2*
			process.diffmoca8*
			process.diffmoca8pp*
			process.diffmoca8tt*
			process.diffmohep
			)
process.out = cms.OutputModule("PoolOutputModule",
							   fileName = cms.untracked.string("diffmotester.root"),
							   SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p')),
							   outputCommands = cms.untracked.vstring('drop *','keep *_diffmo*_*_*', 'keep *_*prunedGenParticles*_*_*', 'keep *_diffmoHadronic_*_*', 'keep *_TriggerResults_*_PAT'))
process.outpath = cms.EndPath(process.out)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
print "-------------------------------------------------------------"
