import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")
import string
import sys

filesin = sys.argv[2]
outfile = sys.argv[3]

#prepend = 'dcap://'
prepend = ''

print 'Input files from ' + filesin
print 'Output file = ' + outfile

infilenames = cms.vstring()
txtfile = open(filesin)
for line in txtfile.readlines():
    infilenames.append( prepend + string.replace(line,'\n',''))

print infilenames


from Analysis.SHyFT.shyftAnalysis_cfi import shyftAnalysis as inputShyftAnalysis

process.shyftAnalysis = inputShyftAnalysis.clone(
    muonSrc = cms.InputTag('selectedPatMuonsPFlow'),
    electronSrc = cms.InputTag('selectedPatElectronsPFlow'),
    metSrc = cms.InputTag('patMETsPFlow'),
    jetSrc = cms.InputTag('selectedPatJetsPFlow'),
    jetClonesSrc = cms.InputTag('goodPFJets'),
    jetPtMin = cms.double(20.0),
    minJets = cms.int32(5),
    useJetClones = cms.bool(False)
    )

process.inputs = cms.PSet (
    fileNames = infilenames,
    maxEvents = cms.int32(1000)
)

process.outputs = cms.PSet (
    outputName = cms.string(outfile)
)
