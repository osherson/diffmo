import FWCore.ParameterSet.Config as cm

from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import *

wPlusBJetAnalysis = cms.PSet(
    WPlusBJetEventSelection = cms.PSet( 
      jetSrc = cms.InputTag( "selectedPatJetsCA8PrunedPF" ),
      trigSrc = cms.InputTag( "patTriggerEvent" ),
      trig    = cms.string( "HLT_Jet50U" ),
      pfJetIDParams = cms.PSet( pfJetIDSelector.clone() ),
      BoostedTopWJetParameters  = boostedTopWTagParams.clone( ),
      jetPtMin  = cms.double(30.0),
      jetEtaMax = cms.double(3.0),
      bTagAlgorithm = cms.string( "trackCountingHighEffBJetTags" ),
      bTagOP   = cms.double( 3.3 ),
      bTagOPMedium = cms.double( 3.3 ),
      bTagOPLoose = cms.double(1.7),
      mistagFileName = cms.string("mistag.root"),
      leadJetPtCut = cms.double(200),
      secondJetPtCut  = cms.double(150),
      thirdJetPtCut   = cms.double(50),
      fourthJetPtCut  = cms.double(30),
      wMassMin    = cms.double(60),
      wMassMax    = cms.double(100),
      topMassMin  = cms.double(140),
      topMassMax  = cms.double(230)
    ),

    runOnData = cms.bool( False ),
    runOnTTbar = cms.bool( False ),
    debugOn = cms.bool( False )
)
