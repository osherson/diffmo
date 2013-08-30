#!/bin/bash
for i in 0 1
do 
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/ZprimeNrw2000/res/' --outfile=Aug19 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=1 &
done
exit
