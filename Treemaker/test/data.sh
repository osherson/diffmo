#!/bin/bash

for i in `seq 0 20`;
do 
#     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms_data/d3/dfehling/NTUPLES/from_jim/TTBAR_ALLHAD/all_data/' --outfile=Jan14_data_newPilotMistag --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile='/uscms_data/d3/dfehling/NTUPLES/from_pilot/data_AllBscore_mistag_Dec16.root' --useMC --sec=${i} --totalSec=20 &

python run_TopXS_Tree.py --dirs='/uscms_data/d3/dfehling/diffmo_091014_update/CMSSW_5_3_17/src/Analysis/Ntuplizer/test/Data_Run_C_condor/res/' --outfile=data_run_C_lumi_sec_${i} --sec=${i} --totalSec=20 &

done
exit
