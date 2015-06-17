#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_FF.sh.*

Q2SCALE=10

#for SORT in FF FFK FFP FFR
#for SORT in FF 
#for SORT in FFK FFP FFR 
#for SORT in FFP
for SORT in FF FFK
do

for ORD in NLO
do
    
for FIX in dynamic 
do
      
for OUTPUT in org
do
	
####### My Calc ######
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=/home/imazu/work/QCD/FF_fit_all/PR_FF/FF/best/0907101722_Multi_FFP_NLO_0.05-0.8.xml
##      --${SORT}=/home/imazu/work/QCD/FF_fit_all/PR_FF/FF/tmpbest/0907101819_Multi_FFP_NLO_0.05-0.8.xml
##      --${SORT}=/home/imazu/work/QCD/FF_fit_all/PR_FF/FF/best/0907101722_Multi_FFP_NLO_0.05-0.8.xml
##      --${SORT}=FF_Final/Multi_${SORT}_NLO_0.05-0.8_werr_FBB.xml \
##      --comp \
##      --wError 

##### rescale ####
### 0.5 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=FF_Final/Multi_${SORT}_NLO_0.05-0.8_FFr0.5_UPDFr0.5_werr.xml \
#      --rescaleFF=0.5
### 1.5 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=FF_Final/Multi_${SORT}_NLO_0.05-0.8_FFr1.5_UPDFr1.5_werr.xml \
#      --rescaleFF=1.5
### 2.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --${SORT}=FF_Final/Multi_${SORT}_NLO_0.05-0.8_FFr2_UPDFr2_werr.xml \
#      --rescaleFF=2.0

###### Kretzer00 ######
  if [ ${SORT} == FF ]; then
      time  ./main  --dtype=Kretzer${SORT} --otype=${OUTPUT} \
	  --nf=${FIX} --order=${ORD} \
	  --Kretzer${SORT}=Kretzer${ORD}.xml  --kret 
  else
      if [ ${SORT} == FFK ]; then
	  time  ./main  --dtype=Kretzer${SORT} --otype=${OUTPUT} \
	      --nf=${FIX} --order=${ORD} \
	      --Kretzer${SORT}=Kretzer${ORD}_K.xml  --kret       
      else
	  if [ ${SORT} == FFP ]; then
	      time  ./main  --dtype=Kretzer${SORT} --otype=${OUTPUT} \
		  --nf=${FIX} --order=${ORD} \
		  --Kretzer${SORT}=Kretzer${ORD}_R.xml  --kret       
	  fi
      fi
  fi

###### DSS05 ######
  time  ./main  --dtype=DSS${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=${ORD} \
      --DSS${SORT}=DSS05_${SORT}.xml  --angle=0.55 

done
done
done
done


#for SORT in hadronFF
#do
#for ORD in NLO
#do  
#for FIX in dynamic 
#do    
#for OUTPUT in org
#do
#	
###### My Calc ######
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} \
#      --FF=FF_Final/Multi_FF_NLO_0.05-0.8_werr_FBB.xml \
#      --FFK=FF_Final/Multi_FFK_NLO_0.05-0.8_werr_FBB.xml \
#      --FFP=FF_Final/Multi_FFP_NLO_0.05-0.8_werr_FBB.xml \
#      --FFR=FF_Final/Multi_FFR_NLO_0.05-0.8_werr_FBB.xml \
##      --comp \
##      --wError 
#
####### Kretzer00 ######
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} --KretzerH \
#      --KretzerFF=Kretzer${ORD}.xml \
#      --KretzerFFK=Kretzer${ORD}_K.xml \
#      --KretzerFFR=Kretzer${ORD}_R.xml  --kret 
#
####### DSS05 ######
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=${ORD} --DSSH \
#      --DSSFF=DSS05_FF.xml \
#      --DSSFFK=DSS05_FFK.xml \
#      --DSSFFP=DSS05_FFP.xml \
#      --DSSFFR=DSS05_FFR.xml 
#
#done
#done
#done
#done
