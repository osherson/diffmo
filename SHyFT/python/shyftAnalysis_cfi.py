import FWCore.ParameterSet.Config as cms


from Analysis.SHyFT.shyftselection_cfi import wplusjetsAnalysis

shyftAnalysis = cms.PSet(
    wplusjetsAnalysis.clone() ,
    sampleName = cms.string("top"),
    heavyFlavour = cms.bool(False),
    doMC           = cms.bool(False),
    doBTagPerformance = cms.bool(True),
    payload = cms.string( "PayLoad.root" ),
    bPerformanceTag = cms.string( "MCSSVMb" ),
    cPerformanceTag = cms.string( "MCSSVMc" ),
    lPerformanceTag = cms.string( "MCSSVMl" ),
    btaggerString = cms.string('simpleSecondaryVertexHighEffBJetTags'),
    identifier = cms.string('Douglas Adams'),
    reweightPDF = cms.bool(False),
    pdfEigenToUse = cms.int32(0),    
    pdfSrc = cms.InputTag('generator'),
    pdfToUse = cms.string('cteq6ll.LHpdf'),
    pdfVariation = cms.int32(1),
    reweightBTagEff = cms.bool(False),
    bcEffScale = cms.double(1.0),
    lfEffScale = cms.double(1.0),
    useDefaultDiscriminant = cms.bool(True),
    bDiscriminantCut = cms.double(-1.0),
    cDiscriminantCut = cms.double(-1.0),
    lDiscriminantCut = cms.double(-1.0),
    allDiscriminantCut = cms.double(1.74),
    simpleSFCalc = cms.bool(False),
    weightSFCalc = cms.bool(False),
    jetAlgo = cms.string("pf"),
    useCustomPayload = cms.bool(False),
    customPayload = cms.string('ttbarEffSF_unity.root')   
)

