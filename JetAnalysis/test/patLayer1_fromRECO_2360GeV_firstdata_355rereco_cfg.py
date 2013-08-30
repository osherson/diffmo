# This is an example PAT configuration showing the usage of PAT on minbias data

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## global tag for data
process.GlobalTag.globaltag = cms.string('GR09_R_35X_V3::All')

#switch off new tau features introduced in 33X to restore 31X defaults
# new feaures: - shrinkingConeTaus instead of fixedCone ones
#              - TaNC discriminants attached for shrinkingConeTaus
#              - default preselection on cleaningLayer1
from PhysicsTools.PatAlgos.tools.tauTools import *
switchTo31Xdefaults(process)


# turn off MC matching for the process
removeMCMatching(process, ['All'])

# add pf met
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')


# get the 900 GeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "2360GeV")

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
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')


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
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/E61F175C-C337-DF11-AC54-00261894393D.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/E0D21B99-C537-DF11-9A0B-0026189438BF.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D0A74E6B-C937-DF11-B25E-002618FDA211.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/C42B2E73-D337-DF11-95D1-001A928116B0.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A827FD5B-C337-DF11-949D-0026189438CF.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A4E4B175-C837-DF11-AE54-00261894396D.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/8AE7496C-C937-DF11-B9A3-002618943863.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/36BFC724-DD37-DF11-960B-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/00CA545F-C337-DF11-AF31-002618FDA25B.root'
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
process.out.fileName = cms.untracked.string('reco_2360GeV_firstdata_355rereco_pat.root')

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
        "keep *_selectedPatJets*_*_*",
        "keep *_patMET*_*_*"
        ]
                                                   )
