import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")
import string
import sys

filesin = sys.argv[2]
outfile = sys.argv[3]

#prepend = 'dcap:///pnfs/cms/WAX/11'
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
    #jetSrc = cms.InputTag('goodPatJetsPFlow'),
    jetSrc = cms.InputTag('selectedPatJetsPFlow'),
    jetPtMin = cms.double(25.0),
    minJets = cms.int32(5),
    eRelIso = cms.double(0.1),
    ePlusJets = cms.bool(True),
    muPlusJets = cms.bool(False),
    #usePFIso = cms.bool(True), ###
    metMin = cms.double(0.),
    wMTMax = cms.double(10000.),
    cutsToIgnore=cms.vstring('Trigger'),

    #metMax = cms.double(25.),

   #***********Uncomment if want to test on WJets, ZJets, Vqq, WcJets******************
    #sampleName="Wjets",
    #sampleName = "Top",
    #doMC = True,
    #heavyFlavour = True,
   #***********************************************************************************
    
    #useEleMC   = cms.bool(True),

    #********Uncomment if want to produce anti-electrons***************************
    #useAntiSelection = cms.bool(True),
    #useEleMC  = cms.bool(False)
    #*******************************************************************************
    
    )

process.inputs = cms.PSet (
    fileNames = infilenames,
    maxEvents = cms.int32(1000)
)


process.outputs = cms.PSet (
    outputName = cms.string(outfile)
)
