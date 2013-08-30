import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('Analysis.SHyFT.shyftAnalysis_cfi')


process.shyftAnalysis.muSrc = cms.InputTag('selectedPatMuons')
process.shyftAnalysis.electronSrc = cms.InputTag('selectedPatElectrons')
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJets')
process.shyftAnalysis.metSrc = cms.InputTag('patMETs')
process.shyftAnalysis.trigSrc = cms.InputTag('patTriggerEvent')

process.shyftAnalysis.cutsToIgnore = cms.vstring( 'Trigger' )

process.shyftAnalysis.muPtMin = cms.double( '10' )
process.shyftAnalysis.elePtMin = cms.double( '10' )
process.shyftAnalysis.muPlusJets = cms.bool(False)
process.shyftAnalysis.ePlusJets = cms.bool(True)

process.inputs = cms.PSet (
    fileNames = cms.vstring(
'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_1.root',
#'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_10.root',
#'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_11.root',
'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_2.root',
'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_3.root',
'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_4.root',
'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_5.root',
'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_6.root',
'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_7.root'
#'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_8.root',
#'/Users/rappocc/data_local/MinimumBias/pat_7TeV_minbias_data_v2/reco_7TeV_firstdata_356_pat_9.root'
        )
)

process.outputs = cms.PSet (
    outputName = cms.string('shyftPlots_minbias_data.root')
)
