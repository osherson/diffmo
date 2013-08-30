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
removeMCMatching(process, ['All'])

# get the 900 GeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "900GeV")


# Add PF jets
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5', 'PF',
                 doJTA        = False,
                 doBTagging   = False,
                 jetCorrLabel = ('AK5','PF'),
                 doType1MET   = False,
                 doL1Cleaning = False,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = False,
                 jetIdLabel   = "ak5"
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
process.patJets.addTagInfos = False
process.selectedPatJets.cut = cms.string('pt > 10 & abs(eta) < 3.0')
process.patJetsAK5PF.addTagInfos = False
process.selectedPatJetsAK5PF.cut = cms.string('pt > 8 & abs(eta) < 3.0')

# Add the files for runs 123575 and 123596
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()

readFiles.extend( [
'/store/data/BeamCommissioning09/MinimumBias/RECO/18thFebPreProd_351p1-v3/0000/0824ACD9-C71D-DF11-9A6E-00237DA1AC2A.root'
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
process.out.fileName = cms.untracked.string('reco_900GeV_firstdata_351patch1_18feb_rereco_pat.root')

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
        "keep *_selectedPatJets*_*_*"
        ]
                                                   )
