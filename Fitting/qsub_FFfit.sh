#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_FFfit.sh.*

for ORD in LO NLO
#for ORD in NLO
do
for RCL in 1.0 4.0 0.25
do

if test ${RCL} == 1.0
then
#time ./testFragXsecFit --KretzerFFK=../config/Kretzer${ORD}_K.xml \
time ./testFragXsecFit --KretzerFFK=../config/Kretzer${ORD}_K_g_fx.xml \
--order=${ORD} \
--writeDB --expdata=../ExpData/kretzerKA.xml \
--zmin=0.05 --zmax=0.8 --errorMatrix | tee tmp_FFfit.dat
#--length=15.0 \
fi

done
done

#if test ${RCL} == 1.0
#then
#time ./testFragXsecFit --KretzerFF=../config/Kretzer${ORD}.xml --order=${ORD} \
#--writeDB --expdata=../ExpData/kretzerPI.xml \
#--zmin=0.05 --zmax=0.8 --errorMatrix --length=15.0
#else
#time ./testFragXsecFit --KretzerFF=../config/Kretzer${ORD}.xml --order=${ORD} \
#--writeDB --expdata=../ExpData/kretzerPI.xml \
#--zmin=0.05 --zmax=0.8 --rescaleFF=${RCL} --length=15.0
#fi

