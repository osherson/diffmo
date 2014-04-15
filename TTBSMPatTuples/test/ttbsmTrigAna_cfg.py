import FWCore.ParameterSet.Config as cms

process = cms.Process("Trig")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
       fileNames = cms.untracked.vstring(
#'/store/user/lpctlbsm/jpilot/RSGluonToTT_M-2000_Tune4C_8TeV-pythia8/RSGluonToTT_M-2000_Tune4C_8TeV-pythia8/fe5dcf8cf2a24180bf030f68a7d97dda/ttbsm_53x_mc_9_1_2tL.root',

'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1000_2_yD8.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1001_2_Obb.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1002_2_npB.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1003_1_ELD.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1004_1_UCh.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1005_1_lgE.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1006_1_nw3.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1007_3_hDM.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1008_7_ZFB.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1009_2_tbj.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_100_1_cQh.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1010_2_aJU.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1011_2_6f3.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1012_1_PDH.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1013_2_iZO.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1014_2_b3q.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1015_2_ISJ.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1016_2_zhY.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1017_1_e7M.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1018_7_NjR.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1019_2_hee.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_101_1_aps.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1020_2_fVm.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1021_3_uJa.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1022_8_u1U.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1023_5_GfB.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1024_1_shA.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1025_1_4BD.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1026_5_Q0z.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1027_2_6rG.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1028_3_TnR.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1029_7_D2A.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_102_1_1JD.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1030_2_QFP.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1031_1_lM1.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1032_2_IU2.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1033_1_94h.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1034_2_Vqk.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1035_2_l1t.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1036_2_mB9.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1037_2_zIf.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1038_2_Fuh.root',
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_1039_8_SUH.root'


)
                                )



## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'START53_V7F::All'
process.load("Configuration.StandardSequences.MagneticField_cff")
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("trigger_output.root")
                                   )

#process.load("HLTrigger.HLTcore.hltEventAnalyzerAOD_cfi")
#process.hltEventAnalyzerAOD.triggerName = cms.string( 'HLT_HT750_v*' )

process.hltHighLevel = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths = cms.vstring('HLT_PFJet320_v*'),           # provide list of HLT paths (or patterns) you want
    eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
    andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
    throw = cms.bool(True)    # throw exception on unknown path names
)


process.boostedTopTrigger = cms.EDAnalyzer('BoostedTopTrigger',
                                           jetTag1 = cms.untracked.InputTag("goodPatJetsCATopTagPFPacked")
                                           , jetTag2 = cms.untracked.InputTag("goodPatJetsCA8PrunedPFPacked")
                                           , TargetTrigger = cms.untracked.string("HLT_HT750")
                                           , threshold = cms.untracked.double(0)
                                           #, TargetTrigger = cms.untracked.string("HLT_HT550")
                                           , Verbose = cms.untracked.bool(False)
                                         )




process.MessageLogger.cerr.FwkReport.reportEvery = 10000

print 'Making the path'

process.p = cms.Path(
    #process.patTriggerDefaultSequence*
    process.hltHighLevel
    *process.boostedTopTrigger
    #*process.pdfWeights
    )


process.out = cms.OutputModule("PoolOutputModule",
                                fileName = cms.untracked.string("ttbsm_ultraslim.root"),
                                SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                                outputCommands = cms.untracked.vstring('drop *'
                                                                       #'keep *_ttbsmAna*_*_*'
                                                                       #,'keep *_hltTriggerSummaryAOD_*_*'
                                                                       #, 'keep *_goodPatJetsCA8PrunedPF_*_*'
                                                                       #, 'keep *_goodPatJetsCATopTagPF_*_*'
                                                                       #, 'keep recoPFJets_*_*_*'
                                                                      ) 
                                )
process.outpath = cms.EndPath(process.out)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
##process.out.dropMetaData = cms.untracked.string("DROPPED")
