#!/bin/bash
for ORD in NLO
#for ORD in LO NLO
do

for Q2 in 1.0 2.0 4.0 20.0 200.0
#for Q2 in 1.0
do

cat <<EOF | ./plotXS  --Q2=${Q2} --order=${ORD} \
 --FF --logy --ymin=1.0E-2 --ymax=1.0E+2
../browser/xsout${ORD}KretzerFF_werr.dat Kretzer_fit
../browser/xsout${ORD}KretzerFF_r0.25.dat Kretzer_fit_0.25
../browser/xsout${ORD}KretzerFF_r4.dat Kretzer_fit_4.0
../browser/xsout${ORD}KretzerFF_kret.dat Kretzer_evo
EOF


done
done

# MRST F2 / 2
#cat <<EOF | ./plotXS  --sum  --F2 --order=${ORD} \
#--MRST
#../browser/xsout${ORD}MRST_F2.dat MRST_evo

# MRST F1
#cat <<EOF | ./plotXS   --Q2=${Q2} --order=${ORD} \
#--MRST
#../browser/xsout${ORD}MRST_F2.dat MRST_evo

# Kretzer
#cat <<EOF | ./plotXS  --Q2=${Q2} --order=${ORD} \
# --FF
#../browser/xsout${ORD}KretzerFF_werr.dat Kretzer_fit
#../browser/xsout${ORD}KretzerFF_r0.25.dat Kretzer_fit_0.25
#../browser/xsout${ORD}KretzerFF_r4.dat Kretzer_fit_4.0
#../browser/xsout${ORD}KretzerFF_kret.dat Kretzer_evo
#../browser/xsout${ORD}KretzerFF.dat Kretzer_fit
