import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('Analysis.BoostedTopAnalysis.hadronicAnalysis_cfi')

#process.hadronicAnalysis.dijetSelectorParams.cutsToIgnore = cms.vstring(['Calo Jet ID', 'PF Jet ID'])
#process.hadronicAnalysis.cutsToIgnore = cms.vstring(['Trigger'])

process.hadronicAnalysis.dijetSelectorParams.ptMin = cms.double(25.0)


process.plotOptions = cms.PSet(
    plotTracks = cms.bool(True),
#    reweightHistoFile = cms.string('boostedTopPlots.root'),
#    sampleHistoFile   = cms.string('boostedTopPlots.root'),
#    reweightHistoName = cms.string('histos/nTracks')
    )

process.inputs = cms.PSet (
    
    fileNames = cms.vstring(
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_9_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_8_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_7_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_6_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_5_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_51_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_50_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_4_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_49_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_48_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_47_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_46_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_41_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_40_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_3_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_39_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_38_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_37_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_36_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_35_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_34_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_33_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_31_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_30_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_2_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_29_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_28_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_27_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_26_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_25_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_24_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_22_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_21_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_20_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_1_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_19_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_18_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_17_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_16_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_15_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_14_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_13_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_12_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_11_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/JetMETTau/May27thReReco_JetMETTau-PD-v1-ttbsm/a39b27a1daee71e2d2dfa202cd64c68d/ttbsm_361_10_1.root',




        ),
        lumisToProcess = cms.untracked.VLuminosityBlockRange(
	'136088:1-136088:3',
	'136088:6-136088:17',
	'136088:19-136088:39',
	'136088:41-136088:47',
	'136088:50-136088:50',
	'136088:52-136088:52',
	'136088:54-136088:63',
	'136088:65-136088:65',
	'136088:67-136088:89',
	'136088:91-136088:113',
	'136088:115-136088:131',
	'136088:133-136088:137',
	'136088:139-136088:184',
	'136088:187-136088:191',
	'136088:193-136088:194',
	'136088:196-136088:206',
	'136088:208-136088:208',
	'136088:210-136088:262',
	'136097:1-136097:1',
	'136097:3-136097:3',
	'136097:5-136097:5',
	'136097:7-136097:7',
	'136097:9-136097:9',
	'136097:11-136097:12',
	'136097:14-136097:79',
	'136098:1-136098:11',
	'136119:1-136119:36',

        )
)

process.outputs = cms.PSet (
    outputName = cms.string('boostedTopPlots_later_v1_jetid_trig_pt_gt25_templates.root')
)



        
