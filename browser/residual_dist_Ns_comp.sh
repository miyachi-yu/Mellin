#!/bin/bash
if [ -z "$1" ]; then
cd ${PBS_O_WORKDIR}
fi

rm -rf ./residual_dist_N.*

source ~/mellin/bin/setup.sh

[ ! -e "./dist_residual_Ns_comp" ] && mkdir -p ./dist_residual_Ns_comp
cd dist_residual_Ns_comp

BASEDIR=/home/imazu/work/QCD/tmpFit
FFBASE=${BASEDIR}/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final

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
--FFADD1=${FFBASE}/FF_FULL_MultConstNf/100319223731_Multi_FF_NLO_0.05-0.8_werr.xml \
--q2=${Q2} --flavor=${Flavor} --logx \
--setLC1=1 --setLW1=2 --setLS1=5 \
--setLC2=4 --setLW2=2 --setLS2=2 \
--setLC3=1 --setLW3=2 --setLS3=1 \
--setLC4=2 --setLW4=3 --setLS4=1 \
--setFC5=3 --setFS5=3001 \
--setLC5=3 --setLW5=2 --flegend5 --fillplot5 \
--setFC6=2 --setFS6=3001 \
--setLC6=2 --setLW6=2 --flegend6 --fillplot6 \
--tgorder=5,6,4,2,3,1 \
--legend2="Kretzer00" --legend3="DSS07" \
--legend4="Present Analysis" \
--legend5="Tolerance(90%CL)" --legend6="Stat. Err." \
--nolegend1 --nolegend4 \
--legparam=0.12,0.65,0.42,0.88 --lgtxtsize=0.040 \
--lorder=4,6,5,2,3,1 \
--addlegend \
--addlegend1="w/o Ns (Q^{2} > 2.0, z_{H} < 0.6)" \
--noaddlegend2 --noaddlegend3 --noaddlegend4 --noaddlegend5 --noaddlegend6 \
--addlegparam=0.13,0.17,0.50,0.32 --addlgtxtsize=0.045 \
--addlorder=6,5,4,3,2,1 \
--tolerance=2.91 \
--xytitle="z,#Delta(zD^{\pi+}) / #sigma_{TOL}" \
--xdiv=50 --fill --format=${FOM}
#--noplot5 --nolegend5 \
#--noheader \
#--xytitle="z,( Dist - Fit ) / #sigma_{Fit}" \

### W/ Ns ###
#--FF=${FFBASE}/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml \
#--FFADD1=${FFBASE}/FF_FULL_MultConstNf/100319223731_Multi_FF_NLO_0.05-0.8_werr.xml \
#--addlegend1="w/o Ns (Q^{2} > 2.0, z_{H} < 0.6)" \

### W/O Ns ###
#--FF=${FFBASE}/FF_FULL_MultConstNf/100319223731_Multi_FF_NLO_0.05-0.8_werr.xml \
#--FFADD1=${FFBASE}/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml \
#--addlegend1="w/ Ns (Q^{2} > 2.0, z_{H} < 0.6)" \

### W/ diff. cond. ###
#--FFADD2=${FFBASE}/FF_FULL_MultConstNf_Q2lim1.5_MZ0.6_W_NORM/100422215612_Multi_FF_NLO_0.05-0.8_werr.xml \
#--legend2="Q^{2} > 1.5, z < 0.6" \

#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_Q2lim1.5_MZ0.8_W_NORM/100422215647_Multi_FF_NLO_0.05-0.8_werr.xml \
#--legend3="Q^{2} > 1.5, z < 0.8" \

#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_Q2lim1.0_W_NORM/100421185530_Multi_FF_NLO_0.05-0.8_werr.xml \
#--legend3="Q^{2} > 1.0, z < 0.6" \

#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_Q2lim1.0_MZ0.8_W_NORM/100422215713_Multi_FF_NLO_0.05-0.8_werr.xml \
#--legend3="Q^{2} > 1.0, z < 0.8" \

#--FFADD3=${FFBASE}/FF_FULL_MultConstNf_Q2lim1.0_MZ0.6_W_NORM/100422215536_Multi_FF_NLO_0.05-0.8_werr.xml \
#--legend3="Q^{2} > 1.0, z < 0.6" \

done

done

done