#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_polPDF.sh.*

Q2SCALE=10

for SORT in DSSV
do

for ORD in NLO
do
    
for FIX in fix 
do
      
for OUTPUT in org
do

####### My Calc ######
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=NLO \
#      --${SORT}=polPDF_Final/A1h_pPDF_NLO_0.8-0.2_werr_FBB.xml \
#      --kret \
#      --comp \
#      --wError 

##### My Calc ######
  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
      --nf=${FIX} --order=NLO \
      --${SORT}=/home/imazu/work/QCD/polPDF_fit/confirm/POLPDF/COMP/FF/0907231052_A1h_pPDF_NLO_0.8-0.2_fx.xml \
      --kret
	
###### rescale ####
### 0.5 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=NLO \
#      --${SORT}=polPDF_Final/A1h_pPDF_NLO_0.8-0.2_FFr0.5_UPDFr0.5_PPDFr0.5_werr.xml \
#      --kret \
#      --rescaleFF=0.5
### 1.5 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=NLO \
#      --${SORT}=polPDF_Final/A1h_pPDF_NLO_0.8-0.2_FFr1.5_UPDFr1.5_PPDFr1.5_werr.xml \
#      --kret \
#      --rescaleFF=1.5
### 2.0 ##
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=NLO \
#      --${SORT}=polPDF_Final/A1h_pPDF_NLO_0.8-0.2_FFr2_UPDFr2_PPDFr2_werr.xml \
#      --kret \
#      --rescaleFF=2.0

###### DSSV ######
#  time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#      --nf=${FIX} --order=NLO \
#      --${SORT}=${SORT}08.xml 

####### GRSV00 ######
#  time  ./main  --dtype=GRSV00 --otype=${OUTPUT} \
#      --nf=${FIX} --order=NLO \
#      --${SORT}=${SORT}08.xml \
#      --GRSV2000=../PDFLIB/grsv2000_table 

done
done
done
done
