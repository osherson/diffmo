import FWCore.ParameterSet.Config as cms

#from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector as pvSel
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
from PhysicsTools.SelectorUtils.jetIDSelector_cfi import jetIDSelector
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector



wplusjetsAnalysis = cms.PSet(
    # Primary vertex
    #pvSelector = cms.PSet( pvSel.clone(
    #    maxZ=cms.double(24.0)
    #    ) ),
    pvSelector = pvSelector.clone(), 
    # input parameter sets
    muonSrc = cms.InputTag('selectedPatMuons'),
    electronSrc = cms.InputTag('selectedPatElectrons'),
    jetSrc = cms.InputTag('selectedPatJets'),
    jetClonesSrc = cms.InputTag('myClones'),
    metSrc = cms.InputTag('patMETs'),
    trigSrc = cms.InputTag('patTriggerEvent'),
    muTrig = cms.string('HLT_Mu9'),
    eleTrig = cms.string('HLT_Ele10_LW_L1R'),
    pvSrc   = cms.InputTag('offlinePrimaryVertices'),
    useAntiSelection = cms.bool(False),
    useWP95Selection = cms.bool(False),
    useWP90Selection = cms.bool(False),
    useWP85Selection = cms.bool(False),
    useWP80Selection = cms.bool(False),
    useWP70Selection = cms.bool(True),
    useEleMC = cms.bool(False),
    usePFIso = cms.bool(False),
    useCone3 = cms.bool(False),
    useNoID  = cms.bool(False),
    # tight muons
    muonIdTight = cms.PSet(
        version = cms.string('FALL10'),
        Chi2 = cms.double(10.0),
        D0 = cms.double(0.02),
        ED0 = cms.double(999.0),
        SD0 = cms.double(999.0),
        NHits = cms.int32(11),
        NValMuHits = cms.int32(0),
        ECalVeto = cms.double(999.0),
        HCalVeto = cms.double(999.0),
        RelIso = cms.double(0.05),
        LepZ = cms.double(1.0),
        nPixelHits = cms.int32(1),
        nMatchedStations=cms.int32(1),
        cutsToIgnore = cms.vstring('ED0', 'SD0', 'ECalVeto', 'HCalVeto'),
        RecalcFromBeamSpot = cms.bool(False),
        beamLineSrc = cms.InputTag("offlineBeamSpot"),
        pvSrc = cms.InputTag("offlinePrimaryVertices"),
        ),
    # tight electrons
    electronIdTight = cms.PSet(
        version = cms.string('FIRSTDATA'),
        D0 = cms.double(999.0),
        ED0 = cms.double(999.0),
        SD0 = cms.double(3.0),
        RelIso = cms.double( 0.1 ),
        cutsToIgnore = cms.vstring('D0', 'ED0')
        ),
    # loose muons
    muonIdLoose = cms.PSet(
        version = cms.string('FALL10'),
        Chi2 = cms.double(999.0),
        D0 = cms.double(999.0),
        ED0 = cms.double(999.0),
        SD0 = cms.double(999.0),
        NHits = cms.int32(-1),
        NValMuHits = cms.int32(-1),
        ECalVeto = cms.double(999.0),
        HCalVeto = cms.double(999.0),
        RelIso = cms.double(0.2),
        LepZ = cms.double(1.0),
        nPixelHits = cms.int32(1),
        nMatchedStations=cms.int32(1),        
        cutsToIgnore = cms.vstring('Chi2', 'D0', 'ED0', 'SD0', 'NHits','NValMuHits','ECalVeto','HCalVeto','LepZ','nPixelHits','nMatchedStations'),
        RecalcFromBeamSpot = cms.bool(False),
        beamLineSrc = cms.InputTag("offlineBeamSpot"),
        pvSrc = cms.InputTag("offlinePrimaryVertices")
        ),
    # loose electrons
    electronIdLoose = cms.PSet(
        version = cms.string('FIRSTDATA'),
        D0 = cms.double(999.0),
        ED0 = cms.double(999.0),
        SD0 = cms.double(999.0),
        RelIso = cms.double( 0.2 ),
        cutsToIgnore = cms.vstring( 'D0', 'ED0', 'SD0')
        ),
	# my electrons
	mywp70 = cms.PSet(
	

	),
    # loose jets
    jetIdLoose = jetIDSelector.clone(),
    pfjetIdLoose = pfJetIDSelector.clone(),
    # kinematic cuts
    minJets        = cms.int32( 1 ),
    muPlusJets     = cms.bool( True ),
    ePlusJets      = cms.bool( False ),
    muPtMin        = cms.double( 30.0 ),
    muEtaMax       = cms.double( 2.1 ),
    eleEtMin       = cms.double( 20.0 ),
    eleEtaMax      = cms.double( 2.4 ),
    muPtMinLoose   = cms.double( 10.0 ),
    muEtaMaxLoose  = cms.double( 2.5 ),
    eleEtMinLoose  = cms.double( 15.0 ),
    eleEtaMaxLoose = cms.double( 2.5 ),
    elDist         = cms.double( 0.02 ),
    elDcot         = cms.double( 0.02 ),
    eRelIso        = cms.double( 0.1 ),
    eEtCut         = cms.double( 30. ),
    vertexCut      = cms.double( 1.),
    dxy            = cms.double( 0.02),
    jetPtMin       = cms.double( 30.0 ),
    jetEtaMax      = cms.double( 2.4 ),
    jetScale       = cms.double( 0.0 ),
    jetUncertainty = cms.double( 0.0 ),
    jetSmear       = cms.double( 0.0 ),
    ePtScale       = cms.double( 0.0 ),
    ePtUncertaintyEE = cms.double( 0.025), 
    metMin         = cms.double( 0.0 ),
    metMax         = cms.double( 100000.0),
    wMTMax         = cms.double( 100000.0),
    unclMetScale   = cms.double( 0.0 ),
    muJetDR        = cms.double( 0.3 ),
    useJetClones   = cms.bool(False),
    eleJetDR       = cms.double( 0.3 ),
    rawJetPtCut    = cms.double( 0.0 ),
    useData        = cms.bool(False),
    useL1Offset    = cms.bool(True),
    jecPayload     = cms.string('Jec10V1_Uncertainty_AK5PF.txt')
)
