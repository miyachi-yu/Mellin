#!/bin/bash
cd ${PBS_O_WORKDIR}
#rm -rf ~/.mellin
rm -rf ./qsub_tF1h.sh.*
#make clean
cd ../
#make
#source ~/work/QCD/Mellin_current/setup.sh
cd ${PBS_O_WORKDIR}

#for ORD in LO NLO
#for ORD in LO
for ORD in NLO
do 
#time ./testF1h  --unpolPDF=../browser/compare_unpolPDF.xml \
#time ./testF1h  --CTEQ6=../../PDF++/cteq6_tables/ \
#--FF=../browser/compare_FF.xml 
time ./testF1h --MRST=../config/MRST.xml \
--KretzerFF=../config/KretzerNLO.xml \
--alphaMz=0.114 \
--order=${ORD} --writeDB
done
