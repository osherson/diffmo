import FWCore.ParameterSet.Config as cms

process = cms.Process("TTBSM")


###############################
####### Parameters ############
###############################
#from FWCore.ParameterSet.VarParsing import VarParsing
#options = VarParsing ('python')




#options.register ('sample',
#                  'Jet',
#                  VarParsing.multiplicity.singleton,
#                  VarParsing.varType.string,
#                  "Sample to use.")


#options.register ('writeLiteJets',
#                  0,
#                  VarParsing.multiplicity.singleton,
#                  VarParsing.varType.int,
#                  "Write out edm-tuple dumps of 4-vectors of jets.")


#options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )



## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'FT_53_V6_AN1::All'
process.load("Configuration.StandardSequences.MagneticField_cff")

from EventFilter.HcalRawToDigi.hcallasereventfilter2012_cfi import *
process.hcallasereventfilter2012=cms.EDFilter("HcalLaserEventFilter2012",
                                      EventList = cms.untracked.vstring(['AllBadHCALLaser.txt']),
                                      verbose   = cms.untracked.bool(False),
                                      prefix    = cms.untracked.string(""),
                                      minrun    = cms.untracked.int32(-1),
                                      maxrun    = cms.untracked.int32(-1),
                                      WriteBadToFile = cms.untracked.bool(True),
                                      forceFilterTrue=cms.untracked.bool(False) 
                                      )





from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from Analysis.BoostedTopAnalysis.CATopTagParams_cfi import caTopTagParams
from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import boostedTopWTagParams


#'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_727_4_dOP.root'                                    

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
#                                    'file:ttbsm_42x_mc.root'
#'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/guofan/HT/ttbsm_v5_Run2011A-May10ReReco-v1/66893cf1a11d0e3066e5097c4fdc37a6/ttbsm_42x_data_74_1_PRV.root',
#'/store/user/lpctlbsm/jpilot/Jet/Run2012A-23May2012-v2_TLBSM_53x_v1/e0d656cdbc16ec465a5494ca10c7261f/ttbsm_53x_data_10_1_KBI.root',
# 'file:/uscms/home/pilot/3DayLifetime/TOBTEC/tlbsm_53x_v2_data.root'
'/store/user/lpctlbsm/yxin/JetHT/Run2012D-22Jan2013-v1_TLBSM_53x_v3/45cbb6c27540456f7aaf244304c73a89/tlbsm_53x_v3_data_727_4_dOP.root'                                    
                                   )
                                )
    
mytrigs1 = ['HLT_Jet240_v1',
                'HLT_Jet300_v*',
                'HLT_Jet370_v*']
mytrigs2 = ['HLT_HT750_v*']
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltSelection1 = hltHighLevel.clone(TriggerResultsTag = 'TriggerResults::HLT', HLTPaths = mytrigs1)
process.hltSelection1.throw = False
process.hltSelection2 = hltHighLevel.clone(TriggerResultsTag = 'TriggerResults::HLT', HLTPaths = mytrigs2)
process.hltSelection2.throw = False
process.hltSelection = cms.Sequence( process.hltSelection2 )

    
# run the trigger on the fly
process.load('PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff')
#process.patTrigger.onlyStandAlone = cms.bool(True)

#process.load("HLTrigger.HLTcore.hltEventAnalyzerAOD_cfi")
#process.hltEventAnalyzerAOD.triggerName = cms.string( 'HLT_PFJet320_v4' )

myAnaTrigs = [
    'HLT_HT750_v1',
    'HLT_HT750_v2',
    'HLT_HT750_v3',
    'HLT_HT750_v4',
    'HLT_HT750_v5',
    'HLT_HT750_v6',
    'HLT_HT750_v7',
    'HLT_HT750_v8',
    'HLT_HT750_v9',
    'HLT_HT750_v10',
    'HLT_HT750_v11',
    'HLT_HT750_v12'
    ]
process.ttbsmAna = cms.EDFilter('TTBSMProducer',
                                wTagSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
                                topTagSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
                                trigSrc = cms.InputTag('hltTriggerSummaryAOD'),
                                rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
                                pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
                                genJetsSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
				trigs = cms.vstring(
                                    myAnaTrigs
                                    ),
                                topTagParams = caTopTagParams.clone(
                                    tagName = cms.string('CATop')
                                    ),
                                wTagParams = boostedTopWTagParams.clone(
                                    yCut = cms.double(0.0)
                                    ),
                                readTrig = cms.bool(True),
                                jetScale = cms.double(0.0),   #
                                jetPtSmear = cms.double(-99.0), # note these three are fractional
                                jetEtaSmear = cms.double(0.0),#
                                jecPayloads = cms.vstring([
                                    'FT_53_V10_AN3_L1FastJet_AK7PFchs.txt',
                                    'FT_53_V10_AN3_L2Relative_AK7PFchs.txt',
                                    'FT_53_V10_AN3_L3Absolute_AK7PFchs.txt',
                                    'FT_53_V10_AN3_L2L3Residual_AK7PFchs.txt',
                                    'FT_53_V10_AN3_Uncertainty_AK7PFchs.txt'
                                    ]),
                                pdfSet = cms.string("")
)


process.triggerMatcher = cms.EDProducer(
    "PATTriggerMatcherDRLessByR",
    src = cms.InputTag('goodPatJetsCA8TopTagPF'),
    matched = cms.InputTag( "patTrigger" ),          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
    matchedCuts = cms.string( 'path( "HLT_PFJet320_v*" )' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),        # only one match per trigger object
    resolveByMatchQuality = cms.bool( True ),
)

process.ak5PFJetsLite = cms.EDProducer(
    "CandViewNtpProducer", 
    src = cms.InputTag("goodPatJetsPFlow"),
    lazyParser = cms.untracked.bool(True),
    eventInfo = cms.untracked.bool(False),
    variables = cms.VPSet(
        cms.PSet(
            tag = cms.untracked.string("mass"),
            quantity = cms.untracked.string("mass")
            ),
        cms.PSet(
            tag = cms.untracked.string("pt"),
            quantity = cms.untracked.string("pt")
            ),
        cms.PSet(
            tag = cms.untracked.string("eta"),
            quantity = cms.untracked.string("eta")
            ),
        cms.PSet(
            tag = cms.untracked.string("phi"),
            quantity = cms.untracked.string("phi")
            ),
        cms.PSet(
            tag = cms.untracked.string("ssvhe"),
            quantity = cms.untracked.string("bDiscriminator('simpleSecondaryVertexHighEffBJetTags')")
            ),
        cms.PSet(
            tag = cms.untracked.string("tche"),
            quantity = cms.untracked.string("bDiscriminator('trackCountingHighEffBJetTags')")
            ),
        cms.PSet(
            tag = cms.untracked.string("tchp"),
            quantity = cms.untracked.string("bDiscriminator('trackCountingHighPurBJetTags')")
            ),
        cms.PSet(
            tag = cms.untracked.string("jetArea"),
            quantity = cms.untracked.string("jetArea")
            ),
        )  
    )

process.ca8PFJetsPrunedLite = process.ak5PFJetsLite.clone(
    src = cms.InputTag("goodPatJetsCA8PrunedPF")
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100

print 'Making the path'

process.p = cms.Path(
    process.hltSelection*
    #process.hltEventAnalyzerAOD*
    process.patTriggerDefaultSequence*
    #process.hcallasereventfilter2012*
    #process.patTriggerMatchEmbedderDefaultSequence*
    #process.patTrigger*
    #process.triggerMatcher*
    process.ttbsmAna
    )

#if options.writeLiteJets == 1 :
#    process.p *= cms.Sequence(process.ak5PFJetsLite*process.ca8PFJetsPrunedLite)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("ttbsm_ultraslim2.root"),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_ttbsmAna_*_*',
                                                                      #'keep *_patTrigger_*_*',
								      'keep *_*Lite*_*_*'
                                                                      #'keep *_*hlt*_*_*'
								      #, 'keep *_goodPatJetsCA8PrunedPF_*_*'
                                                                      #, 'keep *_goodPatJetsCATopTagPF_*_*'
                                                                      #, 'keep recoPFJets_*_*_*'
                                                                      ) 
                               )
process.outpath = cms.EndPath(process.out)

#process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
process.MessageLogger.suppressWarning.append('patTrigger')
process.MessageLogger.suppressWarning.append('patTrigger')
process.MessageLogger.cerr.FwkJob.limit=1
process.MessageLogger.cerr.ERROR = cms.untracked.PSet( limit = cms.untracked.int32(0) )
#process.MessageLogger.destinations.remove( 'errors' )
