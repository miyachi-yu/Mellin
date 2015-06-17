#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub.sh.*

#Q2SCALE=100
Q2SCALE=1
#Q2SCALE=4

#for SORT in FF polPDF unpolPDF
#for SORT in FF polPDF
#for SORT in FFK
#for SORT in FF
#for SORT in KretzerFF
#for SORT in KretzerFFK
#for SORT in polPDF
#for SORT in MRST
for SORT in DSSV
#for SORT in DSSV KretzerFF MRST
do

for ORD in NLO
#for ORD in LO  
#for ORD in LO NLO
do
    
#for FIX in fix dynamic
for FIX in dynamic 
do
      
#for OUTPUT in org singlet valence
#for OUTPUT in valence 
#for OUTPUT in singlet
for OUTPUT in org
do
	
    if [ ${SORT} != KretzerFF ]; then
	
	if [ ${SORT} != DSSV ]; then

	    if [ ${SORT} != MRST ]; then
		
		if [ ${SORT} != KretzerFFK ]; then
		    
		    time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
			--nf=${FIX} --order=${ORD} \
			--${SORT}=./../../fit_work/FFPI_fit/set2_r1.5/Multi_FF_NLO_0.05-0.8_FFr1.5_UPDFr1.5_werr.xml \
			--rescaleFF=1.5
#			--${SORT}=./../../fit_work/FFPI_fit/set2_r2.0/Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_werr.xml \
#			--rescaleFF=2.0
#			--${SORT}=./../../fit_work/FFPI_fit/set2_r0.5/Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_werr.xml \
#			--rescaleFF=0.5
#--${SORT}=./../../fit_work/FFPI_fit/set2/Multi_FF_NLO_0.05-0.8_werr_FBBF.xml \
#			--comp \
#			--wError
#			--${SORT}=./compare_${SORT}.xml

#		    time  ./main  --dtype=DSSFFK --otype=${OUTPUT} \
#			--nf=${FIX} --order=${ORD}  \
#			--DSSFFK=../config/DSS05_FFK.xml --angle=0.55
		    
		else
		    
		    time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
			--nf=${FIX} --order=${ORD}  \
			--${SORT}=../config/Kretzer${ORD}_K.xml  --kret
		    
		    time ./main --dtype=${SORT} --otype=${OUTPUT} \
			--nf=${FIX} --order=${ORD}  \
			--${SORT}=../Fitting/FFK_${ORD}_0-0.05-0.8_werr.xml

		fi
		
	    else
		
		time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
		    --nf=${FIX} --order=NLO \
		    --${SORT}=../config/${SORT}.xml \
		    --angle=0.55 --offset=1.8 --length=15 \
		    --MRSTMz=0.1175 \
		    --precision=1.0E-4 \
		    --fixQ2=${Q2SCALE} \
		    --MRSTMc=1.4
#		--fixQ2=1
#		--fixQ2=4
	    fi

	else

	    time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
		--nf=fix --order=NLO \
		--${SORT}=A1h_pPDF_NLO_0.8-0.2_werr_FBB.xml \
		--fixQ2=${Q2SCALE} \
		--kret \
		--comp \
		--wError

#		--${SORT}=../../fit_work/polPDF_A1h_nocond_ubardbar_Final_3_1/present_params_0.xml \
#		--${SORT}=../../fit_work/polPDF_A1h_nocond_Final_1/present_params_0.xml \
#		--${SORT}=../../fit_work/polPDF_A1h_nocond_ubardbar_5_4_3_1/A1h_pPDF_NLO_0.8-0.2_werr.xml \

#		--${SORT}=../../fit_work/polPDF_A1h_nocond_ubardbar_5_4_3_1_0/present_params_0.xml \
#		--${SORT}=../../fit_work/polPDF_A1h_nocond_ubardbar_5_4_3_1/A1h_pPDF_NLO_0.8-0.2_werr_BBB.xml \
#		--${SORT}=../../fit_work/polPDF_A1h_nocond_ubardbar_5_4_3_1/present_params_0.xml \
#		--${SORT}=../../fit_work/polPDF_A1h_nocond_ubardbar_5/A1h_pPDF_NLO_0.8-0.2_werr_BB.xml \
#		--${SORT}=../../fit_work/polPDF_A1h_nocond_ubardbar_5/A1h_pPDF_NLO_0.8-0.2_werr_org.xml \
#		--${SORT}=../../fit_work/polPDF_A1h_nocond_ubardbar_9/present_params_0_tmp.xml \
#		--${SORT}=../../fit_work/polPDF_nocond_tmp2/A1h_pPDF_NLO_0.8-0.2_werr.xml \
#		--${SORT}=../../fit_work/polPDF_nocond_tmp/present_params_0_tmp_tmp.xml \
#		--${SORT}=../../fit_work/polPDF_nocond/A1h_pPDF_NLO_0.8-0.2_werr.xml \
#		--${SORT}=../../fit_work/polPDF_nocond/A1h_pPDF_NLO_0.8-0.2_werr_BBBB.xml \

#	    time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#		--nf=fix --order=NLO \
#		--${SORT}=../config/${SORT}08.xml \
#		--angle=0.55 --offset=1.0 --length=15 \
#		--precision=1.0E-4 \
#		--alphaDSSVMz=0.1195 \

#		--fixQ2=${Q2SCALE} \
##		--${SORT}=../config/polPDF_${SORT}_imazu.xml \
#		--${SORT}=../config/polPDF_${SORT}_imazu_SU3.xml \
#		--alphaDSSVMz=0.1190 \

	fi

    else
	
#	time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#	    --nf=${FIX} --order=${ORD} \
#	    --${SORT}=../config/Kretzer${ORD}.xml  --kret \
#	    --angle=0.55 --offset=1.8 --length=15 \
#	    --precision=1.0E-4 \
#	    --fixQ2=${Q2SCALE} 
	
#	time  ./main  --dtype=DSSFF --otype=${OUTPUT} \
#	    --nf=${FIX} --order=${ORD}  \
#	    --DSSFF=../config/DSS05_NLO.xml --angle=0.55
#	
#	for RCL in 0.25 4;do
#	    
#	    time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
#		--nf=${FIX} --order=${ORD}  \
#		--${SORT}=../Fitting/FF_${ORD}_0-0.05-0.8_r${RCL}.xml \
#		--rescaleFF=${RCL}
#	    
#	done
#	

	time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
	    --nf=${FIX} --order=${ORD}  \
	    --${SORT}=./../../fit_work/FFPI_fit/kre_r0.5/Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_werr.xml \
	    --rescaleFF=0.5

	time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
	    --nf=${FIX} --order=${ORD}  \
	    --${SORT}=./../../fit_work/FFPI_fit/kre_r2.0/Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_werr.xml \
	    --rescaleFF=2.0

	time  ./main  --dtype=${SORT} --otype=${OUTPUT} \
	    --nf=${FIX} --order=${ORD}  \
	    --${SORT}=./../../fit_work/FFPI_fit/kre/Multi_FF_NLO_0.05-0.8_werr.xml \
	    --comp \
	    --wError
	
    fi	
    
done
done
done
done
