import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.jetStudies = cms.PSet(
    # input parameter sets
    jetSrc = cms.InputTag('selectedPatJets'),
    pfJetSrc = cms.InputTag('selectedPatJetsAK5PF'),
    metSrc = cms.InputTag('patMETs'),
    pfMetSrc = cms.InputTag('patMETsPF'),
    useCalo = cms.bool(True),
    minNJets = cms.int32(2),
    ptMin = cms.double(30.0)
)
process.pfJetStudies = process.jetStudies.clone( useCalo = cms.bool(False) , 
                                                 ptMin = cms.double(30.0) )


process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')

process.plotParameters = cms.PSet (
    doTracks = cms.bool(False),
    useMC = cms.bool(False),
    runs = cms.vint32(132606,
                      132605,
                      132602,
                      132601,
                      132599,
                      132473,
                      132440)
)


process.inputs = cms.PSet (
    fileNames = cms.vstring(
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_early_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_1_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_early_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_2_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_early_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_3_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_early_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_4_1.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_10_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_11_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_12_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_13_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_14_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_15_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_1_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_2_1.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_3_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_4_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_5_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_6_1.root',
        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_7_1.root'
#        'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_8_1.root',
#        'dcap:///pnfsy!/cms/WAX/11/store/user/rappocc/MinimumBias/pat_7TeV_goodcoll_later_v1/a33509872d8a154712f5dbcd3cd763f5/reco_7TeV_firstdata_356_pat_9_1.root'
        )
)

process.outputs = cms.PSet (
    outputName = cms.string('jetPlots_goodcoll_highpt.root')
)
