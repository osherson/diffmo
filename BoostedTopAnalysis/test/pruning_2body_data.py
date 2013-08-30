print "About to process"

from PhysicsTools.PatAlgos.patTemplate_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## global tag for data
process.GlobalTag.globaltag = cms.string('GR10_P_V2::All')

# add pf met
from PhysicsTools.PatAlgos.tools.metTools import *
removeMCMatching(process, ['All'])
addPfMET(process, 'PF')


# get the 900 GeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

# run ak5 gen jets
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *


# add the trigger information to the configuration
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

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

# switch on PAT trigger
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger( process )

# PV Filter
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(15), 
                                           maxd0 = cms.double(2) 
                                           )



# Pruned jets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters

from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *

process.caPrunedPFJets = cms.EDProducer(
    "SubJetProducer",
    PFJetParameters,
    AnomalousCellParameters,
    SubJetParameters,
    jetAlgorithm = cms.string("CambridgeAachen"),
    rParam = SubJetParameters.jetSize
    )
process.caPrunedPFJets.nSubjets = cms.int32(2) 
process.caPrunedPFJets.inputEMin = cms.double(1.0)
process.caPrunedPFJets.jetCollInstanceName = cms.string("subjets")

from RecoJets.Configuration.RecoGenJets_cff import ca4GenJets
process.ca8GenJets = ca4GenJets.clone( rParam = cms.double(0.8) )


# switch jet collection to our juets
from PhysicsTools.PatAlgos.tools.jetTools import *

print "About to add jet collections"


# Add PF jets
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5', 'PF',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5','PF'),
                 doType1MET   = False,
                 doL1Cleaning = False,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = False
                 )


addJetCollection(process, 
                 cms.InputTag('caPrunedPFJets'),         # Jet collection; must be already in the event when patLayer0 sequence is executed
                 'CA8', 'PrunedPF',
                 doJTA=False,            # Run Jet-Track association & JetCharge
                 doBTagging=False,       # Run b-tagging
                 jetCorrLabel=('KT6', 'PF'),   # example jet correction name; set to None for no JEC
                 doType1MET=False,
                 doL1Cleaning=False,
                 doL1Counters=False,
                 genJetCollection = cms.InputTag("ca8GenJets"),
                 doJetID = False,
                 jetIdLabel = "ak5"
                 )


#jet ID does not work reliably in PAT for now (PAT as of 2009-10-20), so switch it off -- Jochen:
# Place appropriate jet cuts (NB: no cut on number of constituents)
process.selectedPatJets.cut = cms.string('pt > 20. & abs(eta) < 3.0')
process.selectedPatJetsAK5PF.cut = cms.string('pt > 20. & abs(eta) < 3.0')
process.selectedPatJetsCA8PrunedPF.cut = cms.string('pt > 20. & abs(eta) < 2.5')
process.selectedPatMuons.cut = cms.string('pt > 15. & abs(eta) < 2.5')
process.selectedPatElectrons.cut = cms.string('pt > 15. & abs(eta) < 2.5')
# reduce size of leptons
process.patElectrons.isoDeposits = cms.PSet()
process.patMuons.isoDeposits = cms.PSet()
#embed the inner track of the muon: --Jochen
process.patMuons.embedTrack = cms.bool(True)
process.patElectrons.embedTrack = cms.bool(True)

# Jets
from PhysicsTools.PatAlgos.tools.jetTools import *


# Add CATopTag info... piggy-backing on b-tag functionality
process.patJetsCA8PrunedPF.addTagInfos = cms.bool(False)
process.patJets.addTagInfos = cms.bool(False)
process.patJetsAK5PF.addTagInfos = cms.bool(False)


print "Done switching jet collection"

## produce ttGenEvent
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

process.countPatJets.minNumber = cms.uint32(2)

process.leadingJet = cms.EDProducer("EtaPtMinCandViewSelector",
                                    src = cms.InputTag("caPrunedPFJets"),
                                    ptMin   = cms.double(100),
                                    etaMin = cms.double(-2.5),
                                    etaMax = cms.double(2.5)
                                    )

process.leadingJetFilter = cms.EDFilter("CandViewCountFilter",
                                        src = cms.InputTag("leadingJet"),
                                        minNumber = cms.uint32(1)
                                        )


process.source.fileNames = ['/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/440/0AA7C390-0F3C-DF11-BD65-000423D998BA.root']


process.p = cms.Path(
    process.hltLevel1GTSeed*
    process.scrapingVeto*
    process.physDecl*
    process.primaryVertexFilter*
    process.caPrunedPFJets*
    process.patDefaultSequence*
    process.countPatJets*
    process.leadingJet*
    process.leadingJetFilter
    )

# rename output file
process.out.fileName = cms.untracked.string('reco_7TeV_pruneddata_356_pat.root')

# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# process events
process.maxEvents.input = 1000
process.options.wantSummary = True


# drop the meta data for dropped data
process.out.dropMetaData = cms.untracked.string("DROPPED")

#On MC, there are often non-unique run and event ids. Safeguard against skipping in that case: --Jochen
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# event content
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
process.out.outputCommands = patEventContentNoCleaning
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += [
        'keep recoPFCandidates_particleFlow_*_*',
        'keep recoPFJets_caPrunedPFJets_*_*'
        ]

