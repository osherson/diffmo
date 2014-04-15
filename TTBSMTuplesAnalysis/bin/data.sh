#!/bin/bash
for i in 0 1 2 3 4 5 6 7 8 9 10
do 
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/data2013_A/res/' --outfile=/uscms/home/pilot/3DayLifetime/Mar20_dataA --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin_tau32  --useMC --sec=${i} --totalSec=10 &
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/data2013_B/res/' --outfile=/uscms/home/pilot/3DayLifetime/Mar20_dataB --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin_tau32  --useMC --sec=${i} --totalSec=10 &
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/data2013_C/res/' --outfile=/uscms/home/pilot/3DayLifetime/Mar20_dataC --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin_tau32  --useMC --sec=${i} --totalSec=10 &
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/data2013_D/res/' --outfile=/uscms/home/pilot/3DayLifetime/Mar20_dataD --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin_tau32  --useMC --sec=${i} --totalSec=10 &
done
exit
