# This is an example PAT configuration showing the usage of PAT on minbias data

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## global tag for data
process.GlobalTag.globaltag = cms.string('GR09_R_34X_V4::All')

#switch off new tau features introduced in 33X to restore 31X defaults
# new feaures: - shrinkingConeTaus instead of fixedCone ones
#              - TaNC discriminants attached for shrinkingConeTaus
#              - default preselection on cleaningLayer1
from PhysicsTools.PatAlgos.tools.tauTools import *
switchTo31Xdefaults(process)


# turn off MC matching for the process
removeMCMatching(process, 'All')

# get the 900 GeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "2360GeV")


# Add PF jets
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'PF',
                 doJTA        = False,
                 doBTagging   = False,
                 jetCorrLabel = ('AK5', 'PF'),
                 doType1MET   = False,
                 doL1Cleaning = False,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = False
                 )


# require physics declared
process.physDecl = cms.EDFilter("PhysDecl",
    applyfilter = cms.untracked.bool(True)
)

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.2)
                                    )


# configure HLT
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND ((40 OR 41) AND NOT (36 OR 37 OR 38 OR 39))')


process.load('Analysis.AnalysisFilters.PVFilter_cff')


# remove the tag infos
process.allLayer1Jets.addTagInfos = False
process.selectedLayer1Jets.cut = cms.string('pt > 10 & abs(eta) < 3.0')
process.allLayer1JetsPF.addTagInfos = False
process.selectedLayer1JetsPF.cut = cms.string('pt > 8 & abs(eta) < 3.0')

# Add the files for runs 123575 and 123596
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()

readFiles.extend( [
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/3CED767C-2307-DF11-9EA4-0024E8768D00.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0007/302132F8-0A07-DF11-978F-00151796D4F0.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0006/F4E59AA1-0107-DF11-965C-00151796C1D0.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0006/6C98880A-0307-DF11-B8A8-00151796C05C.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0006/4ED133AB-0007-DF11-95B9-00151796DA38.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0006/3481B179-0007-DF11-B477-00151796C134.root',
        '/store/data/BeamCommissioning09/MinimumBias/RECO/Jan21stPreProd_336p3_v1/0006/242ADF9B-FE06-DF11-8CDA-00151796C05C.root'
        ] );
process.source.fileNames = readFiles

# let it run
process.p = cms.Path(
    process.hltLevel1GTSeed*
    process.scrapingVeto*
    process.physDecl*
    process.pvFilter*
    process.patDefaultSequence
    )

# rename output file
process.out.fileName = cms.untracked.string('reco_2360GeV_firstdata_336patch3_rereco_pat.root')

# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# process all the events
process.maxEvents.input = 5000
process.options.wantSummary = True

process.out.outputCommands = cms.untracked.vstring([
        #'drop *_genParticles_*_*',
        #'keep *_genEvt_*_*',
        'keep *_offlinePrimaryVertices_*_*',
        'keep *_towerMaker_*_*',
        'keep recoPFCandidates_particleFlow_*_*',
        'keep *_offlineBeamSpot_*_*',
        'keep *_generalTracks_*_*',
        "keep *_selectedLayer1Jets*_*_*"
        ]
                                                   )
