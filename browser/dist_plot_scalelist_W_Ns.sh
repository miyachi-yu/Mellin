#!/bin/bash
if [ -z "$1" ]; then
cd ${PBS_O_WORKDIR}
fi
rm -rf ./dist_plot_scale.*

source ~/mellin/bin/setup.sh

[ ! -e "./dist_direct_scalelist_W_Ns" ] && mkdir -p ./dist_direct_scalelist_W_Ns
cd dist_direct_scalelist_W_Ns
/bin/rm -rf ./scalelist.dat

BASEDIR=/home/imazu/work/QCD/tmpFit
FFBASE=${BASEDIR}/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final
FFSCALEBASE=${BASEDIR}/Scale_PI_FF_Final_SCALE_Study_Final

#Q2LIST="0.41 0.5 0.71 0.84 1 1.19 1.41 1.68"
#Q2BASE1="0.41"
#Q2BASE2="1.68"
#SCALE="0.125 0.15 0.18 0.21 0.25 0.3 0.35 0.41 0.5 0.59 0.71 0.84 \
#       1 1.19 1.41 1.68 2 2.38 2.83 3.37 4 4.16 5.66 6.74 8"
Q2LIST="0.3 0.5 1 2"
Q2BASE1="0.3"
Q2BASE2="2"

for FFSCALEDIR in FF_FULL_MultConstNf_W_Ns_SCALE
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
for Q2 in 10
  do

  for Flavor in -2 0 1 2 3 4 5
#  for Flavor in 2
  do

    for SUFFIX in png eps
#    for SUFFIX in png
    do
    
    time dist_plot --DSSFF=DSS05_FF.xml --KretzerFF=KretzerNLO.xml \
	--FF=${FFBASE}/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml \
	--FFSCALE1=${SFILE1} --FFSCALE2=${SFILE2} \
	--q2=${Q2} --flavor=${Flavor} --logx --ymax=1.0 \
	--setLC1=2 --setLW1=3 --setLS1=1 \
	--setLC4=4 --setLW4=2 --setLS4=2 \
	--setLC5=1 --setLW5=2 --setLS5=1 \
	--setLC6=2 --setLW6=2 --setFC6=2 --setFS6=3001 \
	--setLC7=3 --setLW7=2 --setFC7=3 --setFS7=3001 \
	--setLC8=1 --setLW8=2 --setFC8=1 --setFS8=3013 \
	--flegend6 --fillplot6 \
	--flegend7 --fillplot7 \
	--flegend8 --fillplot8 \
	--noplot2 --noplot3 --nolegend2 --nolegend3 --zerohorizon \
	--legend1="Present Analysis" --legend4="Kretzer00" --legend5="DSS07" \
	--legend6="Stat. Err." --legend7="Tolerance(90%CL)" \
        --legend8="Scale(1/2<#mu^{2}/Q^{2}<2)" \
	--tgorder=7,6,8,4,5,1 --lorder=1,6,7,8,4,5 \
	--legparam=0.12,0.12,0.42,0.38 --lgtxtsize=0.040 \
	--tolerance=2.91 --xdiv=50 --fill --scale --scalekey=FFr \
	--scalelist="scalelist.dat" --noheader --format=${SUFFIX}

# --noplot4 --noplot5 --nolegend4 --nolegend5 \

# left-bottom
# --legparam=0.12,0.12,0.42,0.38 --lgtxtsize=0.040 \
# right-top
# --legparam=0.58,0.62,0.88,0.88 --lgtxtsize=0.040 \

# --nolegend
# --setFS8=3013 \
# --setFS8=3002 \
# --setFS8=3003 \
# --nodisplay 
# --nolegend 
# --tolerance=3.19
    done

  done

done
