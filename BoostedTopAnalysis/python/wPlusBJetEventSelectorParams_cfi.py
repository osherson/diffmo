import FWCore.ParameterSet.Config as cms

from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import *

wPlusBJetEventSelectorParams = cms.PSet(
	jetSrc = cms.InputTag( "selectedPatJets" ),
	jetPtMin =  cms.double( 25.0 ),
	jetEtaMax = cms.double( 2.4 ),
	consSize  = cms.double( 2.0 ),
	bTagAlgorithm = cms.string( "trackCountingHighEffBJetTags" ),
	bTagOP   = cms.double( 3.3 ),
	BoostedTopWJetParameters = boostedTopWTagParams.clone( jetPtMin = 200.0, jetEtaMax = 2.4 )
)
