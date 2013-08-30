import FWCore.ParameterSet.Config as cms



simpleSubstructureSelector = cms.PSet(
    # Jets: Already have jet ID applied
    src = cms.InputTag('goodPatJetsCA8PrunedPF'),
    # Rho from the KT6 jets
    rhoSrc = cms.InputTag('kt6PFJetsPFlow', 'rho'),
    # Offline Primary Vertices (all of them) to reweight MC
    pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
    # pt cut for both jets in the sample
    ptCut = cms.double( 50.0 ),
    # rapidity cut for both jets in the sample    
    rapidityCut = cms.double( 2.4 ),
    # Mass drop cut
    muCut = cms.double(0.25),
    # jet mass cut
    mCut = cms.double(60),
    # Jet energy scale "up" or "down" for MC?
    jetScale = cms.double( 0.0 ),
    # Jet energy resolution "up" or "down" for MC?
    jetSmear = cms.double( 0.0 ),
    # Additional uncertainty to add to the jet for MC
    jetUncertainty = cms.double( 0.0 ),
    # JEC and uncertainty payloads
    jecPayloads = cms.vstring( [
        'Jec10V3_L1FastJet_AK5PFchs.txt',
        'Jec10V3_L2Relative_AK5PFchs.txt',
        'Jec10V3_L3Absolute_AK5PFchs.txt',
        'Jec10V3_L2L3Residual_AK5PFchs.txt'] ),
    jecUncPayload = cms.string('Jec10V3_Uncertainty_AK5PFchs.txt')
    
)
