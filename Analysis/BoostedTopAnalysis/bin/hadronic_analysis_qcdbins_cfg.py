import FWCore.ParameterSet.Config as cms
import sys
import string

process = cms.Process("FWLitePlots")


process.load('Analysis.BoostedTopAnalysis.hadronicAnalysis_cfi')

#process.hadronicAnalysis.dijetSelectorParams.cutsToIgnore = cms.vstring(['Calo Jet ID', 'PF Jet ID'])
process.hadronicAnalysis.cutsToIgnore = cms.vstring(['Trigger'])

process.plotOptions = cms.PSet(
    plotTracks = cms.bool(True),
    reweightHistoFile = cms.string('reweight_histo.root'),
    reweightHistoName = cms.string('ratio')
            )


filesin = sys.argv[2]
outfile = sys.argv[3]

print 'Input files from ' + filesin
print 'Output file = ' + outfile

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
