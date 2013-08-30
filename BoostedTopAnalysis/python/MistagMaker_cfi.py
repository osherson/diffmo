import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.jetIDSelector_cfi import jetIDSelector
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import boostedTopWTagParams as inputBoostedTopWTagParams

MistagMakerParams = cms.PSet(
    dijetSelectorParams = cms.PSet(
        jetSrc = cms.InputTag('selectedPatJetsCA8PrunedCalo'),
        pfJetSrc = cms.InputTag('goodPatJetsCA8PrunedPF'),
        metSrc = cms.InputTag('patMETs'),
        pfMetSrc = cms.InputTag('patMETsPF'),
        useCalo = cms.bool(False),
        minNJets = cms.uint32(2),
        ptMin = cms.double(200),
        etaMax = cms.double(2.4),
        htMin = cms.double(0.0),
        ht2Min = cms.double(0.0)
        ),
    jetIDParams = cms.PSet( jetIDSelector.clone() ),
    pfJetIDParams = cms.PSet( pfJetIDSelector.clone() ),
    boostedTopWTagParams = cms.PSet( inputBoostedTopWTagParams ),
    wMassMin  = cms.double(60),
    wMassMax  = cms.double(100),
    jetPtMin  = cms.double(200)
)
