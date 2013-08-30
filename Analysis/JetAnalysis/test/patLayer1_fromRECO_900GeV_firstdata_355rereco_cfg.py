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

'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/FEE4E809-DF37-DF11-B49D-002354EF3BDA.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/FCC08B46-D037-DF11-ADA7-0018F3D09658.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/F6618F59-CE37-DF11-AB18-002618FDA211.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/F4A7228F-C637-DF11-92AB-002354EF3BE6.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/F085B247-CC37-DF11-8904-00261894386E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/EC6ED55B-CE37-DF11-9B3F-001A9281174C.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/E6E15A73-D337-DF11-B702-001BFCDBD11E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/E27CA94B-CF37-DF11-93CA-003048679008.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/E279DB59-CE37-DF11-9DB7-00304867906C.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/E03153F0-FA37-DF11-A1A4-0030486790FE.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/DAF9FD5B-C337-DF11-9AEE-00261894386E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/DA2E5E6B-C937-DF11-9E8D-003048678FE0.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D8D1DA6B-C937-DF11-952D-00304867906C.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D86A849B-C537-DF11-8961-0018F3D09658.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D6AEBF3A-D137-DF11-85CE-0018F3D09658.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D6411A73-D337-DF11-B9B7-0018F3D096A2.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D40F5B91-C637-DF11-9675-00304867C0EA.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D20B4D79-C037-DF11-8320-00261894387D.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D0B3358F-C637-DF11-B51A-0026189438CF.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/D060EA1E-E437-DF11-841C-002618943949.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/CCA0AB4A-CF37-DF11-AF22-00304867918A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/C86F1863-D437-DF11-9C05-0018F3D0970A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/C2CBCA5B-CE37-DF11-8C58-002618943934.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/C219E64E-D637-DF11-836A-001A928116FC.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/C0B40050-D637-DF11-8CC0-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/C0975D5C-D537-DF11-A28E-0018F3D09620.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/BEA22F6F-C937-DF11-B94C-0018F3D0965E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/BAF3CA30-D937-DF11-AD70-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/BAEED58D-C637-DF11-97AD-00261894396D.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/BAB2FA3A-D137-DF11-BA2B-0018F3D09620.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/BAACD08E-C637-DF11-863A-00261894386E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/BA58D66B-C937-DF11-B0EA-002354EF3BE2.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/B8B2416F-C137-DF11-BD64-002618943951.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/B47D8FD7-F337-DF11-98A8-003048679296.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/B2BACD85-C737-DF11-A3D8-002618943962.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/AE4FBA85-C737-DF11-96C0-00261894396D.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/AC393031-D237-DF11-B300-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A8699B34-D837-DF11-AD0A-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A6F8E284-C737-DF11-97DB-00261894389E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A2FD9590-C637-DF11-89B2-003048D15E24.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A272B348-CC37-DF11-8F64-0018F3D0965E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A265D385-C737-DF11-A1EB-00261894386E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A2222175-C837-DF11-8B88-003048678FE0.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/A0CE7867-D437-DF11-9271-001A92810AA0.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/9C642175-C837-DF11-9FBF-00261894389E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/9ACEE52C-DA37-DF11-A354-001A928116C6.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/9A9CCB5C-CE37-DF11-A2D3-00261894383F.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/92200431-D237-DF11-9D71-001A92971B8E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/9009FA6B-C937-DF11-B45C-00261894396D.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/8EE3E250-C437-DF11-9362-003048678A6C.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/8A69535C-D537-DF11-8490-0018F3D09658.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/88B47773-D337-DF11-942D-0018F3D0970A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/84F0F130-D237-DF11-A74B-0018F3D09620.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/82E6446B-C937-DF11-89A4-0026189438F8.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/82BD2A67-D437-DF11-98A5-001A92971B7C.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/8059CD46-CC37-DF11-A392-00304867906C.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/7AC9DA5C-D537-DF11-A42A-001A92971B8E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/7A929A46-CC37-DF11-AA98-0026189438F8.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/7A5E6873-D337-DF11-8016-001A928116FC.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/78A82E85-C737-DF11-A4D1-003048678FE0.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/76B72D1C-DE37-DF11-8248-00261894393E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/6EB4361E-DE37-DF11-A55B-002354EF3BDA.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/6EA2601A-DE37-DF11-B779-001A92810AC4.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/6A57117D-D337-DF11-B1D1-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/6801E35D-CE37-DF11-B3B8-001A928116E2.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/669B1C4F-CF37-DF11-AFC6-001A92971B8C.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/64EBEE86-C737-DF11-8AAC-0018F3D09658.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/64A19159-CE37-DF11-A43D-00304867918A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/64739585-C737-DF11-9965-002618943905.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/5ECBCB08-DF37-DF11-8557-00261894393E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/5CDF7F9A-FF37-DF11-AFC6-003048678F1C.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/526DCE46-CC37-DF11-8087-00304867918A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/504DA634-D837-DF11-9113-0018F3D0970A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/4E2ED066-D437-DF11-B8FC-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/4A151385-C737-DF11-A236-0026189438F8.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/4885C24B-CF37-DF11-AE92-002618FDA211.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/48197831-D237-DF11-8C5F-0018F3D09658.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/46B9B151-CB37-DF11-AD93-002618FDA211.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/42A8126C-C937-DF11-9ED2-00261894386E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/3E86F58E-C637-DF11-9E32-002354EF3BE2.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/3E091D8F-C637-DF11-BE84-002618943875.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/3C78A485-C737-DF11-9E12-002354EF3BE2.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/3AF6235B-CE37-DF11-AA03-0026189438F8.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/3ADA2E29-DA37-DF11-8B07-002618FDA210.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/3A40901C-DE37-DF11-AFBF-0018F3D09660.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/36C7C964-D437-DF11-BEFA-001A92971B8E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/32879575-C837-DF11-8DA1-002354EF3BE2.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/2EEE705C-D537-DF11-A332-0018F3D0970A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/2E9FB68D-C637-DF11-B635-002618FDA211.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/2E723A76-C837-DF11-ADD7-00261894386E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/2A759070-D337-DF11-A64F-0026189438C2.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/2A5D5C42-D037-DF11-AB08-00304867918A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/2A163066-D437-DF11-AF81-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/26480E5B-CE37-DF11-BE6F-002618943964.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/20D0B546-D737-DF11-8A85-001A92971B8E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/1C633F73-D337-DF11-8A19-001A92810AC4.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/1AE70035-D837-DF11-A7BC-001A92811722.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/167BFA73-D337-DF11-9552-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/1496605C-D537-DF11-AA4A-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/124BED3E-D137-DF11-8CD5-0018F3D0965A.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/0CB53885-C737-DF11-9D8E-003048678BF4.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/0C811667-CD37-DF11-8FAB-0018F3D0965E.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/0C07CA4C-D637-DF11-A7E0-0026189438C2.root',
'/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/08A11FA2-FE37-DF11-AA45-0026189438FF.root'

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
process.out.fileName = cms.untracked.string('reco_900GeV_firstdata_355rereco_pat.root')

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
