import FWCore.ParameterSet.Config as cms

import sys
import string

#input stuff for Run/Lumi selection with the "JSON"-formatted files from the PVT group
import FWCore.PythonUtilities.LumiList as LumiList

# setup process
process = cms.Process("FWLitePlots")

# get JSON file correctly parced
JSONfile = 'Cert_132440-141961_7TeV_StreamExpress_Collisions10_JSON.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')


from Analysis.BoostedTopAnalysis.hadronicAnalysis_cfi import hadronicAnalysis as inputHadronicAnalysis

process.hadronicAnalysis = inputHadronicAnalysis.clone(
    inputHadronicAnalysis.dijetSelectorParams.clone(
        pfMetSrc = cms.InputTag('patMETsPFlow'),
        ptMin = cms.double(50.0)
        ),
    maxNPV = cms.int32(1),
    trig = cms.string('HLT_Jet30U')
    )



#process.hadronicAnalysis.dijetSelectorParams.cutsToIgnore = cms.vstring(['Calo Jet ID', 'PF Jet ID'])
#process.hadronicAnalysis.cutsToIgnore = cms.vstring(['Trigger'])

process.plotOptions = cms.PSet(
    plotTracks = cms.bool(True)
#    reweightHistoFile = cms.string('boostedTopPlots.root'),
#    sampleHistoFile   = cms.string('boostedTopPlots.root'),
#    reweightHistoName = cms.string('histos/nTracks')
    )


filesin = sys.argv[2]
outfile = sys.argv[3]

print 'Input files from ' + filesin
print 'Output file = ' + outfile

pnfsdir = 'dcap:///pnfs/cms/WAX/11'

infilenames = cms.vstring()
txtfile = open(filesin)
for line in txtfile.readlines():
    infilenames.append( pnfsdir + string.replace(line,'\n',''))

print 'Processing these files : ' 
for infilename in infilenames :
    print infilename


process.inputs = cms.PSet (
    fileNames = infilenames,
    lumisToProcess = cms.untracked.VLuminosityBlockRange( myList )

)
process.outputs = cms.PSet (
    outputName = cms.string(outfile)
)
