import FWCore.ParameterSet.Config as cms

from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import boostedTopWTagParams
from PhysicsTools.SelectorUtils.wplusjetsAnalysis_cfi import wplusjetsAnalysis as wPlusJets


semileptonicAnalysis = cms.PSet(
    hadronicWParams = boostedTopWTagParams.clone(),
    semileptonicSelection = cms.PSet(
        WPlusJetsParams = wPlusJets.clone(
            jetSrc = cms.InputTag("selectedPatJetsCA8PrunedPF"),
            minJets = cms.int32(2),
            muPtMin = cms.double(25.0),
            jetPtMin = cms.double(25.0),
            muonIdTight  = wPlusJets.muonIdTight.clone( cutsToIgnore= ['RelIso']),
            muJetDR = cms.double(-1.0)
            ),
        jetSrc = cms.InputTag("selectedPatJetsCA8PrunedPF"),
        trigTag = cms.InputTag("patTriggerEvent"),
        cutsToIgnore      = cms.vstring(['Trigger']), #'Relative Pt','Minimum Delta R' ,'Opposite leadJetPt'
        dRMin             = cms.double(0.5),
        ptRelMin          = cms.double(25.0),
        mu                = cms.double(0.7),
        ycut              = cms.double(0.1),
        anyLeadJetPt      = cms.double(260.0),
        oppLeadJetPt      = cms.double(100.0),
        htLepCut          = cms.double(150.0),
        bDiscriminator    = cms.string('trackCountingHighEffBJetTags'),
        bDiscriminatorCut = cms.double(2.96)
        )

)
