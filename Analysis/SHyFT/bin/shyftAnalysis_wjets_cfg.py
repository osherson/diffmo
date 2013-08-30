import FWCore.ParameterSet.Config as cms

import sys

ijob = int(sys.argv[2])
njob = int(sys.argv[3])


process = cms.Process("FWLitePlots")


from Analysis.SHyFT.shyftAnalysis_cfi import shyftAnalysis as inputShyftAnalysis

process.shyftAnalysis = inputShyftAnalysis.clone(
    sampleName="Wjets",
    doMC = True,
    heavyFlavour = True,
    doTagWeight = True,
    btaggerString = 'simpleSecondaryVertexBJetTags'
)




fileNamesLong = cms.vstring(
    [
       'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/shyft_38xOn35x_v3/11c19408717a8a2546a9b3c7cb40b7a6/shyft_382_mc_9_1_vca.root',
]
)

fileNamesShort = cms.vstring(
    [
'dcap:///pnfs/cms/WAX/11/store/user/dfehling/WJets-madgraph/SHYFT_pat362_fhf/7359e37053c9d3aa1a521cbe89ffd6d6/shyft_362_pat_100_0_16T.root',
'dcap:///pnfs/cms/WAX/11/store/user/dfehling/WJets-madgraph/SHYFT_pat362_fhf/7359e37053c9d3aa1a521cbe89ffd6d6/shyft_362_pat_101_0_Urd.root',
'dcap:///pnfs/cms/WAX/11/store/user/dfehling/WJets-madgraph/SHYFT_pat362_fhf/7359e37053c9d3aa1a521cbe89ffd6d6/shyft_362_pat_102_0_cMG.root',
'dcap:///pnfs/cms/WAX/11/store/user/dfehling/WJets-madgraph/SHYFT_pat362_fhf/7359e37053c9d3aa1a521cbe89ffd6d6/shyft_362_pat_103_0_et2.root',
'dcap:///pnfs/cms/WAX/11/store/user/dfehling/WJets-madgraph/SHYFT_pat362_fhf/7359e37053c9d3aa1a521cbe89ffd6d6/shyft_362_pat_104_0_hE7.root',
]
)

nper = len(fileNamesLong) / (njob-1)


#fileNamesIn = fileNamesLong[nper * ijob : nper*(ijob+1)]
fileNamesIn = fileNamesLong
print 'Processing these files : '
for iifile in fileNamesIn :
    print iifile


process.inputs = cms.PSet (
    maxEvents = cms.int32(-1),
    fileNames = cms.vstring( fileNamesIn )
)

process.outputs = cms.PSet (
    outputName = cms.string('shyftPlots_wjets_' + str(ijob) + '.root')
)
