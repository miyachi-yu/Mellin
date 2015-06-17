#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_werr.sh.*

#for SORT in FF polPDF unpolPDF
#for SORT in FF polPDF
#for SORT in FF
for SORT in FFK
#for SORT in KretzerFF
#for SORT in KretzerFFK
#for SORT in polPDF
do

#for ORD in LO
#for ORD in LO NLO
for ORD in NLO
do

#for FIX in fix dynamic
#for FIX in fix
for FIX in dynamic
do

#for OUTPUT in org singlet valence
#for OUTPUT in valence 
#for OUTPUT in singlet
for OUTPUT in org 
do

if [ ${SORT} != KretzerFF ]
    then

    if [ ${SORT} != KretzerFFK ]
	then

	time ./main --dtype=${SORT} --otype=${OUTPUT} \
	    --nf=${FIX} --order=${ORD} \
	    --${SORT}=../../fit_work/defence_plot/kaon/Multi_FFK_NLO_0.05-0.8_werr_org.xml \
	    --comp \
	    --wError --fixQ2=100.0

#	    --${SORT}=../../fit_work/defence_plot/pion/Multi_FF_NLO_0.05-0.8_werr_org.xml \
#	    --${SORT}=./compare_${SORT}.xml 
	
    else

	time ./main --dtype=${SORT} --otype=${OUTPUT} \
	    --${SORT}=../Fitting/FFK_${ORD}_0-0.05-0.8_werr.xml \
	    --nf=${FIX} --order=${ORD} \

    fi

else

    time ./main --dtype=${SORT} --otype=${OUTPUT} \
	--nf=${FIX} --order=${ORD} --writeDB \
	--${SORT}=../Fitting/FF_${ORD}_0-0.05-0.8_werr.xml \
	--wError --comp

fi

done
done
done
done
