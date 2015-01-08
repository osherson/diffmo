#!/bin/bash
for i in `seq 0 20`;
do 

python run_top_xs_MistagMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_A/'  --outfile=output/mistag/mistag_data_A_sec_${i}   --sec=${i} --totalSec=20 &
python run_top_xs_MistagMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_B/'  --outfile=output/mistag/mistag_data_B_sec_${i}   --sec=${i} --totalSec=20 &
python run_top_xs_MistagMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_C/'  --outfile=output/mistag/mistag_data_C_sec_${i}   --sec=${i} --totalSec=20 &
python run_top_xs_MistagMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/data_D/'  --outfile=output/mistag/mistag_data_D_sec_${i}   --sec=${i} --totalSec=20 &

if [ ${i} -lt 6 ]; then 
	python run_top_xs_MistagMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/ttbar10/' --outfile=output/mistag/mistag_ttjets10_sec_${i} --sec=${i} --totalSec=5 &
fi

if [ ${i} -lt 14 ]; then
	python run_top_xs_MistagMaker.py --dirs='/uscms_data/d3/dfehling/NTUPLES/Jan7/ttbar7/'  --outfile=output/mistag/mistag_ttjets7_sec_${i}  --sec=${i} --totalSec=13 &
fi

done
exit
