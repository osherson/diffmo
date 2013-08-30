#!/bin/bash
for i in 0 1 2 
do 
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_300_470/res/' --outfile=Aug19_qcd300 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin --useMC --sec=${i} --totalSec=2 &
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_470_600/res/' --outfile=Aug19_qcd470 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin --useMC --sec=${i} --totalSec=2 &
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_600_800/res/' --outfile=Aug19_qcd600 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin --useMC --sec=${i} --totalSec=2 &
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_800_1000/res/' --outfile=Aug19_qcd800 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin --useMC --sec=${i} --totalSec=2 &
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_1000_1400/res/' --outfile=Aug19_qcd1000 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin --useMC --sec=${i} --totalSec=2 &
     python TTHadronicAnalyzerSplit.py --analyzer=TTPairAnalyzer --dirs='/uscms/home/pilot/3DayLifetime/QCD_1400_1800/res/' --outfile=Aug19_qcd1400 --triggerFile=TRIGGER_EFF_TYPE11 --triggerWeight=noWeight --modMassFile=ModMassFileQCD --mistagFile=mistag_lowmMin --useMC --sec=${i} --totalSec=2 &
done
exit
