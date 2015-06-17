#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_FF.sh.*

IDIR=/home/imazu/work/QCD/tmpFit

Q2SCALE=10

for SORT in FF 
do

for ORD in NLO
do
    
for FIX in dynamic 
do
      
for OUTPUT in org
do
	
###### My Calc ######
### Dchi2 = 1 ####
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --${SORT}=${IDIR}/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml \
      --comp \
      --wError 

### Dchi2 != 1 ####
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --${SORT}=${IDIR}/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml \
      --Dchi2=8.47 \
      --comp \
      --wError 
#      --Dchi2=9.0 \

###### rescale ####
#### 0.25 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_FF_Final_SCALE_Study_Final/FF_FULL_MultConstNf_SCALE/100407234702_Multi_FF_NLO_0.05-0.8_FFr0.25_UPDFr0.25_PPDFr0.25_werr.xml \
#      --rescaleFF=0.25
#### 0.5 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_FF_Final_SCALE_Study_Final/FF_FULL_MultConstNf_SCALE/100407232450_Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_PPDFr0.5_werr.xml \
#      --rescaleFF=0.5
#### 2.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_FF_Final_SCALE_Study_Final/FF_FULL_MultConstNf_SCALE/100407230525_Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_PPDFr2_werr.xml \
#      --rescaleFF=2.0
#### 4.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_FF_Final_SCALE_Study_Final/FF_FULL_MultConstNf_SCALE/100407231106_Multi_FF_NLO_0.05-0.8_FFr4_UPDFr4_PPDFr4_werr.xml \
#      --rescaleFF=4.0

done
done
done
done
