#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_exp.sh.*

#for SORT in FF polPDF unpolPDF
#for SORT in FF polPDF
#for SORT in FF
#for SORT in KretzerFF
#for SORT in polPDF
for SORT in MRST
do

for ORD in NLO
#for ORD in LO  
#for ORD in LO NLO
do
    
  if [ ${SORT} != KretzerFF ]; then
	
      if [ ${SORT} != MRST ]; then
	  
	  time  ./main_exp  --dtype=${SORT} \
	      --order=${ORD} --writeDB \
	      --${SORT}=./compare_${SORT}.xml
	  
      else
	  
	  time  ./main_exp  --dtype=${SORT} \
	      --order=NLO --writeDB \
	      --${SORT}=../config/${SORT}.xml \
	      --angle=0.55 --offset=1.5 --length=15 \
	      --precision=1.0E-6
#		--MRSTMc=1.6
#	  --MRSTMz=0.1175
	  
      fi
      
  else
      
      time  ./main_exp  --dtype=${SORT} \
	  --order=${ORD} --writeDB \
	  --${SORT}=../config/Kretzer${ORD}.xml --kret
      
      time  ./main_exp  --dtype=${SORT} \
	  --order=${ORD} --writeDB \
	  --${SORT}=../Fitting/FF_${ORD}_0-0.05-0.8_werr.xml

      for RCL in 0.25 4;do
	  time  ./main_exp  --dtype=${SORT} \
	      --order=${ORD} --writeDB \
	      --${SORT}=../Fitting/FF_${ORD}_0-0.05-0.8_werr.xml \
	      --rescaleFF=${RCL}
      done
      
      time  ./main_exp  --dtype=${SORT} \
	  --order=${ORD} --writeDB \
	  --${SORT}=../Fitting/FF_${ORD}_0-0.05-0.8_werr.xml \
	  --wError
      
  fi	
  
done
done
