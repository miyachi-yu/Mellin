#!/bin/bash
if [ -z "$1" ]; then
cd ${PBS_O_WORKDIR}
fi

rm -rf ./residual_dist_m.*

source ~/mellin/bin/setup.sh

[ ! -e "./dist_residual_map" ] && mkdir -p ./dist_residual_map
cd dist_residual_map

BASEDIR=/home/imazu/work/QCD/tmpFit
FFBASE=${BASEDIR}/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final
FFSCALEBASE=${BASEDIR}/Scale_PI_FF_Final_SCALE_Study_Final

#FFSCALEDIR=FF_PART_cdfx_cfree_MultConstNf_SCALE
#FFSCALEDIR=FF_FULL_MultConstNf_SCALE
#for FFSCALEDIR in FF_PART_cdfx_cfree_MultConstNf_SCALE FF_FULL_MultConstNf_SCALE
#for FFSCALEDIR in FF_FULL_wdcd_MultConstNf_SCALE
#for FFSCALEDIR in FF_FULL_wudef_MultConstNf_SCALE
for FFSCALEDIR in FF_FULL_MultConstNf_W_Ns_SCALE FF_FULL_MultConstNf_W_Ns_SCALE_W_NORM_3
do

for Q2 in 10 1 4 30 100
do

  SFILELIST=`ls ${FFSCALEBASE}/${FFSCALEDIR} | grep "Multi_"`

  for SFILE1 in ${SFILELIST}
  do

  for SFILE2 in ${SFILELIST}
  do

  for Flavor in -2 0 1 2 3 4 5
  do

    time residual_dist --DSSFF=DSS05_FF.xml --KretzerFF=KretzerNLO.xml \
	--FF=${FFBASE}/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml \
	--FFSCALE1=${FFSCALEBASE}/${FFSCALEDIR}/${SFILE1} \
	--FFSCALE2=${FFSCALEBASE}/${FFSCALEDIR}/${SFILE2} \
	--q2=${Q2} --flavor=${Flavor} --logx \
	--setLC3=4 --setLC4=1 --setLW3=2 --setLW4=2 \
	--setFC5=1 --setFS5=3002 \
	--setLC5=1 --setLW5=2 --flegend5 --fillplot5 \
	--noplot1 --noplot2 --nolegend1 --nolegend2 \
	--legend3="Kretzer00" --legend4="DSS07" --legend5="Scale(1/2<#mu<2)" \
	--tolerance=3.19 --xytitle="z,( Dist - Fit ) / #sigma_{Fit}" \
	--xdiv=200 --fill --scale --scalekey=FFr --nolegend --format=png --nodisplay 
# --nolegend5 \
#	--FF=${FFBASE}/FF_FULL_MultConstNf/100319223731_Multi_FF_NLO_0.05-0.8_werr.xml \
#	--FF=${FFBASE}/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml \

  done

  done

  done

done

done
