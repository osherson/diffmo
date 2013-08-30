import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('Analysis.SHyFT.shyftAnalysis_cfi')


process.inputs = cms.PSet (
    maxEvents = cms.int32(-1),
    fileNames = cms.vstring(
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_1.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_10.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_11.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_12.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_13.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_14.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_15.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_16.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_17.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_18.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_19.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_2.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_20.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_21.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_22.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_23.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_24.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_25.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_26.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_27.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_28.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_29.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_3.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_30.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_31.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_32.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_33.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_34.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_35.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_36.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_37.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_38.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_39.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_4.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_40.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_41.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_42.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_43.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_44.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_45.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_46.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_47.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_48.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_49.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_5.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_50.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_51.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_6.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_7.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_8.root',
'/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_9.root'

        )
)

process.outputs = cms.PSet (
    outputName = cms.string('shyftPlots_ttbar_pythia_syncex.root')
)
