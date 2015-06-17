#!/bin/bash
if [ -z "$1" ]; then
cd ${PBS_O_WORKDIR}
fi

rm -rf ./residual_dist_N.*

source ~/mellin/bin/setup.sh

[ ! -e "./dist_residual_Ns" ] && mkdir -p ./dist_residual_Ns
cd dist_residual_Ns

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
--FFADD1=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim2.0_MZ0.8_W_NORM/100630040611_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFADD2=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.5_MZ0.6_W_NORM/100630040720_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFADD3=${FFBASE}/FF_FULL_MultConstNf_W_Ns_Q2lim1.5_MZ0.8_W_NORM/100630040657_Multi_FF_NLO_0.05-0.8_werr.xml \
--q2=${Q2} --flavor=${Flavor} --logx \
--setLC1=1 --setLW1=2 --setLS1=7 \
--setLC2=1 --setLW2=2 --setLS2=5 \
--setLC3=1 --setLW3=2 --setLS3=8 \
--setLC4=4 --setLW4=2 --setLS4=2 \
--setLC5=1 --setLW5=2 --setLS5=1 \
--setLC6=2 --setLW6=3 --setLS6=1 \
--setFC7=3 --setFS7=3001 \
--setLC7=3 --setLW7=2 --flegend7 --fillplot7 \
--setFC8=2 --setFS8=3001 \
--setLC8=2 --setLW8=2 --flegend8 --fillplot8 \
--tgorder=7,8,6,4,5,1,2,3 \
--noplot4 --nolegend4 \
--legend4="Kretzer00" --legend5="DSS07" \
--legend6="Present Analysis" \
--legend7="Tolerance(90%CL)" --legend8="Stat. Err." \
--nolegend1 --nolegend2 --nolegend3 --nolegend6 \
--legparam=0.12,0.65,0.42,0.88 --lgtxtsize=0.040 \
--lorder=6,8,7,4,5,1,2,3 \
--addlegend \
--addlegend1="w/ cond. (Q^{2} > 2.0, z_{H} < 0.8)" \
--addlegend2="w/ cond. (Q^{2} > 1.5, z_{H} < 0.6)" \
--addlegend3="w/ cond. (Q^{2} > 1.5, z_{H} < 0.8)" \
--noaddlegend4 --noaddlegend5 --noaddlegend6 --noaddlegend7 --noaddlegend8 \
--addlegparam=0.13,0.14,0.50,0.29 --addlgtxtsize=0.045 \
--addlorder=7,6,4,5,1,2,3 \
--tolerance=2.91 \
--xytitle="z,#Delta(zD^{\pi+}) / #sigma_{TOL}" \
--xdiv=50 --fill --format=${FOM}
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