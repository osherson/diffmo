import FWCore.ParameterSet.Config as cms


caTopTagParams = cms.PSet(
    version = cms.string('PF'),
    useTopMassCuts = cms.bool(True),
    topMassMin = cms.double(100.0),
    topMassMax = cms.double(250.0),
    minMass = cms.double(50.0),
    useWMassCuts = cms.bool(False),
    wMassMin = cms.double(0.0),
    wMassMax = cms.double(9999999.),
    tagName = cms.string('CATopPFJetTagInfos')
    )
