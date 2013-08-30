import FWCore.ParameterSet.Config as cms

process = cms.Process("TTBSM")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
       fileNames = cms.untracked.vstring(
       #'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/srappocc/TTJets_TuneZ2_7TeV-madgraph-tauola/ttbsm_v8_Summer11-PU_S4_START42_V11-v1/5c91b0700768331a44f51c8a9892d391/ttbsm_42x_mc_1_2_RlY.root'
       #'dcap:///pnfs/cms/WAX/11/store/user/lpctlbsm/weizou/WprimeToTBbar_M-2000_TuneZ2_7TeV-pythia6/ttbsm_v8_Summer11-PU_S4_-START42_V11-v1/2bcf344afee8f9cb5489a05cc32c05cf/ttbsm_42x_mc_1_1_J1v.root'
#    	'/store/results/B2G/TT_8TeV-mcatnlo/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v2-c04f3b4fa74c8266c913b71e0c74901d/TT_8TeV-mcatnlo/USER/StoreResults-Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v2-c04f3b4fa74c8266c913b71e0c74901d/0000/FEBBE69C-1942-E211-9959-002354EF3BE2.root',
        '/store/user/lpctlbsm/jdolen/TT_CT10_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2_TLBSM_53x_v3_bugfix_v1/99bd99199697666ff01397dad5652e9e/tlbsm_53x_v3_mc_1912_2_rsE.root'
				)
                                )



## Geometry and Detector Conditions (needed for a few patTuple production steps)
#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR_R_42_V12::All'
#process.load("Configuration.StandardSequences.MagneticField_cff")


from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from Analysis.BoostedTopAnalysis.CATopTagParams_cfi import caTopTagParams
from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import boostedTopWTagParams

process.hltHighLevel = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths = cms.vstring('HLT_HT750_v*'),           # provide list of HLT paths (or patterns) you want
    eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
    andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
    throw = cms.bool(True)    # throw exception on unknown path names
)



process.ttbsmAna = cms.EDFilter('TTBSMProducer',
                                wTagSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
                                topTagSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
                                trigSrc = cms.InputTag('hltTriggerSummaryAOD'),
                                rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
                                genJetsSrc = cms.InputTag('ca8GenJetsNoNu'),
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
                                readTrig = cms.bool(False),
                                trigs = cms.vstring(
                                    ''
                                    ),
                                topTagParams = caTopTagParams.clone(
                                    tagName = cms.string('CATop')
                                    ),
                                wTagParams = boostedTopWTagParams.clone(
                                    yCut = cms.double(0.0)
                                    ),
                                jetScale = cms.double(0.0),   #
                                jetPtSmear = cms.double(0.1), # note these three are fractional
                                jetEtaSmear = cms.double(0.1),#
                                jecPayloads = cms.vstring([
                                    'FT_53_V10_AN3_L1FastJet_AK7PFchs.txt',
                                    'FT_53_V10_AN3_L2Relative_AK7PFchs.txt',
                                    'FT_53_V10_AN3_L3Absolute_AK7PFchs.txt',
                                    #'FT_53_V10_AN3_L2L3Residual_AK7PFchs.txt',
                                    'FT_53_V10_AN3_Uncertainty_AK7PFchs.txt'
                                    ]),
                                pdfSet = cms.string("")

)


process.ttbsmAnaScaleDown = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetScale = cms.double(-0.05)
    )
process.ttbsmAnaScaleUp = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetScale = cms.double(0.05)
    )
process.ttbsmAnaPtSmearDown = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetPtSmear = cms.double(0.0)
    )
process.ttbsmAnaPtSmearUp = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetPtSmear = cms.double(0.2)
    )
process.ttbsmAnaEtaSmearDown = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetEtaSmear = cms.double(0.0)
    )
process.ttbsmAnaEtaSmearUp = process.ttbsmAna.clone(
    pdfSet = cms.string(""),
    jetEtaSmear = cms.double(0.2)
    )


# Produce PDF weights (maximum is 3)
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
      # Fix POWHEG if buggy (this PDF set will also appear on output, 
      # so only two more PDF sets can be added in PdfSetNames if not "")
      FixPOWHEG = cms.untracked.string(""),
      GenTag = cms.untracked.InputTag("prunedGenParticles"),
      PdfInfoTag = cms.untracked.InputTag("generator"),
      PdfSetNames = cms.untracked.vstring( "cteq66.LHgrid"
                                          ,"MRST2006nnlo.LHgrid"
                                           )
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

print 'Making the path'

process.p = cms.Path(
    #process.patTriggerDefaultSequence*
    #process.hltHighLevel
    process.ttbsmAna
    *process.ttbsmAnaScaleDown
    *process.ttbsmAnaScaleUp
    *process.ttbsmAnaPtSmearDown
    *process.ttbsmAnaPtSmearUp
    *process.ttbsmAnaEtaSmearDown
    *process.ttbsmAnaEtaSmearUp
    #*process.pdfWeights
    )


process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("ttbsm_ultraslim.root"),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_ttbsmAna*_*_*'
                                                                      #, 'keep *_goodPatJetsCA8PrunedPF_*_*'
                                                                      #, 'keep *_goodPatJetsCATopTagPF_*_*'
                                                                      #, 'keep recoPFJets_*_*_*'
                                                                      ) 
                               )
process.outpath = cms.EndPath(process.out)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
