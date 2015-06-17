#!/bin/bash
cd ${PBS_O_WORKDIR}
#rm -rf ~/.mellin
rm -rf ./qsub_xc.sh.*
#make clean
cd ../
#make
source ~/work/QCD/Mellin/setup.sh
cd ${PBS_O_WORKDIR}

for ORD in LO NLO
#for ORD in LO
#for ORD in NLO
do 
time ./main  --FF=../browser/compare_FF.xml --order=${ORD} \
--writeDB
done
