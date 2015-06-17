#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_posi.sh.*

Q2SCALE=100

#for SORT in MRST
#for SORT in FF
#for SORT in polPDF
for SORT in DSSV
do
for ORD in NLO
do  
for FIX in dynamic 
do
	
  time  ./positivity  --dtype=${SORT} \
      --dtype2=MRST \
      --MRST=../config/MRST2001.xml \
      --nf=${FIX} --order=${ORD} --writeDB \
      --flav=1 --param=1 --da=0.2 \
      --npow=1 \
      --straight --addl=100.0 \
      --${SORT}=../config/DSSV08.xml
#      --${SORT}=./compare_${SORT}.xml 
#      --fixQ2=${Q2SCALE} \ --straight \
#      --straight --addl=100.0 \
#      --nreal=100.0 --nimag=0.0 \
#      --negative \

done
done
done
