#!/bin/bash
#XMIN=0.005
XMIN=0.01

for ORD in NLO
#for ORD in LO NLO
do

#for Q2 in 1.0 10.0 30.0
#for Q2 in 1.0
#for Q2 in 30.0
for Q2 in 10.0 30.0
do

#for iflav in -5 -4 -3 -2 -1 0 1 2 3 4 5
for iflav in -3 -2 -1 0 1 2 3 4 5
#for iflav in -3
#for iflav in 4 5
#for iflav in -2 -1 0 1 2 3
do

cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
 --FF --HAD=2 --xmin=${XMIN} --fill --shade1=3001 --shade3=3006 \
 --woCenter2 --flegend2 \
 --addflegend1="Stat. Err. (1#sigma)" \
 --suffix=png 
../browser/FFK_FULL/distout${ORD}FFK_werr.dat FFK_FULL
../browser/FFK_PART_effx/distout${ORD}FFK_werr.dat FFK_effx
../browser/distout${ORD}KretzerFFK_kret.dat Kretzer00
../browser/distout${ORD}DSSFFK.dat DSS07
EOF

# --nolegend
# --woCenter3 --flegend3 \
# --woCenter4 --flegend4 \
#../browser/FFK_PART_efx/distout${ORD}FFK_werr.dat FFK_efx
#../browser/FFK_PART_dgfx_effx/distout${ORD}FFK_werr.dat FFK_dgfx

# --flegend2
#../browser/distout${ORD}FFK_werr.dat Present_Analysis
#../browser/distout${ORD}FFK_werr_Dchi2_7.13.dat LM(90%CL)

#../browser/distout${ORD}FFK_werr_PART_gfx.dat Present_Analysis
#../browser/distout${ORD}FFK_werr_PART_gfx_Dchi2_7.13.dat LM(90%CL)

#--ymax=0.3 iflav others
#--ymax=0.5 iflav 4 5
#--ymax=1.0 iflav -3
#--ymax=1.2
#../browser/distout${ORD}FFK_best_KA.dat Best_KA
#../browser/distout${ORD}FFK.dat Best_KA_2

#--wyellow \
# --shade1=3001 --shade2=3013 --shade3=3006

done
done
done

