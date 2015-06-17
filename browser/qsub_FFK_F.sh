#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_FFK_F.sh.*

IDIR=/home/imazu/work/QCD/tmpFitKA

Q2SCALE=10

for SORT in FFK
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
      --${SORT}=${IDIR}/Scale_KA_FF_Final/FFK_PART_efx/100309195106_Multi_FFK_NLO_0.05-0.8_werr.xml \
      --comp \
      --wError

#      --${SORT}=${IDIR}/Scale_KA_FF_Final/FFK_FULL/100309102013_Multi_FFK_NLO_0.05-0.8_werr.xml \
#      --${SORT}=${IDIR}/Scale_KA_FF_Final/FFK_PART_effx/100309102054_Multi_FFK_NLO_0.05-0.8_werr.xml \
#      --${SORT}=${IDIR}/Scale_KA_FF_Final/FFK_PART_dgfx_effx/100309103529_Multi_FFK_NLO_0.05-0.8_werr.xml \

#### Dchi2 != 1 ####
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_KA_FF/FF_PART_wg_woDEL_B/100211162230_Multi_FFK_NLO_0.05-0.8_werr.xml \
#      --Dchi2=7.13 \
#      --comp \
#      --wError 

###### rescale ####
#### 0.25 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_FF_Final_SCALE/FF_PART_cdfx_SCALE/091218011043_Multi_FF_NLO_0.05-0.8_FFr0.25_UPDFr0.25_PPDFr0.25_werr.xml \
#      --rescaleFF=0.25
#### 0.5 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_FF_Final_SCALE/FF_PART_cdfx_SCALE/091218011043_Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_PPDFr0.5_werr.xml \
#      --rescaleFF=0.5
#### 2.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_FF_Final_SCALE/FF_PART_cdfx_SCALE/091218011043_Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_PPDFr2_werr.xml \
#      --rescaleFF=2.0
#### 4.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_FF_Final_SCALE/FF_PART_cdfx_SCALE/091218011043_Multi_FF_NLO_0.05-0.8_FFr4_UPDFr4_PPDFr4_werr.xml \
#      --rescaleFF=4.0

done
done
done
done
