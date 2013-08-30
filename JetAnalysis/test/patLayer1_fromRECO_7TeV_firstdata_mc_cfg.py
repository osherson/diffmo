# This is an example PAT configuration showing the usage of PAT on minbias data

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## global tag for data
process.GlobalTag.globaltag = cms.string('START3X_V25::All')

# turn off MC matching for the process
#removeMCMatching(process, ['All'])

# add pf met
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')



# get the 900 GeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

# run ak5 gen jets
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
run33xOnReRecoMC( process, "ak5GenJets")

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
process.patJets.embedCaloTowers = True
process.selectedPatJets.cut = cms.string('pt > 10 & abs(eta) < 3.0')
process.patJetsAK5PF.addTagInfos = False
process.patJetsAK5PF.embedPFCandidates = True
process.selectedPatJetsAK5PF.cut = cms.string('pt > 8 & abs(eta) < 3.0')


# Add the files for runs 123575 and 123596
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()

readFiles.extend( [
       '/store/mc/Summer09/MinBias/GEN-SIM-RECO/START3X_V25_preproduction-v2/0026/FEDECD35-302C-DF11-AB36-0018F3D095EC.root',
       '/store/mc/Summer09/MinBias/GEN-SIM-RECO/START3X_V25_preproduction-v2/0026/FE4BD102-2F2C-DF11-8153-002354EF3BDB.root',
       '/store/mc/Summer09/MinBias/GEN-SIM-RECO/START3X_V25_preproduction-v2/0026/FC0049EC-2E2C-DF11-A722-001A92971AA8.root',
       '/store/mc/Summer09/MinBias/GEN-SIM-RECO/START3X_V25_preproduction-v2/0026/FAC2E7F6-2E2C-DF11-9D47-001A92971B38.root',
       '/store/mc/Summer09/MinBias/GEN-SIM-RECO/START3X_V25_preproduction-v2/0026/FAB0E50E-302C-DF11-9B9F-0018F3D0965A.root',
       '/store/mc/Summer09/MinBias/GEN-SIM-RECO/START3X_V25_preproduction-v2/0026/FA17E007-302C-DF11-A4B6-001A92810ADE.root',
       '/store/mc/Summer09/MinBias/GEN-SIM-RECO/START3X_V25_preproduction-v2/0026/F848F40A-302C-DF11-8E27-001731AF68ED.root'
        ] );
process.source.fileNames = readFiles

# let it run
process.p = cms.Path(
#    process.hltLevel1GTSeed*
    process.scrapingVeto*
#    process.physDecl*
    process.pvFilter*
    process.patDefaultSequence
    )

# rename output file
process.out.fileName = cms.untracked.string('reco_7TeV_firstdata_354_pat.root')

# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# process all the events
process.maxEvents.input = 5000
process.options.wantSummary = True

process.out.outputCommands = cms.untracked.vstring([
        #'drop *_genParticles_*_*',
        #'keep *_genEvt_*_*',
        'keep *_offlinePrimaryVertices_*_*',
        #'keep *_towerMaker_*_*',
        #'keep recoPFCandidates_particleFlow_*_*',
        'keep *_offlineBeamSpot_*_*',
        #'keep *_generalTracks_*_*',
        "keep *_selectedPatJets*_*_*",
        "keep *_patMETs*_*_*"
        ]
                                                   )
