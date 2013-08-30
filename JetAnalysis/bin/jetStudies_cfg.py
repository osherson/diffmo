import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.pfJetStudies = cms.PSet(
    # input parameter sets
    jetSrc = cms.InputTag('selectedPatJets'),
    pfJetSrc = cms.InputTag('selectedPatJetsPFlow'),
    metSrc = cms.InputTag('patMETs'),
    pfMetSrc = cms.InputTag('patMETsPF'),
    useCalo = cms.bool(False),
    minNJets = cms.uint32(2),
    ptMin = cms.double(25.0),
    etaMax = cms.double(3.0),
    htMin = cms.double(0.0),
    ht2Min = cms.double(0.0),
    cutsToIgnore = cms.vstring( ['PF Jet ID'] )
)

process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')

process.plotParameters = cms.PSet (
    doTracks = cms.bool(False),
    useMC = cms.bool(False),
    runs = cms.vint32([])
)


process.inputs = cms.PSet (
    fileNames = cms.vstring(
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_10_1_EJL.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_11_1_MFK.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_12_1_mEi.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_13_1_M5k.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_14_1_mrk.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_15_1_6iz.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_16_1_Okw.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_17_1_oMY.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_18_1_uRZ.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_1_1_Z5O.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_2_1_Fnp.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_3_1_wTC.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_4_1_Ii3.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_5_1_j9a.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_6_1_KU9.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_7_1_6Rd.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_8_1_c48.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/Jet/ttbsm_v1_Run2011A-PromptReco-v1/3dc70b6acc7a164ec5660c65e5f36a85/ttbsm_413_9_1_34q.root'
        ),
        lumisToProcess = cms.untracked.VLuminosityBlockRange(
	'160431:19-160431:218',
	'160577:254-160577:306',
	'160578:6-160578:53',
	'160578:274-160578:400',
	'160871:68-160871:208',
	'160872:1-160872:9',
	'160872:25-160872:35',
	'160872:38-160872:55',
	'160873:1-160873:147',
	'160874:1-160874:51',
	'160874:97-160874:113',
	'160939:1-160939:123',
	'160940:1-160940:79',
	'160942:1-160942:12',
	'160943:1-160943:54',
	'160955:55-160955:71',
	'160955:73-160955:76',
	'160955:78-160955:86',
	'160955:88-160955:130',
	'160955:133-160955:138',
	'160955:140-160955:151',
	'160955:153-160955:154',
	'160955:156-160955:172',
	'160955:175-160955:201',
	'160955:204-160955:206',
	'160956:2-160956:65',
	'160957:2-160957:299',
	'160957:302-160957:400',
	'160957:402-160957:465',
	'160957:467-160957:474',
	'160957:478-160957:953',
	'160998:2-160998:252',
	'161008:1-161008:78',
	'161016:2-161016:300',        
        )

)

process.outputs = cms.PSet (
    outputName = cms.string('jetPlots_2011a.root')
)
