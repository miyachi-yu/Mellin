#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_KRE_F.sh.*

IDIR=/home/imazu/work/QCD/tmpFit

Q2SCALE=10

for SORT in KretzerFF 
do

for ORD in NLO
do
    
for FIX in dynamic 
do
      
for OUTPUT in org
do

####### KRE_PART_wgb ########	
###### My Calc ######
### Dchi2 = 1 ####
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --${SORT}=${IDIR}/Scale_PI_KRE_ALL/KRE_PART_wgb_woDEL_B/091215030419_Multi_FF_NLO_0.05-0.8_werr.xml \
      --comp \
      --wError 

### Dchi2 != 1 ####
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --${SORT}=${IDIR}/Scale_PI_KRE_ALL/KRE_PART_wgb_woDEL_B/091215030419_Multi_FF_NLO_0.05-0.8_werr.xml \
      --Dchi2=6.76 \
      --comp \
      --wError 


##### rescale ####
### 0.25 ##
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_PART_wgb_woDEL_B_SCALE/091215192222_Multi_FF_NLO_0.05-0.8_FFr0.25_UPDFr0.25_PPDFr0.25_werr.xml \
      --rescaleFF=0.25
### 0.5 ##
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_PART_wgb_woDEL_B_SCALE/091215192222_Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_PPDFr0.5_werr.xml \
      --rescaleFF=0.5
### 2.0 ##
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_PART_wgb_woDEL_B_SCALE/091215192222_Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_PPDFr2_werr.xml \
      --rescaleFF=2.0
### 4.0 ##
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_PART_wgb_woDEL_B_SCALE/091215192222_Multi_FF_NLO_0.05-0.8_FFr4_UPDFr4_PPDFr4_werr.xml \
      --rescaleFF=4.0



######### KRE_PART ########
####### My Calc ######
#### Dchi2 = 1 ####
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL/KRE_PART_woDEL_B/091204035543_Multi_FF_NLO_0.05-0.8_werr.xml \
#      --comp \
#      --wError 
#
#### Dchi2 != 1 ####
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL/KRE_PART_woDEL_B/091204035543_Multi_FF_NLO_0.05-0.8_werr.xml \
#      --Dchi2=6.76 \
#      --comp \
#      --wError 
#
###### rescale ####
#### 0.25 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_PART_woDEL_B_SCALE/091215191230_Multi_FF_NLO_0.05-0.8_FFr0.25_UPDFr0.25_PPDFr0.25_werr.xml \
#      --rescaleFF=0.25
#### 0.5 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_PART_woDEL_B_SCALE/091215191230_Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_PPDFr0.5_werr.xml \
#      --rescaleFF=0.5
#### 2.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_PART_woDEL_B_SCALE/091215191230_Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_PPDFr2_werr.xml \
#      --rescaleFF=2.0
#### 4.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_PART_woDEL_B_SCALE/091215191230_Multi_FF_NLO_0.05-0.8_FFr4_UPDFr4_PPDFr4_werr.xml \
#      --rescaleFF=4.0


######### KRE_FULL ########
####### My Calc ######
#### Dchi2 = 1 ####
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL/KRE_FULL_woDEL_B/091204041940_Multi_FF_NLO_0.05-0.8_werr.xml \
#      --comp \
#      --wError 
#
#### Dchi2 != 1 ####
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL/KRE_FULL_woDEL_B/091204041940_Multi_FF_NLO_0.05-0.8_werr.xml \
#      --Dchi2=6.76 \
#      --comp \
#      --wError 
#
###### rescale ####
#### 0.25 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_FULL_woDEL_B_SCALE/100220011627_Multi_FF_NLO_0.05-0.8_FFr0.25_UPDFr0.25_PPDFr0.25_werr.xml \
#      --rescaleFF=0.25
#### 0.5 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_FULL_woDEL_B_SCALE/100220011626_Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_PPDFr0.5_werr.xml \
#      --rescaleFF=0.5
#### 2.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_FULL_woDEL_B_SCALE/100220011629_Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_PPDFr2_werr.xml \
#      --rescaleFF=2.0
#### 4.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=${IDIR}/Scale_PI_KRE_ALL_SCALE/KRE_FULL_woDEL_B_SCALE/100220011630_Multi_FF_NLO_0.05-0.8_FFr4_UPDFr4_PPDFr4_werr.xml \
#      --rescaleFF=4.0


done
done
done
done
