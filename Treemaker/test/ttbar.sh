#!/bin/bash
seed=1

for i in `seq 0 10`;
do

python run_TopXS_Tree.py --outfile=ttjets_powheg10_sec_${i} --sec=${i} --totalSec=10 --dirs='/uscms_data/d3/dfehling/diffmo_091014_update/CMSSW_5_3_17/src/Analysis/Ntuplizer/test/TT_Mtt_1000toInf_condor_try2/res/' &

python run_TopXS_Tree.py --outfile=ttjets_powheg7_sec_${i} --sec=${i} --totalSec=10 --dirs='/uscms_data/d3/dfehling/diffmo_091014_update/CMSSW_5_3_17/src/Analysis/Ntuplizer/test/TT_Mtt_700to1000/res/' &

done
exit
