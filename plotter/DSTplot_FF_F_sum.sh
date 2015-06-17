#!/bin/bash
#XMIN=0.005
XMIN=0.01

#BASEDIR=""
#BASEDIR=FF_FULL_MultConstNf_Final
BASEDIR=FF_FULL_MultConstNf_Final_W_Ns
#BASEDIR=FF_PART_cdfx_cfree_MultConstNf_Final

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

#cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
# --Kretzer --HAD=1 --xmin=${XMIN} --woerror --ymax=2.0 \
# --skips --skips-bar --skipd-bar --skipu-bar
#../browser/distout${ORD}KretzerFF_werr.dat My_Fit
#EOF

for SUFF in png eps
do

cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
 --FF --HAD=1 --xmin=${XMIN} --woerror --ymax=2.0 \
 --setLW1=3 \
 --skips-bar --skipd-bar --skipu-bar --suffix=${SUFF}
../browser/${BASEDIR}/distout${ORD}FF_werr.dat Present_Analysis
EOF
# --skips --skips-bar --skipd-bar --skipu-bar --suffix=${SUFF}
# --nodisplay
# --gstyle=Pub

done

#--ymax=2.0 \
#--ymax=1.2
#--suffix=eps
#--gluon=0.1

#../browser/distout${ORD}FF_werr.dat My_Fit
#../browser/distout${ORD}FF_werr_Dchi2_4.97.dat My_LM(90%CL)
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
#../browser/distout${ORD}DSSFF.dat DSS07

done
done
done

