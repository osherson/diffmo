import FWCore.ParameterSet.Config as cms
from Analysis.BoostedTopAnalysis.CATopTagParams_cfi import *


caTopJetEventSelectorParams = cms.PSet(
	CATopJetSrc = cms.InputTag( "selectedPatJets" ),
	jetPtMin    = cms.double( 500 ),  #JHU Top Tagger turn on point
	jetEtaMax   = cms.double( 2.4 ),
	coneSize    = cms.double( 2.0 ),
	CATopJetParameters = caTopTagParams.clone() 
)
