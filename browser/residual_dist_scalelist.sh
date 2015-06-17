#!/bin/bash
if [ -z "$1" ]; then
cd ${PBS_O_WORKDIR}
fi
rm -rf ./residual_dist_s.*

source ~/mellin/bin/setup.sh

[ ! -e "./dist_residual_scalelist" ] && mkdir -p ./dist_residual_scalelist
cd dist_residual_scalelist
/bin/rm -rf ./scalelist.dat

BASEDIR=/home/imazu/work/QCD/tmpFit
FFBASE=${BASEDIR}/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final
FFSCALEBASE=${BASEDIR}/Scale_PI_FF_Final_SCALE_Study_Final

#Q2LIST="1 1.19 1.41 1.68 2 2.38 2.83 3.37 4 4.16 5.66 6.74 8 \
#        0.84 0.71 0.59 0.5 0.41 0.35 0.30 0.25 0.21 0.18 0.15 0.125"
Q2LIST="0.5 0.71 0.84 1 1.19 1.41 1.68 2"
Q2BASE1="0.5"
Q2BASE2="2"

#for FFSCALEDIR in FF_PART_cdfx_cfree_MultConstNf_SCALE FF_FULL_MultConstNf_SCALE
#for FFSCALEDIR in FF_FULL_MultConstNf_SCALE FF_FULL_wdcd_MultConstNf_SCALE
#for FFSCALEDIR in FF_FULL_MultConstNf_SCALE
#for FFSCALEDIR in FF_PART_cdfx_cfree_MultConstNf_SCALE
#for FFSCALEDIR in FF_PART_cdfx_cfree_MultConstNf_SCALE FF_FULL_MultConstNf_SCALE FF_FULL_wdcd_MultConstNf_SCALE
#for FFSCALEDIR in FF_PART_cdfx_cfree_MultConstNf_SCALE FF_FULL_MultConstNf_SCALE FF_FULL_wdcd_MultConstNf_SCALE FF_FULL_wudef_MultConstNf_SCALE
for FFSCALEDIR in FF_FULL_MultConstNf_SCALE
do
for SCALE in ${Q2LIST}
do

  SCALEFILE=`ls ${FFSCALEBASE}/${FFSCALEDIR} | grep "Multi_" | grep "FFr${SCALE}_"`
  if [ ! -z "${SCALEFILE}" ]; then
      echo ${FFSCALEBASE}/${FFSCALEDIR}/${SCALEFILE} >> scalelist.dat
      if [ ${SCALE} = ${Q2BASE1} ]; then
	  SFILE1=${FFSCALEBASE}/${FFSCALEDIR}/${SCALEFILE}
      fi
      if [ ${SCALE} = ${Q2BASE2} ]; then
	  SFILE2=${FFSCALEBASE}/${FFSCALEDIR}/${SCALEFILE}
      fi
  fi

done
done

#for Q2 in 1 4 10 30 100
#for Q2 in 10
for Q2 in 10 100
  do

  for Flavor in -2 0 1 2 3 4 5
#  for Flavor in 0 1 2 4
  do

    for SUFFIX in png eps
#    for SUFFIX in png
    do

    time residual_dist --DSSFF=DSS05_FF.xml --KretzerFF=KretzerNLO.xml \
	--FF=${FFBASE}/FF_FULL_MultConstNf/100319223731_Multi_FF_NLO_0.05-0.8_werr.xml \
	--FFSCALE1=${SFILE1} --FFSCALE2=${SFILE2} \
	--q2=${Q2} --flavor=${Flavor} --logx \
	--setLC3=4 --setLW3=2 --setLS3=2 \
	--setLC4=1 --setLW4=2 --setLS4=1 \
	--setLC6=2 --setLW6=3 --setLS6=1 \
	--setFC5=1 --setFS5=3013 \
	--setLC5=1 --setLW5=2 --flegend5 --fillplot5 \
	--setFC7=3 --setFS7=3001 \
	--setLC7=3 --setLW7=2 --flegend7 --fillplot7 \
	--setFC8=2 --setFS8=3001 \
	--setLC8=2 --setLW8=2 --flegend8 --fillplot8 \
	--tgorder=7,8,6,5,3,4 \
	--noplot1 --noplot2 \
	--legend3="Kretzer00" --legend4="DSS07" \
	--legend6="Present Analysis" \
	--legend5="Scale(1/2<#mu^{2}/Q^{2}<2)" \
	--legend7="Tolerance(90%CL)" --legend8="Stat. Err." \
	--lorder=6,8,7,5,3,4 \
	--nolegend1 --nolegend2 --nolegend6 \
	--legparam=0.12,0.56,0.42,0.88 --lgtxtsize=0.040 \
	--tolerance=2.89 \
	--xytitle="z,#Delta(zD^{\pi+}) / #sigma_{TOL}" \
	--xdiv=50 --fill --scale --scalekey=FFr --scalelist="scalelist.dat" \
	--format=${SUFFIX}
# --nolegend
# --setFS5=3013 \
# --setFS5=3002 \
# --setFS5=3003 \
# --nodisplay 
# --nolegend5 \
# --tolerance=3.19
    done

  done

done
