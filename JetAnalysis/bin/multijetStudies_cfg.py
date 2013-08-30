import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.jetStudies = cms.PSet(
    # input parameter sets
    jetSrc = cms.InputTag('selectedPatJets'),
    pfJetSrc = cms.InputTag('selectedPatJetsAK5PF'),
    metSrc = cms.InputTag('patMETs'),
    pfMetSrc = cms.InputTag('patMETsPF'),
    useCalo = cms.bool(True),
    minNJets = cms.uint32(4),
    ptMin = cms.double(25.0),
    etaMax = cms.double(3.0),
    htMin = cms.double(0.0),
    ht2Min = cms.double(0.0)
)
process.pfJetStudies = process.jetStudies.clone( useCalo = cms.bool(False)  )


process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')

process.plotParameters = cms.PSet (
    doTracks = cms.bool(False),
    useMC = cms.bool(False),
    runs = cms.vint32([])
)


process.inputs = cms.PSet (
    fileNames = cms.vstring(
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_10_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_11_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_12_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_13_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_14_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_15_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_16_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_17_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_18_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_19_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_1_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_20_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_21_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_22_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_23_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_24_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_25_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_26_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_2_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_3_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_4_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_5_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_6_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_7_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_8_1.root',
'/uscms/home/rappocc/nobackup/TopTagging36xFiles/JetMETTau_early_17may2010/pruning_361_9_1.root'

        ),
        lumisToProcess = cms.untracked.VLuminosityBlockRange(
	'132440:157-132440:378',
	'132596:382-132596:383',
	'132596:447-132596:447',
	'132598:174-132598:176',
	'132599:1-132599:379',
	'132599:381-132599:437',
	'132601:1-132601:207',
	'132601:209-132601:259',
	'132601:261-132601:1107',
	'132602:1-132602:70',
	'132605:1-132605:444',
	'132605:446-132605:522',
	'132605:526-132605:622',
	'132605:624-132605:814',
	'132605:816-132605:829',
	'132605:831-132605:867',
	'132605:896-132605:942',
	'132606:1-132606:26',
	'132656:1-132656:111',
	'132658:1-132658:51',
	'132658:56-132658:120',
	'132658:127-132658:148',
	'132659:1-132659:76',
	'132661:1-132661:116',
	'132662:1-132662:74',
	'132716:220-132716:436',
	'132716:440-132716:487',
	'132716:491-132716:586',
	'132959:1-132959:13',
	'132959:25-132959:53',
	'132959:57-132959:108',
	'132959:112-132959:143',
	'132959:147-132959:276',
	'132959:278-132959:334',
	'132960:1-132960:124',
	'132961:1-132961:222',
	'132961:226-132961:230',
	'132961:237-132961:381',
	'132965:1-132965:68',
	'132968:1-132968:67',
	'132968:75-132968:169',
	'133029:101-133029:115',
	'133029:129-133029:332',
	'133031:1-133031:18',
	'133034:132-133034:287',
	'133035:1-133035:63',
	'133035:67-133035:302',
	'133036:1-133036:222',
	'133046:1-133046:43',
	'133046:45-133046:210',
	'133046:213-133046:227',
	'133046:229-133046:323',
	'133158:65-133158:786',
	'133321:1-133321:383',
	'133446:105-133446:266',
	'133448:1-133448:484',
	'133450:1-133450:329',
	'133450:332-133450:658',
	'133474:1-133474:95',
	'133483:94-133483:159',
	'133483:161-133483:591',
	'133874:166-133874:297',
	'133874:299-133874:721',
	'133874:724-133874:814',
	'133874:817-133874:864',
	'133875:1-133875:20',
	'133875:22-133875:37',
	'133876:1-133876:315',
	'133877:1-133877:77',
	'133877:82-133877:104',
	'133877:113-133877:231',
	'133877:236-133877:294',
	'133877:297-133877:437',
	'133877:439-133877:622',
	'133877:624-133877:853',
	'133877:857-133877:1472',
	'133877:1474-133877:1640',
	'133877:1643-133877:1931',
	'133881:1-133881:71',
	'133881:74-133881:223',
	'133881:225-133881:551',
	'133885:1-133885:132',
	'133885:134-133885:728',
	'133927:1-133927:44',
	'133928:1-133928:645',
        )

)

process.outputs = cms.PSet (
    outputName = cms.string('multijetPlots_1nb.root')
)
