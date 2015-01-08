#!/bin/bash
seed=1

for i in `seq 0 13`;
do

python run_top_xs_TreeMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/ttbar7/'  --outfile=output/ttjets_powheg7_sec_${i}  --sec=${i} --totalSec=13 --mistagFile=mistag_lowMPM &

if [ ${i} -lt 6 ]; then
	python run_top_xs_TreeMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/ttbar10/' --outfile=output/ttjets_powheg10_sec_${i} --sec=${i} --totalSec=6 --mistagFile=mistag_lowMPM &

done
exit
