#!/bin/bash
#for i in 0 1 2 3 4 5 6 7 8 9
for i in 0 1 2 3 4 5 
do 
     
     echo i
     python TTHadronicAnalyzerSplit.py --analyzer=MistagMakerType1 --dirs='/uscms/home/pilot/3DayLifetime/ttjets_powheg7_new/res/' --outfile=Aug19_ttjets7 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=5 &
     python TTHadronicAnalyzerSplit.py --analyzer=MistagMakerType1 --dirs='/uscms/home/pilot/3DayLifetime/ttjets_powheg10_new/res/' --outfile=Aug19_ttjets10 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=5 &
done

for i in 0 1 2 3 4 5 6 7 8 9
do
     echo ${i}
     #python TTHadronicAnalyzerSplit.py --analyzer=MistagMakerType1 --dirs='/uscms/home/pilot/3DayLifetime/data2013_A/res/' --outfile=Aug19_dataA --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=9 &
     #python TTHadronicAnalyzerSplit.py --analyzer=MistagMakerType1 --dirs='/uscms/home/pilot/3DayLifetime/data2013_B/res/' --outfile=Aug19_dataB --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=9 &
     #python TTHadronicAnalyzerSplit.py --analyzer=MistagMakerType1 --dirs='/uscms/home/pilot/3DayLifetime/data2013_C/res/' --outfile=Aug19_dataC --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=9 &
     #python TTHadronicAnalyzerSplit.py --analyzer=MistagMakerType1 --dirs='/uscms/home/pilot/3DayLifetime/data2013_D/res/' --outfile=Aug19_dataD --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=9 &
done

exit
