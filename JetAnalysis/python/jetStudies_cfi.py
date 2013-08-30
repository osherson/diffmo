import FWCore.ParameterSet.Config as cms

jetStudies = cms.PSet(
    # input parameter sets
    jetSrc = cms.InputTag('selectedPatJets'),
    pfJetSrc = cms.InputTag('selectedPatJetsAK5PF'),
    metSrc = cms.InputTag('patMETs'),
    pfMetSrc = cms.InputTag('patMETsPF'),
    isData = cms.bool(True),
    useCalo = cms.bool(True),
    doPV = cms.bool(True),
    # kinematic cuts
    minJets        = cms.int32( 1 ),
    jetPtMin       = cms.double( 20.0 ),
    jetEtaMax      = cms.double( 3.0 ),
    jetScale       = cms.double( 1.0 )
)


