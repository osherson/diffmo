import FWCore.ParameterSet.Config as cms



jetStudies = cms.PSet(
    # input parameter sets
    jetSrc = cms.InputTag('selectedPatJetsStd'),
    pfJetSrc = cms.InputTag('selectedPatJets'),
    metSrc = cms.InputTag('patMETsStd'),
    pfMetSrc = cms.InputTag('patMETs'),
    useCalo = cms.bool(True),
    minNJets = cms.uint32(4),
    ptMin = cms.double(25.0),
    etaMax = cms.double(3.0),
    htMin = cms.double(100.0),
    ht2Min = cms.double(100.0)
    
)
pfJetStudies = jetStudies.clone( useCalo = cms.bool(False),
                                 ptMin = cms.double(25.0) )
