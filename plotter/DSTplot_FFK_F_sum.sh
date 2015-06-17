#!/bin/bash
#XMIN=0.005
XMIN=0.01

for ORD in NLO
#for ORD in LO NLO
do

#for Q2 in 1.0 10.0 30.0
#for Q2 in 10.0
for Q2 in 10.0 30.0
do

#for iflav in -5 -4 -3 -2 -1 0 1 2 3 4 5
for iflav in 0
do

cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
 --FF --HAD=2 --xmin=${XMIN} --woerror --ymax=0.5 \
 --skips --skipd-bar --skipu-bar
../browser/FFK_FULL/distout${ORD}FFK_werr.dat FFK_FULL
../browser/FFK_PART_efx/distout${ORD}FFK_werr.dat FFK_efx
../browser/FFK_PART_effx/distout${ORD}FFK_werr.dat FFK_effx
../browser/FFK_PART_dgfx_effx/distout${ORD}FFK_werr.dat FFK_dgfx
EOF

#../browser/distout${ORD}FFK_werr.dat My_Fit
#../browser/distout${ORD}FFK_werr_PART_gfx.dat My_Fit

#../browser/distout${ORD}FFK_werr.dat My_Fit
#../browser/distout${ORD}FFK_werr_PART_gfx.dat My_Fit

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

