import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')
options.register('runOnData', 0, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "Flag for data (True) or MC (False), used to decide whether to apply b-tagging SF")
options.register('JES', 'nominal', VarParsing.multiplicity.singleton, VarParsing.varType.string, "Flag for Jet Energy Scale. Options are nominal (off), up, and down - forced to nominal for data")
options.register('JER', 'nominal', VarParsing.multiplicity.singleton, VarParsing.varType.string, "Flag for Jet Energy Resolution Smearing. Options are nominal, up, and down - forced to nominal for data")
options.register('includePDF', 1, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "Flag for if to include PDF Weights")
options.register('runOnCrab', 0, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "Flag to process JEC files properly on the grid")
options.setDefault('maxEvents', -1)
options.setDefault('outputFile', 'jhutester.root')
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
	filenames = cms.untracked.vstring("/store/user/lpctlbsm/knash/SingleElectron/Run2012C-22Jan2013-MissingLumi_take2-v1_TLBSM_53x_v3/b11da83ff7a298b6297fefd09b0b7f27/tlbsm_53x_v3_data_10_1_bTg.root")
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
	filenames = cms.untracked.vstring("/store/results/B2G/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v3-99bd99199697666ff01397dad5652e9e/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/USER/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v3-99bd99199697666ff01397dad5652e9e/0000/76662898-3BD9-E211-8290-003048678FFA.root")

if options.runOnCrab:
	jec_prepend = ''
else:
	jec_prepend = '../JEC/'

if options.runOnData:
	theJecPayloads_AK7 = cms.vstring([
		jec_prepend+'Winter14_V5_DATA_L1FastJet_AK7PFchs.txt',
		jec_prepend+'Winter14_V5_DATA_L2Relative_AK7PFchs.txt',
		jec_prepend+'Winter14_V5_DATA_L3Absolute_AK7PFchs.txt',
		jec_prepend+'Winter14_V5_DATA_L2L3Residual_AK7PFchs.txt',
		jec_prepend+'Winter14_V5_DATA_Uncertainty_AK7PFchs.txt'
	])
	theJecPayloads_AK5 = cms.vstring([
		jec_prepend+'Winter14_V5_DATA_L1FastJet_AK5PFchs.txt',
		jec_prepend+'Winter14_V5_DATA_L2Relative_AK5PFchs.txt',
		jec_prepend+'Winter14_V5_DATA_L3Absolute_AK5PFchs.txt',
		jec_prepend+'Winter14_V5_DATA_L2L3Residual_AK5PFchs.txt',
		jec_prepend+'Winter14_V5_DATA_Uncertainty_AK5PFchs.txt'
	])
else:
	theJecPayloads_AK7 = cms.vstring([
		jec_prepend+'START53_V27_L1FastJet_AK7PFchs.txt',
		jec_prepend+'START53_V27_L2Relative_AK7PFchs.txt',
		jec_prepend+'START53_V27_L3Absolute_AK7PFchs.txt',
		jec_prepend+'START53_V27_Uncertainty_AK7PFchs.txt'
	])
	theJecPayloads_AK5 = cms.vstring([
		jec_prepend+'START53_V27_L1FastJet_AK5PFchs.txt',
		jec_prepend+'START53_V27_L2Relative_AK5PFchs.txt',
		jec_prepend+'START53_V27_L3Absolute_AK5PFchs.txt',
		jec_prepend+'START53_V27_Uncertainty_AK5PFchs.txt'
	])

# Run:
process = cms.Process("jhu")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents))
process.source = cms.Source("PoolSource", fileNames = filenames)
#General
process.jhuGen = cms.EDFilter('jhuGeneral',
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				metSrc = cms.InputTag('patMETsPFlow'),
				isData = runOnData)
#Muons
process.jhuMuonPFlow = cms.EDFilter('jhuLepton',
				lepSrc = cms.InputTag('selectedPatMuonsPFlow'),
				lepType = cms.string('muon'),
				lepName = cms.string('muon'),
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				beamSpotSrc = cms.InputTag('offlineBeamSpot'))
process.jhuMuonPFlowLoose = process.jhuMuonPFlow.clone(
				lepSrc = cms.InputTag('selectedPatMuonsPFlowLoose'),
				lepName = cms.string('muonLoose'))
#Electrons
process.jhuElePFlow = cms.EDFilter('jhuLepton',
				lepSrc = cms.InputTag('selectedPatElectronsPFlow'),
				lepType = cms.string('electron'),
				lepName = cms.string('electron'),
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				beamSpotSrc = cms.InputTag('offlineBeamSpot'))
process.jhuElePFlowLoose = process.jhuElePFlow.clone(
				lepSrc = cms.InputTag('selectedPatElectronsPFlowLoose'),
				lepName = cms.string('electronLoose'))
#CA8 Jets
process.jhuCa8 = cms.EDFilter("jhuHadronic",
				jetSrc = cms.InputTag('goodPatJetsCA8PF'),
				genSrc = cms.InputTag('ca8GenJetsNoNu'),
				npvSrc = cms.InputTag("jhuGen", "npv"),
				rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
				useNsub = cms.string('yes'),
				subcorr = cms.string('no'),
				btagType = cms.string(''),
				basecorr = cms.int32(1),
				isData = runOnData,
				jetScale = jesFactor,
				jetPtSmear = jerFactor,
				jetAngularSmear = jerFactor,
				jecPayloads = theJecPayloads_AK7,
				mkSubSize = cms.double(1.0),
				addTopTagInfo = cms.bool(False),
				jetName = cms.string('UnprunedCA8'))
#Pruned CA8 Jets
process.jhuCa8pp = process.jhuCa8.clone(
				jetSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
				useNsub = cms.string('no'),
				subcorr = cms.string('yes'),
				btagType = cms.string('combinedSecondaryVertexBJetTags'),
				jetName = cms.string('PrunedCA8'))
#TopTagged CA8 Jets
process.jhuCa8tt = process.jhuCa8pp.clone(
				jetSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
				addTopTagInfo = cms.bool(True),
				jetName = cms.string('TopTaggedCA8'))
#HEP Top Tagged Jets
process.jhuHep = process.jhuCa8pp.clone(
				jetSrc = cms.InputTag('goodPatJetsCAHEPTopTagPFPacked'),
				jetName = cms.string('HEPTopTagged'))
#AK5 Jets
process.jhuAk5 = process.jhuCa8.clone(
				jetSrc = cms.InputTag('goodPatJetsPFlow'),
				genSrc = cms.InputTag('ak5GenJetsNoNu'),
				# rhoSrc = cms.InputTag('ak5GenJetsNoNu', 'rho'), #kt6 is the only one filled in our pat-tuples
				useNsub = cms.string('no'),
				jecPayloads = theJecPayloads_AK5,
				jetName = cms.string('AK5'))
#PDF Weights
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
	# Fix POWHEG if buggy (this PDF set will also appear on output, 
	# so only two more PDF sets can be added in PdfSetNames if not "")
	FixPOWHEG = cms.untracked.string(""),
	GenTag = cms.untracked.InputTag("prunedGenParticles"),
	PdfInfoTag = cms.untracked.InputTag("generator"),
	PdfSetNames = cms.untracked.vstring("CT10.LHgrid",
										"GJR08VFnloE.LHgrid",
										"cteq66.LHgrid"))

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.p = cms.Path(
			process.jhuGen*
			process.jhuMuonPFlow*
			process.jhuMuonPFlowLoose*
			process.jhuElePFlow*
			process.jhuElePFlowLoose*
			process.jhuCa8*
			process.jhuCa8pp*
			process.jhuCa8tt*
			process.jhuHep*
			process.jhuAk5)

if options.includePDF:
	process.p *= process.pdfWeights

process.out = cms.OutputModule("PoolOutputModule",
							   fileName = cms.untracked.string(options.outputFile),
							   SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p')),
							   outputCommands = cms.untracked.vstring(
								'drop *',
								'keep *_jhu*_*_*',
								'keep *_*prunedGenParticles*_*_*',
								'keep *_TriggerResults_*_HLT',
								'keep *_pdfWeights*_*_*'))
process.outpath = cms.EndPath(process.out)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
print "-------------------------------------------------------------"
