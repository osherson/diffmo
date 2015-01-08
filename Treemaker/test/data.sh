#!/bin/bash

for i in `seq 0 20`;
do 

python run_top_xs_TreeMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_A/' --outfile=output/data_A_sec_${i} --sec=${i} --sec=${i} --totalSec=20 --mistagFile=mistag_lowMPM &
python run_top_xs_TreeMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_B/' --outfile=output/data_B_sec_${i} --sec=${i} --sec=${i} --totalSec=20 --mistagFile=mistag_lowMPM &
python run_top_xs_TreeMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_C/' --outfile=output/data_C_sec_${i} --sec=${i} --sec=${i} --totalSec=20 --mistagFile=mistag_lowMPM &
python run_top_xs_TreeMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_D/' --outfile=output/data_D_sec_${i} --sec=${i} --sec=${i} --totalSec=20 --mistagFile=mistag_lowMPM &

done
exit
