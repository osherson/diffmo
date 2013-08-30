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
switchJECSet( process, "900GeV")


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
process.selectedLayer1JetsPF.cut = cms.string('pt > 10 & abs(eta) < 3.0')

# Add the files for runs 123575 and 123596
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()

readFiles.extend( [
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0013/CC989CAE-6009-DF11-8137-002618943900.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/FEE02EB8-9608-DF11-9B93-002618943811.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/FE896A77-A208-DF11-9513-002618943868.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/ECE91120-9B08-DF11-B0CA-002618943861.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/EC73271C-9B08-DF11-A3C0-002618943831.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/E41FCEFF-9908-DF11-B5BB-00248C0BE014.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/D4C5A711-9B08-DF11-A9E4-002618FDA259.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/C4ADB497-A308-DF11-A8DA-00261894380B.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/BA843D72-A008-DF11-AF73-002618943905.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/B6CBA256-A008-DF11-8588-002354EF3BDA.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/B65AB831-9E08-DF11-9D9F-00261894390A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/AA7BBAF6-9908-DF11-97A0-002354EF3BDC.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/A0E5D8FA-9908-DF11-AD84-00261894396F.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/92895BB4-A408-DF11-B543-002618943832.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/902DBCF4-9808-DF11-A041-002618943983.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/76D9FA1C-9B08-DF11-837C-002354EF3BDA.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/7660AA77-A008-DF11-9A33-00261894387A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/74FBEE2C-9F08-DF11-9FC3-002618943861.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/6A883E77-A008-DF11-9F46-00261894389E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/4A226F5E-A008-DF11-8053-002618943833.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/3A3A62F2-9808-DF11-8FEC-002618943833.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/32F23603-9A08-DF11-906D-002618943923.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/3057322C-9F08-DF11-8FB7-00261894385A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/1834C169-9F08-DF11-846F-002618943875.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/0E991A78-A008-DF11-A88B-00261894385A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/0E7B595C-A008-DF11-9EF9-002618943838.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/0C80C376-A008-DF11-ADB4-002618943964.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/04CA4078-A308-DF11-BAF7-0026189438DE.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Jan23ReReco-v1/0012/047FA102-9A08-DF11-BAAB-0026189438FC.root'
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
process.out.fileName = cms.untracked.string('reco_900GeV_firstdata_336patch3_rereco_pat.root')

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
