#!/bin/bash
for i in 0 1 2 3 4 5 6 7 8 9 10
do 
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/data2013_D/res/' --outfile=Aug19_data --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin --useMC --sec=${i} --totalSec=10 &
done
exit
