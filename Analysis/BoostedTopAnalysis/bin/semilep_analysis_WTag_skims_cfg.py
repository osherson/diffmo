import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")
process.load('Analysis.BoostedTopAnalysis.semileptonicAnalysis_cfi')
#process.semileptonicAnalysis.semileptonicSelection.cutsToIgnore = ['Hemispheric', 'Opposite leadJetPt']

import sys
import string

filesin = sys.argv[2]
outfile = sys.argv[3]

print 'Input files from ' + filesin
print 'Output file = ' + outfile

process.semileptonicAnalysis.semileptonicSelection.anyLeadJetPt = sys.argv[4]
process.semileptonicAnalysis.semileptonicSelection.oppLeadJetPt = sys.argv[4]
process.semileptonicAnalysis.semileptonicSelection.htLepCut = sys.argv[5]


infilenames = cms.vstring()
txtfile = open(filesin)
for line in txtfile.readlines():
    infilenames.append(string.replace(line,'\n',''))

print infilenames


process.inputs = cms.PSet (
    fileNames = infilenames

)


process.outputs = cms.PSet (
    outputName = cms.string(outfile)
)
