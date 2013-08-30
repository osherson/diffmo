import FWCore.ParameterSet.Config as cms


boostedTopWTagParams = cms.PSet(
    jetPtMin = cms.double(200.0),
    jetEtaMax = cms.double(2.4),
    muMax = cms.double(0.4),
    ycut = cms.double(0.0),  #loose OP
    numOfDaughters = cms.uint32(2)
    )
