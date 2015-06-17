#!/bin/bash
if [ -z "$1" ]; then
cd ${PBS_O_WORKDIR}
fi

rm -rf ./dist_plot.sh.*

source ~/mellin/bin/setup.sh

[ ! -e "./dist_direct" ] && mkdir -p ./dist_direct
cd dist_direct

BASEDIR=/home/imazu/work/QCD/tmpFit
FFBASE=${BASEDIR}/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final
FFSCALEBASE=${BASEDIR}/Scale_PI_FF_Final_SCALE_Study_Final

for Q2 in 1 4 10 30 100
#for Q2 in 1
do

for Flavor in -2 0 1 2 3 4 5
#for Flavor in 0
do

time dist_plot --DSSFF=DSS05_FF.xml --KretzerFF=KretzerNLO.xml \
--FF=${FFBASE}/FF_FULL_MultConstNf/100319223731_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFSCALE1=${FFSCALEBASE}/FF_PART_cdfx_cfree_MultConstNf_SCALE/100407220313_Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_PPDFr0.5_werr.xml \
--FFSCALE2=${FFSCALEBASE}/FF_PART_cdfx_cfree_MultConstNf_SCALE/100407214616_Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_PPDFr2_werr.xml \
--q2=${Q2} --flavor=${Flavor} --logx \
--setLC1=2 --setLW1=3 --setLC4=4 --setLC5=1 --setLW4=2 --setLW5=2 \
--setLC6=2 --setLW6=2 --setFC6=2 --setFS6=3001 \
--setLC7=3 --setLW7=2 --setFC7=3 --setFS7=3006 \
--setLC8=1 --setLW8=2 --setFC8=1 --setFS8=3002 \
--flegend6 --fillplot6 \
--flegend7 --fillplot7 \
--flegend8 --fillplot8 \
--noplot2 --noplot3 --nolegend2 --nolegend3 --zerohorizon \
--legend1="Present Analysis" --legend4="Kretzer00" --legend5="DSS07" \
--legend6="Stat. Err." --legend7="Tolerance(90%CL)" --legend8="Scale(1/2<#mu<2)" \
--tolerance=3.19 --xdiv=50 --fill --scale --scalekey=FFr --noheader --format=png --nolegend
# --setFS8=3013 \
# --noplot1 --noplot2 --nolegend1 --nolegend2 \
# --nodisplay 
# --nolegend 

time dist_plot --DSSFF=DSS05_FF.xml --KretzerFF=KretzerNLO.xml \
--FF=${FFBASE}/FF_FULL_MultConstNf/100319223731_Multi_FF_NLO_0.05-0.8_werr.xml \
--FFSCALE1=${FFSCALEBASE}/FF_PART_cdfx_cfree_MultConstNf_SCALE/100407220313_Multi_FF_NLO_0.05-0.8_FFr0.5_UPDFr0.5_PPDFr0.5_werr.xml \
--FFSCALE2=${FFSCALEBASE}/FF_PART_cdfx_cfree_MultConstNf_SCALE/100407214616_Multi_FF_NLO_0.05-0.8_FFr2_UPDFr2_PPDFr2_werr.xml \
--q2=${Q2} --flavor=${Flavor} --logx \
--setLC1=2 --setLW1=3 --setLC4=4 --setLC5=1 --setLW4=2 --setLW5=2 \
--setLC6=2 --setLW6=2 --setFC6=2 --setFS6=3001 \
--setLC7=3 --setLW7=2 --setFC7=3 --setFS7=3006 \
--setLC8=1 --setLW8=2 --setFC8=1 --setFS8=3002 \
--flegend6 --fillplot6 \
--flegend7 --fillplot7 \
--flegend8 --fillplot8 \
--noplot2 --noplot3 --nolegend2 --nolegend3 --zerohorizon \
--legend1="Present Analysis" --legend4="Kretzer00" --legend5="DSS07" \
--legend6="Stat. Err." --legend7="Tolerance(90%CL)" --legend8="Scale(1/2<#mu<2)" \
--tolerance=3.19 --xdiv=50 --fill --scale --scalekey=FFr --noheader --format=eps --nolegend
# --setFS8=3013 \
# --noplot1 --noplot2 --nolegend1 --nolegend2 \
# --nodisplay 
# --nolegend 

done

done