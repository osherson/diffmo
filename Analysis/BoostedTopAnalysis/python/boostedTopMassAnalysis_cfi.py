import FWCore.ParameterSet.Config as cms

from Analysis.BoostedTopAnalysis.BoostedTopWTagParams_cfi import boostedTopWTagParams
from Analysis.SHyFT.shyftPFSelection_cfi import shyftPFSelection


boostedTopMassAnalysis = cms.PSet(
    hadronicWParams = boostedTopWTagParams.clone(),
    shyftSelection = shyftPFSelection.clone()
)
