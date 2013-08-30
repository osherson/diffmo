import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.jetStudies = cms.PSet(
    # input parameter sets
    jetSrc = cms.InputTag('selectedPatJets'),
    pfJetSrc = cms.InputTag('selectedPatJetsAK5PF'),
    metSrc = cms.InputTag('patMETs'),
    pfMetSrc = cms.InputTag('patMETsPF'),
    useCalo = cms.bool(True),
    minNJets = cms.int32(4),
    ptMin = cms.double(25.0)
)
process.pfJetStudies = process.jetStudies.clone( useCalo = cms.bool(False),
                                                 ptMin = cms.double(25.0) )


process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')

process.plotParameters = cms.PSet (
    doTracks = cms.bool(False),
    useMC = cms.bool(False),
    runs = cms.vint32([])
)


process.inputs = cms.PSet (
    fileNamesShort = cms.vstring(
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_1.root'
        ),
    fileNames = cms.vstring(
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_1.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_10.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_100.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_101.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_102.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_103.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_105.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_106.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_11.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_12.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_13.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_14.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_15.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_16.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_17.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_18.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_19.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_2.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_20.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_21.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_22.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_23.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_24.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_25.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_26.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_27.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_28.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_29.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_3.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_30.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_31.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_32.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_33.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_34.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_35.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_36.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_37.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_38.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_39.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_4.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_40.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_41.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_42.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_43.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_44.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_45.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_46.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_47.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_48.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_49.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_5.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_50.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_51.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_52.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_53.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_54.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_55.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_56.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_57.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_58.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_59.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_6.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_60.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_61.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_62.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_63.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_64.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_65.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_66.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_67.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_68.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_69.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_7.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_70.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_71.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_72.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_73.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_74.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_75.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_76.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_77.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_78.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_79.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_8.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_80.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_81.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_82.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_83.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_84.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_85.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_86.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_87.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_88.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_89.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_90.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_91.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_92.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_93.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_94.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_95.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_96.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_97.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_98.root',
'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinBias/pat_7TeV_minbias_mc_356_v3/8eaeca184c893b426c750ab28dfdeb05/reco_7TeV_firstdata_mc_356_pat_99.root'

        )
)

process.outputs = cms.PSet (
    outputName = cms.string('multijetPlots_mc_4jets_full.root')
)
 
