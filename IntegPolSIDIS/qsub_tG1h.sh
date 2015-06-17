#!/bin/bash
cd ${PBS_O_WORKDIR}
#rm -rf ~/.mellin
#rm -rf ./qsub_tG1h.sh.*
#make clean
cd ../
#make
#source ~/work/QCD/Mellin_current/setup.sh
cd ${PBS_O_WORKDIR}

#for ORD in LO NLO
#for ORD in LO
for ORD in NLO
do 
time ./testG1h --polPDF=../browser/compare_polPDF.xml \
--FF=../browser/compare_FF.xml --order=${ORD} --writeDB

done
