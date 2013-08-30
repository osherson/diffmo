import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.jetStudies = cms.PSet(
    # input parameter sets
    jetSrc = cms.InputTag('selectedPatJets'),
    pfJetSrc = cms.InputTag('selectedPatJetsAK5PF'),
    metSrc = cms.InputTag('patMETs'),
    pfMetSrc = cms.InputTag('patMETsPF'),
    useCalo = cms.bool(True),
    minNJets = cms.int32(2)
)
process.pfJetStudies = process.jetStudies.clone( useCalo = cms.bool(False) )


process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')

process.plotParameters = cms.PSet (
    doTracks = cms.bool(False),
    useMC = cms.bool(False),
    runs = cms.vint32([])
)


process.inputs = cms.PSet (
    fileNames = cms.vstring(
'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_1.root',
#'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_10.root',
#'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_11.root',
'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_2.root',
'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_3.root',
'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_4.root',
'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_5.root',
'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_6.root',
'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_7.root'
#'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_8.root',
#'/uscms/home/rappocc/nobackup/data/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_9.root',

        )
)

process.outputs = cms.PSet (
    outputName = cms.string('multijetPlots.root')
)
 
