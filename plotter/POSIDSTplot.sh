#!/bin/bash
XMIN=0.4
#XMIN=0.1
#XMIN=0.001

for ORD in NLO
do
for Q2 in 1.0
do
for iflav in 0
do

cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
--polPDF --xmin=${XMIN} 
../browser/positiv${ORD}DSSV.dat DSSV_evo 
EOF

#../browser/positiv${ORD}polPDF.dat polPDF_evo 

done
done
done
