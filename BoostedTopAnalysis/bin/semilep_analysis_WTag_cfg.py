import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.load('Analysis.BoostedTopAnalysis.semileptonicAnalysis_cfi')



process.inputs = cms.PSet (

fileNamesShort = cms.vstring( #
       'dcap:///pnfs/cms/WAX/11/store/user/srappocc/Zprime_M1TeV_W10GeV-madgraph/ttbsm_361_v1/2c398110672bba227822a112ea0dde68/ttbsm_361_9_1.root',
       'dcap:///pnfs/cms/WAX/11/store/user/srappocc/Zprime_M1TeV_W10GeV-madgraph/ttbsm_361_v1/2c398110672bba227822a112ea0dde68/ttbsm_361_99_1.root',
       'dcap:///pnfs/cms/WAX/11/store/user/srappocc/Zprime_M1TeV_W10GeV-madgraph/ttbsm_361_v1/2c398110672bba227822a112ea0dde68/ttbsm_361_98_1.root'
       ),
fileNamesInclusiveMu15 = cms.vstring(
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_9_1_9Zg.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_8_1_fyg.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_7_1_imu.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_6_1_9lx.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_5_1_bC2.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_51_1_m0A.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_50_1_DvL.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_4_1_HDo.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_49_1_j5i.root',
        'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_48_1_tgj.root'
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_47_1_vYo.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_46_1_6MC.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_45_1_kVX.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_44_1_X9j.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_43_1_7wt.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_42_1_OuP.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_41_1_VeE.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_40_1_zee.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_3_1_HN7.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_39_1_8fG.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_38_1_RMs.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_37_1_nrS.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_36_1_lJU.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_35_1_OS7.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_34_1_G1c.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_33_1_YQO.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_32_1_Lqr.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_31_1_QtN.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_30_1_8PH.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_2_1_J0G.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_29_1_myp.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_28_1_sYq.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_27_1_7Wg.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_26_1_VgX.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_25_1_gVb.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_24_1_qbD.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_23_1_CBA.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_22_1_DNt.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_21_1_YAx.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_20_1_c1t.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_1_1_bKN.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_19_1_eyh.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_18_1_mtj.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_17_1_lJU.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_16_1_Wbm.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_15_1_nlN.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_14_1_YCB.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_13_1_gCS.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_12_1_2nA.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_11_1_P2J.root',
        # 'dcap:///pnfs/cms/WAX/11/store/user/guofan/InclusiveMu15/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_10_1_zAQ.root'
),
fileNames = cms.vstring(
        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_9_1_zpm.root'
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_8_1_9pD.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_7_1_rHf.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_6_1_CaF.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_5_1_tDc.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_53_1_luX.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_52_1_Xqb.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_51_1_0wp.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_50_1_2hf.root',
#        'dcap:///pnfs/cms/WAX/11/store/user/srappocc/WJets-madgraph/ttbsm_361_v1/6e46b5e195f4b3b727488919472b1913/ttbsm_361_4_1_k85.root'
        ),
fileNames1TeVZPrime = cms.vstring(#
        '/uscms/home/dbjergaa/nobackup/ZPrimePatTuple/BoostedTopPatTuple_50k.root'
        )
   )
process.outputs = cms.PSet (
    outputName = cms.string('boostedSemiLepTopPlots.root')
)
