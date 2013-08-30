import sys
import string

import FWCore.ParameterSet.Config as cms

filesin = sys.argv[2]
outfile = sys.argv[3]

print 'Input files from ' + filesin
print 'Output file = ' + outfile

infilenames = cms.vstring()
txtfile = open(filesin)
for line in txtfile.readlines():
    infilenames.append(string.replace(line,'\n',''))

print infilenames

process = cms.Process("FWLitePlots")

process.jetStudies = cms.PSet(
    # input parameter sets
    jetSrc = cms.InputTag('selectedPatJetsStd'),
    pfJetSrc = cms.InputTag('selectedPatJets'),
    metSrc = cms.InputTag('patMETsStd'),
    pfMetSrc = cms.InputTag('patMETs'),
    useCalo = cms.bool(True),
    minNJets = cms.uint32(4),
    ptMin = cms.double(25.0),
    etaMax = cms.double(3.0),
    htMin = cms.double(100.0),
    ht2Min = cms.double(100.0)
)
process.pfJetStudies = process.jetStudies.clone( useCalo = cms.bool(False),
                                                 ptMin = cms.double(25.0) )


process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')

process.plotParameters = cms.PSet (
    doTracks = cms.bool(False),
    useMC = cms.bool(False),
    runs = cms.vint32([])
)


process.inputs = cms.PSet (
    fileNames = infilenames
    )

process.outputs = cms.PSet (
    outputName = cms.string(outfile)
)
 
