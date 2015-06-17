#!/bin/bash
#XMIN=0.005
XMIN=0.01

#BASEDIR=""
BASEDIR=FF_FULL_MultConstNf_Final
#BASEDIR=FF_PART_cdfx_cfree_MultConstNf_Final
#BASEDIR=FF_FULL_MultConstNf_Final_W_FF_PART_cdfx_cfree_MultConstNf_SCALE

for ORD in NLO
#for ORD in LO NLO
do

for Q2 in 1.0 4.0 10.0 30.0 100.0
#for Q2 in 10.0
#for Q2 in 30.0
do

#for iflav in -5 -4 -3 -2 -1 0 1 2 3 4 5
#for iflav in -3 -2 -1 0 1 2 3 4 5
for iflav in -2 0 1 2 3 4 5
do

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --Kretzer --HAD=1 --xmin=${XMIN} --fill --color2=1  --setLC2=1 --setLC4=3 \
# --setLC5=4 --setLC6=1 --shade1=3001 --shade2=3013 --shade3=3006 --scale \
# --woCenter4 --flegend4 --addflegend1="Stat. Err. (1#sigma)" \
# --scaleLG1 --flegend2 --suffix=png 
#../browser/distout${ORD}KretzerFF_werr.dat Kretzer_Analysis
#../browser/distout${ORD}KretzerFF_r0.5.dat Rescale(1/2<#mu^{2}<2)
#../browser/distout${ORD}KretzerFF_r2.0.dat ReSc_4.0
#../browser/distout${ORD}KretzerFF_werr_Dchi2_6.76.dat LM(90%CL)
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
#../browser/distout${ORD}DSSFF.dat DSS07
#EOF

#--linear
#--scaleabs \
#--shade2=3002

#--ymax=1.2
#../browser/distout${ORD}KretzerFF_r0.25.dat ReSc_0.25
#../browser/distout${ORD}KretzerFF_r4.0.dat ReSc_4.0
#../browser/distout${ORD}KretzerFF_r0.5.dat ReSc_0.5
#../browser/distout${ORD}KretzerFF_r2.0.dat ReSc_2.0

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --FF --HAD=1 --xmin=${XMIN} --fill --color2=1 \
# --shade1=3001 --shade2=3002 --shade3=3006 --scale \
# --setLC2=1 --setLC4=3 --setLC5=4 --setLC6=1 \
# --woCenter4 --flegend4 --addflegend1="Stat. Err. (1#sigma)" \
# --scaleLG1 --flegend2 --suffix=png --nolegend
#../browser/${BASEDIR}/distout${ORD}FF_werr.dat Present_Analysis
#../browser/${BASEDIR}/distout${ORD}FF_r0.5.dat Scale(1/2<#mu^{2}<2)
#../browser/${BASEDIR}/distout${ORD}FF_r2.0.dat ReSc_2.0
#../browser/${BASEDIR}/distout${ORD}FF_werr_Dchi2_10.18.dat Tolerance(90%CL)
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
#../browser/distout${ORD}DSSFF.dat DSS07
#EOF
#
#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --FF --HAD=1 --xmin=${XMIN} --fill --color2=1 \
# --shade1=3001 --shade2=3002 --shade3=3006 --scale \
# --setLC2=1 --setLC4=3 --setLC5=4 --setLC6=1 \
# --woCenter4 --flegend4 --addflegend1="Stat. Err. (1#sigma)" \
# --scaleLG1 --flegend2 --suffix=eps --nolegend
#../browser/${BASEDIR}/distout${ORD}FF_werr.dat Present_Analysis
#../browser/${BASEDIR}/distout${ORD}FF_r0.5.dat Scale(1/2<#mu^{2}<2)
#../browser/${BASEDIR}/distout${ORD}FF_r2.0.dat ReSc_2.0
#../browser/${BASEDIR}/distout${ORD}FF_werr_Dchi2_10.18.dat Tolerance(90%CL)
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
#../browser/distout${ORD}DSSFF.dat DSS07
#EOF

#../browser/distout${ORD}FF_werr_Dchi2_4.97.dat LM(90%CL)

cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
 --FF --HAD=1 --xmin=${XMIN} --fill --shade1=3001 --shade3=3006 \
 --woCenter2 --flegend2 --addflegend1="Stat. Err. (1#sigma)" \
 --suffix=png --nolegend
../browser/${BASEDIR}/distout${ORD}FF_werr.dat Present_Analysis
../browser/${BASEDIR}/distout${ORD}FF_werr_Dchi2_10.18.dat Tolerance(90%CL)
../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
../browser/distout${ORD}DSSFF.dat DSS07
EOF


cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
 --FF --HAD=1 --xmin=${XMIN} --fill --shade1=3001 --shade3=3006 \
 --woCenter2 --flegend2 --addflegend1="Stat. Err. (1#sigma)" \
 --suffix=eps --nolegend
../browser/${BASEDIR}/distout${ORD}FF_werr.dat Present_Analysis
../browser/${BASEDIR}/distout${ORD}FF_werr_Dchi2_10.18.dat Tolerance(90%CL)
../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
../browser/distout${ORD}DSSFF.dat DSS07
EOF

#../browser/distout${ORD}FF_werr_Dchi2_4.97.dat LM(90%CL)

#--suffix=eps
#--ymax=1.4
#--shade3=3006 \
#--ymax=1.0
#--ymax=2.0
#../browser/distout${ORD}FF_werr_Dchi2_9.dat My_Fit_LM_3.0
#../browser/distout${ORD}FF_werr_Dchi2_4.97.dat My_Fit_LM_2.35

#../browser/distout${ORD}FF_r0.25.dat ReSc_0.25
#../browser/distout${ORD}FF_r4.0.dat ReSc_4.0
#../browser/distout${ORD}FF_r0.5.dat ReSc_0.5
#../browser/distout${ORD}FF_r2.0.dat ReSc_2.0

done
done
done

