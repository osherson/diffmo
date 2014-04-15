#!/bin/bash
for i in 0 1 2 3 4 5 6 7 8
do 
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_1400_1800/res/' --outfile=/uscms/home/pilot/3DayLifetime/Dec3_QCD_1400 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=8 &
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_1000_1400/res/' --outfile=/uscms/home/pilot/3DayLifetime/Dec3_QCD_1000 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=8 &
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_800_1000/res/' --outfile=/uscms/home/pilot/3DayLifetime/Dec3_QCD_800 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=8 &
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_600_800/res/' --outfile=/uscms/home/pilot/3DayLifetime/Dec3_QCD_600 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=8 &
     #python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_470_600/res/' --outfile=/uscms/home/pilot/3DayLifetime/Dec3_QCD_470 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=8 &
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_300_470/res/' --outfile=/uscms/home/pilot/3DayLifetime/Dec3_QCD_300 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag --useMC --sec=${i} --totalSec=8 &
done
exit
