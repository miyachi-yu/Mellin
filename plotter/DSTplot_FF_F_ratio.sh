#!/bin/bash
#XMIN=0.005
XMIN=0.01

#BASEDIR=""
#BASEDIR=FF_PART_cdfx_cfree_MultConstNf_Final
BASEDIR=FF_FULL_MultConstNf_Final
BASEDIR=tmp_FF_FULL_MultConstNf_Final

for ORD in NLO
#for ORD in LO NLO
do

for Q2 in 1.0 4.0 10.0 30.0 100.0
#for Q2 in 10.0
#for Q2 in 30.0
do

#for iflav in -5 -4 -3 -2 -1 0 1 2 3 4 5
for iflav in 0
do

#for SUFF in png eps
for SUFF in png
do

cat <<EOF | ./plotDST --ratio=-3,2 --ratiobase=-1 --Q2=${Q2} --order=${ORD} \
 --FF --HAD=1 --xmin=${XMIN} --woerror --setLW1=3  --suffix=${SUFF} \
 --ymax=1.001 --ymin=0.999 --leftmarg=0.17
../browser/${BASEDIR}/distout${ORD}FF.dat Present_Analysis
EOF
#../browser/distout${ORD}KretzerFF_grid.dat Present_Analysis
#../browser/${BASEDIR}/distout${ORD}FF_werr.dat Present_Analysis
# --ymax=1.001 --ymin=0.999 --leftmarg=0.17
#--ymax=2.0
# --nodisplay
# --gstyle=Pub

done

done
done
done

