#!/bin/bash
if [ -z "$1" ]; then
cd ${PBS_O_WORKDIR}
fi

rm -rf ./residual_dist_N.*

source ~/mellin/bin/setup.sh

[ ! -e "./dist_residual_Ns_FuncForm" ] && mkdir -p ./dist_residual_Ns_FuncForm
cd dist_residual_Ns_FuncForm

BASEDIR=/home/imazu/work/QCD/tmpFit
FFBASE=${BASEDIR}/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final
FFSCALEBASE=${BASEDIR}/Scale_PI_FF_Final_SCALE_Study_Final

#for Q2 in 1 4 10 30 100
for Q2 in 10
do

for Flavor in -2 0 1 2 3 4 5
#for Flavor in 0
do

for FOM in png eps
#for FOM in png
do

time residual_dist --DSSFF=DSS05_FF.xml --KretzerFF=KretzerNLO.xml \
--FF=${FFBASE}/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFADD1=${FFBASE}/FF_FULL_MultConstNf_W_Ns_FuncForm_W_NORM_Ucd/100702085439_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFADD2=${FFBASE}/FF_FULL_MultConstNf_W_Ns_FuncForm_W_NORM_UDef/100702085425_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFADD3=${FFBASE}/FF_FULL_MultConstNf_W_Ns_FuncForm_W_NORM_UDcd/100702085402_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFADD4=${FFBASE}/FF_FULL_MultConstNf_W_Ns_FuncForm_W_NORM_UcdefDcd/100702090443_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFADD5=${FFBASE}/FF_FULL_MultConstNf_W_Ns_FuncForm_W_NORM_UDcdef/100702085420_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFADD6=${FFBASE}/FF_FULL_MultConstNf_W_Ns_FuncForm_W_NORM_Uef/100702104404_Multi_FF_NLO_0.05-0.8_werr.xml \
--q2=${Q2} --flavor=${Flavor} --logx \
--setLC1=1 --setLW1=2 --setLS1=1 \
--setLC2=1 --setLW2=2 --setLS2=2 \
--setLC3=1 --setLW3=2 --setLS3=3 \
--setLC4=1 --setLW4=2 --setLS4=4 \
--setLC5=1 --setLW5=2 --setLS5=5 \
--setLC6=1 --setLW6=2 --setLS6=6 \
--setLC7=4 --setLW7=2 --setLS7=2 \
--setLC8=1 --setLW8=2 --setLS8=1 \
--setLC9=2 --setLW9=3 --setLS9=1 \
--setFC10=3 --setFS10=3001 \
--setLC10=3 --setLW10=2 --flegend10 --fillplot10 \
--setFC11=2 --setFS11=3001 \
--setLC11=2 --setLW11=2 --flegend11 --fillplot11 \
--tgorder=10,11,9,7,8,1,2,3,4,5,6 \
--noplot1 --noplot2 \
--noplot7 \
--legend7="Kretzer00" --legend8="DSS07" \
--legend9="Present Analysis" \
--legend10="Tolerance(90%CL)" --legend11="Stat. Err." \
--nolegend1 --nolegend2 --nolegend3 \
--nolegend4 --nolegend5 --nolegend6 \
--nolegend7 --nolegend9 \
--legparam=0.12,0.65,0.42,0.88 --lgtxtsize=0.040 \
--lorder=9,11,10,7,8,1,2,3,4,5,6 \
--addlegend \
--addlegend1="w/ param. (Ucd)" \
--addlegend2="w/ param. (UDef)" \
--addlegend3="w/ param. (UDcd)" \
--addlegend4="w/ param. (UcdefDcd)" \
--addlegend5="w/ param. (UDcdef)" \
--addlegend6="w/ param. (Uef)" \
--noaddlegend1 --noaddlegend2 \
--noaddlegend7 --noaddlegend8 --noaddlegend9 --noaddlegend10 --noaddlegend11 \
--addlegparam=0.13,0.14,0.50,0.30 --addlgtxtsize=0.045 \
--addlorder=10,9,7,8,1,2,3,4,5,6 \
--tolerance=2.91 \
--xytitle="z,#Delta(zD^{\pi+}) / #sigma_{TOL}" \
--xdiv=50 --fill --format=${FOM}
#--noplot1 --noplot2 --noplot4 \
#--noaddlegend1 --noaddlegend2 --noaddlegend4 \

#--noplot5 --nolegend5 \
#--noheader \
#--xytitle="z,( Dist - Fit ) / #sigma_{Fit}" \

##### CUT ORG #####
#--FFADD1=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim2.0_MZ0.8_W_NORM/100629212406_Multi_FF_NLO_0.05-0.8_werr.xml \
#--FFADD2=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.5_MZ0.6_W_NORM/100629212459_Multi_FF_NLO_0.05-0.8_werr.xml \
#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.5_MZ0.8_W_NORM/100629212433_Multi_FF_NLO_0.05-0.8_werr.xml \
#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.0_MZ0.6_W_NORM/100629212537_Multi_FF_NLO_0.05-0.8_werr.xml \
#--addlegend3="w/ cond. (Q^{2} > 1.0, z_{H} < 0.6)" \
#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.0_MZ0.8_W_NORM/100629224125_Multi_FF_NLO_0.05-0.8_werr.xml \
#--addlegend3="w/ cond. (Q^{2} > 1.0, z_{H} < 0.8)" \

##### CUT NEW #####
#--FFADD1=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim2.0_MZ0.8_W_NORM/100630040611_Multi_FF_NLO_0.05-0.8_werr.xml \
#--FFADD2=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.5_MZ0.6_W_NORM/100630040720_Multi_FF_NLO_0.05-0.8_werr.xml \
#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.5_MZ0.8_W_NORM/100630040657_Multi_FF_NLO_0.05-0.8_werr.xml \
#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.0_MZ0.6_W_NORM/100630040758_Multi_FF_NLO_0.05-0.8_werr.xml \
#--addlegend3="w/ cond. (Q^{2} > 1.0, z_{H} < 0.6)" \
#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.0_MZ0.8_W_NORM/100630070619_Multi_FF_NLO_0.05-0.8_werr.xml \
#--addlegend3="w/ cond. (Q^{2} > 1.0, z_{H} < 0.8)" \

done

done

done