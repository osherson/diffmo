import FWCore.ParameterSet.Config as cms

process = cms.Process("TTBSM")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
       fileNames = cms.untracked.vstring(
#       'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/jdolen/Zprime_M1000GeV_W10GeV-madgraph/ttbsm_v8_Summer11-PU_S4_-START42_V11-v2/87037ef7c828ea57e128f1ace23a632e/ttbsm_42x_mc_10_1_Mmd.root'
#       'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/jdolen/Zprime_M2000GeV_W20GeV-madgraph/ttbsm_v8_Summer11-PU_S4_-START42_V11-v2/87037ef7c828ea57e128f1ace23a632e/ttbsm_42x_mc_10_1_rPv.root'

#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_10_1_nSd.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_11_1_wFr.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_12_1_1ZE.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_13_1_5NH.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_14_1_QlG.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_15_1_uUO.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_16_1_Rhh.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_17_1_BBJ.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_18_1_J74.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_19_1_23T.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_1_1_nch.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_20_1_UZ9.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_21_1_xQq.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_22_1_BDk.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_2_1_mhw.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_3_1_pnP.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_4_1_Zx8.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_5_1_HCP.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_6_1_9Xu.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_7_1_XhY.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_8_1_9ga.root',
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1000GeV_W10GeV-madgraph/prod_2011_09_29_16_34_56/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_9_1_6dI.root'


'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_10_1_0Ft.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_11_1_qAA.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_12_1_2x8.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_13_1_9MA.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_14_1_M0j.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_15_1_Wa6.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_16_1_DFB.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_17_1_6i1.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_18_1_MM2.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_1_1_uOH.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_2_1_5wg.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_3_1_CEc.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_4_1_z44.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_5_1_w0T.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_6_1_do7.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_7_1_saS.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_8_1_Zfp.root',
'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/samvel/Zprime_M1500GeV_W15GeV-madgraph/prod_2011_10_01_21_37_43/bf57a985b107a689982b667a3f2f23c7/ttbsm_42x_mc_9_1_uaU.root'


                                
#   'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/jdolen/Zprime_M1500GeV_W15GeV-madgraph/ttbsm_v8_Summer11-PU_S4_-START42_V11-v2/87037ef7c828ea57e128f1ace23a632e/ttbsm_42x_mc_10_1_m45.root'
       #'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/weizou/WprimeToTBbar_M-2000_TuneZ2_7TeV-pythia6/ttbsm_v8_Summer11-PU_S4_-START42_V11-v1/2bcf344afee8f9cb5489a05cc32c05cf/ttbsm_42x_mc_1_1_J1v.root'
                                    )
                                )



## Geometry and Detector Conditions (needed for a few patTuple production steps)
#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR_R_42_V12::All'
#process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("m1500_w15_trigplots_jet370.root")
                                   )


process.boostedTopTrigger = cms.EDAnalyzer('BoostedTopTrigger',
                                           jetTag1 = cms.untracked.InputTag("goodPatJetsCATopTagPF")
                                           , jetTag2 = cms.untracked.InputTag("goodPatJetsCA8PrunedPF")
                                           , TargetTrigger = cms.untracked.string("HLT_Jet240_v1")
                                           , threshold = cms.untracked.double(370)
                                           #, TargetTrigger = cms.untracked.string("HLT_HT550_v2")
                                           #, Verbose = cms.untracked.bool(True)
                                         )




process.MessageLogger.cerr.FwkReport.reportEvery = 10000

print 'Making the path'

process.p = cms.Path(
    #process.patTriggerDefaultSequence*
    #process.hltSelection*
    process.boostedTopTrigger
    #*process.pdfWeights
    )


## process.out = cms.OutputModule("PoolOutputModule",
##                                fileName = cms.untracked.string("ttbsm_ultraslim.root"),
##                                SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
##                                outputCommands = cms.untracked.vstring('drop *',
##                                                                       'keep *_ttbsmAna*_*_*'
##                                                                       ,'keep *_hltTriggerSummaryAOD_*_*'
##                                                                       #, 'keep *_goodPatJetsCA8PrunedPF_*_*'
##                                                                       #, 'keep *_goodPatJetsCATopTagPF_*_*'
##                                                                       #, 'keep recoPFJets_*_*_*'
##                                                                       ) 
##                                )
## process.outpath = cms.EndPath(process.out)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
##process.out.dropMetaData = cms.untracked.string("DROPPED")
