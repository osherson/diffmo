import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector as pvSel
from PhysicsTools.SelectorUtils.jetIDSelector_cfi import jetIDSelector
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from Analysis.BoostedTopAnalysis.CATopTagParams_cfi import caTopTagParams as inputCATopTagParams
from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import boostedTopWTagParams as inputBoostedTopWTagParams

hadronicAnalysis = cms.PSet(
    pvSelector = cms.PSet( pvSel.clone() ),
    caTopTagParams = cms.PSet ( inputCATopTagParams.clone() ),
    boostedTopWTagParams = cms.PSet( inputBoostedTopWTagParams.clone() ),
    dijetSelectorParams = cms.PSet(
        jetSrc = cms.InputTag('selectedPatJetsCA8PrunedCalo'),
        pfJetSrc = cms.InputTag('selectedPatJetsCA8PrunedPF'),
        metSrc = cms.InputTag('patMETs'),
        pfMetSrc = cms.InputTag('patMETsPF'),
        useCalo = cms.bool(False),
        minNJets = cms.uint32(2),
        ptMin = cms.double(25.0),
        etaMax = cms.double(3.0),
        htMin = cms.double(0.0),
        ht2Min = cms.double(0.0)
        ),
    jetIDParams = cms.PSet( jetIDSelector.clone() ),
    pfJetIDParams = cms.PSet( pfJetIDSelector.clone() ),
    pvSrc = cms.InputTag('offlinePrimaryVertices'),
    trigSrc = cms.InputTag('patTriggerEvent'),
    trig = cms.string('HLT_Jet30U'),
    minTags = cms.uint32(0),
    minNPV = cms.int32(0),
    maxNPV = cms.int32(100000),
    usePF = cms.bool(True),
    useWTag = cms.bool(True),
    wMinMass = cms.double(60),
    wMaxMass = cms.double(100),
    bTagOPMedium = cms.double(3.3),
    bTagOPLoose = cms.double(1.7),
    mistagFileName = cms.string("Mistag_Dijet.root"),
    onePV = cms.bool(False)
)
