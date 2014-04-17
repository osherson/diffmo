import FWCore.ParameterSet.Config as cms

# Run:
process = cms.Process("jhu")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20))
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('file:/eos/uscms/store/user/lpctlbsm/noreplica/dsperka/SingletopWprime_M-1100_right_TuneZ2star_8TeV-comphep/Summer12_DR53X-PU_S10_START53_V7A-v1_TLBSM_53x_v3/99bd99199697666ff01397dad5652e9e/tlbsm_53x_v3_mc_128_1_eB8.root'))
process.diffmo = cms.EDFilter('HEPtuplizer', 
				pvSrc = cms.InputTag('goodOfflinePrimaryVertices'),
				ca8Src = cms.InputTag('goodPatJetsCA8PF'),
				ca8prunedSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
				ca8toptagSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
				muonSrc = cms.InputTag('selectedPatMuonsPFlowLoose'),
				electronSrc = cms.InputTag('selectedPatElectronsPFlowLoose')
			       )
print "wonky"
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.p = cms.Path(process.diffmo)
print "bonky"
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("heptest.root"),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p')),
                               outputCommands = cms.untracked.vstring('drop *','keep *_diffmo*_*_*'))
print "ba-donky"
process.outpath = cms.EndPath(process.out)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.out.dropMetaData = cms.untracked.string("DROPPED")
print "-------------------------------------------------------------"
