#!/bin/bash
for i in 0 1 2 3 4 5 6 7 8
do 
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/ttjets_powheg10_new/res/' --outfile=Aug19_ttjets10 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=8 &
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/ttjets_powheg7_new/res/' --outfile=Aug19_ttjets7 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=8 &
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/ttjets_powheg/res/' --outfile=Aug19_ttjetsFULL --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=1 &
done
exit
