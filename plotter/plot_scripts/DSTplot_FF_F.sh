#!/bin/bash
#XMIN=0.005
XMIN=0.01
#XMIN=0.05
EXEC=/home/imazu/work/QCD/Mellin/plotter/plotDST

for ORD in NLO
do

##### Reference Plots #####
REFDIS=/home/imazu/work/QCD/Mellin/browser/distout${ORD}FF_werr.dat

for Q2 in ${Q2list_V}
do

for iflav in -3 -2 -1 0 1 2 3 4 5
do

cat <<EOF | ${EXEC} --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
 --FF --HAD=1 --xmin=${XMIN} --fill --shade1=3001 \
 --setLC2=4 --setLC3=1 --setLC4=1 --setLC5=1 --setLC6=1 \
 --setLS2=1 --setLS3=3 --setLS4=2 --setLS5=2 --setLS6=3 \
 --setLW2=2 --setLW3=2 --setLW4=2 --setLW5=2 --setLW6=2 
${REFDIS} Present_Analysis
${DIR_V}/distout${ORD}FF_r1.dat ReSc_1.0
${DIR_V}/distout${ORD}FF_r0.25.dat ReSc_0.25
${DIR_V}/distout${ORD}FF_r0.5.dat ReSc_0.5
${DIR_V}/distout${ORD}FF_r2.dat ReSc_2.0
${DIR_V}/distout${ORD}FF_r4.dat ReSc_4.0
EOF

#--linear\

done
done
done

