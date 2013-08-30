# import configurations
import FWCore.ParameterSet.Config as cms

print "About to process"

# define the process
process = cms.Process("TTBSM")

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *

# load the standard PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

# note that you can use a bunch of core tools of PAT 
# to taylor your PAT configuration; for a few examples
# uncomment the following lines

from PhysicsTools.PatAlgos.tools.coreTools import *

# add the trigger information to the configuration
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

print "Setting variables"

outputdir = './'
#set 'algorithm' to 'kt', 'antiki' or 'ca':
algorithm = 'ca'
# this is an ID tag to specify versions, etc
idtag = '_332'
# this is the name of the output file.
outputFileName = outputdir +  'subjetfileter_' + algorithm + '_pat' + idtag + '.root'
print "Output file : " + outputFileName
#set 'runon' to '31x' if you intent to run on data which was reconstructed with CMSSW 31X, and to '33x' if you want
# to run it on 33x (fastsim). -- Jochen
runon = '31x'
#set 'type' to 'ttbar' if you want to run the ttbar gen event
type = 'ttbar'
# set "selectJets" to true if you want to remove events without at least one jet with pt > 20
selectJets = cms.bool(False)

# CATopJets
process.load("RecoJets.Configuration.GenJetParticles_cff")
from RecoJets.JetProducers.ca4GenJets_cfi import ca4GenJets
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

process.ca8GenJets = ca4GenJets.clone( rParam = cms.double(0.8) )


from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')

# switch jet collection to our juets
from PhysicsTools.PatAlgos.tools.jetTools import *

print "About to switch jet collection"

if runon=='31x':
    run33xOn31xMC( process,
               jetSrc = cms.InputTag("antikt5CaloJets"),
               jetIdTag = "antikt5")

if runon=='332rereco':
    run33xOnReRecoMC( process, "ak5GenJets" )




addJetCollection(process, 
                 cms.InputTag('antikt5PFJets'),   
                 'PF',
                 doJTA            = True,            
                 doBTagging       = True,            
                 jetCorrLabel     = ('AK5','PF'),  
                 doType1MET       = False,
                 doL1Cleaning     = False,
                 doL1Counters     = False,
                 genJetCollection = cms.InputTag("antikt5GenJets"),
                 doJetID          = False,
                 jetIdLabel       = "ak5"
                 ) 

addJetCollection(process, 
                 cms.InputTag('caPrunedPFJets'),         # Jet collection; must be already in the event when patLayer0 sequence is executed
                 'PrunedPF',
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
#process.allLayer1Jets.addJetID = cms.bool(False)
# Place appropriate jet cuts (NB: no cut on number of constituents)
process.selectedLayer1Jets.cut = cms.string('pt > 20. & abs(eta) < 5.0')
process.selectedLayer1JetsPF.cut = cms.string('pt > 20. & abs(eta) < 5.0')
process.selectedLayer1JetsPrunedPF.cut = cms.string('pt > 20. & abs(eta) < 2.5')
process.selectedLayer1Muons.cut = cms.string('pt > 15. & abs(eta) < 2.5')
process.selectedLayer1Electrons.cut = cms.string('pt > 15. & abs(eta) < 2.5')
# reduce size of leptons
process.allLayer1Electrons.isoDeposits = cms.PSet()
process.allLayer1Muons.isoDeposits = cms.PSet()
#embed the inner track of the muon: --Jochen
process.allLayer1Muons.embedTrack = cms.bool(True)

# Jets
from PhysicsTools.PatAlgos.tools.jetTools import *


process.allLayer1JetsPrunedPF.embedGenJetMatch = cms.bool(False)
#getJetMCFlavour uses jetFlavourAssociation*, see below
process.allLayer1JetsPrunedPF.getJetMCFlavour = cms.bool(False)
#those two use jetPartonMatch*, see below
process.allLayer1JetsPrunedPF.addGenPartonMatch = cms.bool(True)
process.allLayer1JetsPrunedPF.embedGenPartonMatch = cms.bool(True)
# Add CATopTag info... piggy-backing on b-tag functionality
process.allLayer1JetsPrunedPF.discriminatorSources = cms.VInputTag()
process.allLayer1JetsPrunedPF.addBTagInfo = cms.bool(True)
process.allLayer1JetsPrunedPF.addTagInfos = cms.bool(False)
process.allLayer1JetsPrunedPF.addDiscriminators = cms.bool(False)


print "Done switching jet collection"

## produce ttGenEvent
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

# prune gen particles

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
    "drop  *",
    "keep status = 3", #keeps all particles from the hard matrix element
    "+keep (abs(pdgId) = 11 | abs(pdgId) = 13) & status = 1" #keeps all stable muons and electrons and their (direct) mothers.
    )
)



process.countLayer1Jets.minNumber = cms.uint32(3)
process.countLayer1Muons.minNumber = cms.uint32(1)
process.leadingJet = cms.EDProducer("EtaPtMinCandViewSelector",
                                    src = cms.InputTag("caPrunedPFJets"),
                                    ptMin   = cms.double(150),
                                    etaMin = cms.double(-2.5),
                                    etaMax = cms.double(2.5)
                                    )
process.leadingMuon = cms.EDProducer("EtaPtMinCandViewSelector",
                                     src = cms.InputTag("selectedLayer1Muons"),
                                     ptMin   = cms.double(25),
                                     etaMin = cms.double(-2.1),
                                     etaMax = cms.double(2.1)
                                     )

process.leadingJetFilter = cms.EDFilter("CandViewCountFilter",
                                        src = cms.InputTag("leadingJet"),
                                        minNumber = cms.uint32(1)
                                        )

process.leadingMuonFilter = cms.EDFilter("CandViewCountFilter",
                                         src = cms.InputTag("leadingMuon"),
                                         minNumber = cms.uint32(1)
                                         )

# In addition you usually want to change the following parameters:
#
#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
process.out.fileName = outputFileName
process.options.wantSummary = True       ##  (to suppress the long output at the end of the job)    

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

# extend event content to include PAT objects
process.out.outputCommands.extend(['drop *_genParticles_*_*',
                                   'drop *_generalTracks_*_*',
                                   'keep *_prunedGenParticles_*_*',
                                   'keep *_decaySubset_*_*',
                                   'keep *_initSubset_*_*',
				   'keep *_genEvt_*_*',
                                   'keep *_offlineBeamSpot_*_*',
                                   'keep recoGenJets_ca8GenJets_*_*',
                                   'keep recoPFJets_caPrunedPFJets_*_*',
                                   'keep recoPFCandidates_particleFlow_*_*',
                                   "drop *_cleanLayer1Jets*_*_*",
                                   "keep *_selectedLayer1Jets*_*_*",
                                   'drop *_cleanLayer1Taus_*_*',
                                   'drop *_cleanLayer1Hemispheres_*_*',
                                   'drop *_cleanLayer1Photons_*_*',
				   'drop *_cleanLayer1Electrons_*_*',
				   'drop *_cleanLayer1Muons_*_*',
				   'keep *_selectedLayer1Electrons_*_*',
				   'keep *_selectedLayer1Muons_*_*',
                                   "keep *_layer1METs*_*_*",
                                   'keep GenEventInfoProduct_generator_*_*'
                                   #'keep *_CAJetPartonMatcher_*_*',
                                   #'keep *_CAJetFlavourIdentifier_*_*'
                                   ]
                                  )
process.out.outputCommands += patTriggerEventContent

# drop the meta data for dropped data
process.out.dropMetaData = cms.untracked.string("DROPPED")


# define path 'p'
process.p = cms.Path(process.genJetParticles*
                     process.makeGenEvt *
                     process.prunedGenParticles*
                     process.ca8GenJets*
                     process.caPrunedPFJets*
                     process.patDefaultSequence*
                     process.countLayer1Jets*
                     process.countLayer1Muons*
                     process.leadingJet*
                     process.leadingMuon*
                     process.leadingJetFilter*
                     process.leadingMuonFilter
                     )

if type!='ttbar' :
    process.p.remove( process.makeGenEvt )


process.p.remove( process.layer1MHTs )

process.source.fileNames = [
    # /TTbarJets-madgraph/Summer09-MC_31X_V3-v2/GEN-SIM-RECO
       '/store/mc/Summer09/TTbarJets-madgraph/GEN-SIM-RECO/MC_31X_V3-v2/0013/F2C12AE2-35DD-DE11-A3BB-00145EDD7685.root'
    ]

#On MC, there are often non-unique run and event ids. Safeguard against skipping in that case: --Jochen
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.maxEvents.input = cms.untracked.int32(10)         ##  (e.g. -1 to run on all events)

#override settings in CMSSW/ PhysicsTools/ PatAlgos/ python/ recoLayer0/ photonIsolation_cff.py, where
#it is (wrongly) assumed that the reconstruction process label is called "RECO": for fastsim, it is usually called "HLT". Therefore,
# omit the process label. Maybe this breaks some things for 31x (TODO: someone could check that), so do it only for run on 33x: --Jochen
if runon=='33x':
    process.gamIsoDepositEcalFromHits.ExtractorPSet.barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB")
    process.gamIsoDepositEcalFromHits.ExtractorPSet.endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE")
