import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector

Type11QCDEstimationParams = cms.PSet( 
  Type11Selection = cms.PSet(
      pfJetIDParams = cms.PSet(
            pfJetIDSelector.clone(cutsToIgnore=cms.vstring([
                "CHF" ,
                "NHF" ,
                "CEF" ,
                "NEF" ,
                "NCH" ,
                "nConstituents",
                ])) ),
    caTopJetPtMin = cms.double(350),
    caTopJetEtaCut = cms.double(2.4),
    patJetCollectionInputTag = cms.InputTag( "goodPatJetsCA8PF" ),
    caTopJetCollectionInputTag = cms.InputTag( "goodPatJetsCATopTagPF" )
  ),

  caTopJetMassMin = cms.double(140),
  caTopJetMassMax = cms.double(250),
  caTopMinMassMin = cms.double(50),
  caTopMistagFileName = cms.string("top_mistag_rate_v8.root")
)

