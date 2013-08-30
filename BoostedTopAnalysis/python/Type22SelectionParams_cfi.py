import FWCore.ParameterSet.Config as cm

from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import *

Type22QCDEstimationParams = cms.PSet( 
  Type22Selection = cms.PSet(
      pfJetIDParams = cms.PSet(
            pfJetIDSelector.clone(cutsToIgnore=cms.vstring([
                "CHF" ,
                "NHF" ,
                "CEF" ,
                "NEF" ,
                "NCH" ,
                "nConstituents",
                ])) ),
    BoostedTopWJetParameters  = boostedTopWTagParams.clone( ),
    jetPt0 = cms.double(200),
    jetPt1 = cms.double(30),
    jetEta = cms.double(2.4),
    bTagOP = cms.double(3.3),
    bTagAlgo = cms.string("trackCountingHighEffBJetTags"),
    jetSrc = cms.InputTag( "goodPatJetsCA8PrunedPF" )
  ),

  wMassMin = cms.double(60),
  wMassMax = cms.double(100),
  topMassMin = cms.double(140),
  topMassMax = cms.double(230),
  mistagFile = cms.string("mistag.root"),
  Probability = cms.double(0.15),
  runOnData = cms.bool(True)
)

