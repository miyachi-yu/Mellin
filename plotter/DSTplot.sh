#!/bin/bash
XMIN=0.005

for ORD in NLO
#for ORD in LO NLO
do

#for Q2 in 1.0 2.0 4.0 10.0 20.0 100.0 200.0 1000.0
#for Q2 in 1.0 2.0 10.0 100.0 
#for Q2 in 1.0 
#for Q2 in 4.0 
for Q2 in 10.0
#for Q2 in 100.0
#for Q2 in 10.0
do

#for iflav in -5 -4 -3 -2 -1 0 1 2 3 4 5
#for iflav in -4 -3 -2 -1 0 1 2 3 4
#for iflav in 0
#for iflav in -3 -2 -1 0 1 2 3 4
#for iflav in -3 -2 -1 0 1 2 3
#for iflav in -2 -1 0 1 2 3
#for iflav in -3 -2 -1 3

#for iflav in 1
for iflav in -2
do

#cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
#--DSSV --xmin=${XMIN} --gluon=0.8 --u=0.4 --d=0.8 --suffix=eps
#../browser/distout${ORD}DSSV_org.dat DSSV_evo
#../browser/distout${ORD}DSSV_grid.dat DSSV_grid
#EOF

#../browser/distout${ORD}DSSV_imazu.dat DSSV_imazu

#cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
#--MRST --xmin=${XMIN} --gluon=0.1 --skips-bar
#../browser/distout${ORD}MRST.dat MRST_evo
#../browser/distout${ORD}MRST_grid.dat MRST_grid
#EOF

#--suffix=eps

#cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
#--Kretzer --HAD=1 --xmin=${XMIN} --suffix=eps
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer_evo
#../browser/distout${ORD}KretzerFF_grid.dat Kretzer_grid
#EOF

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
#--FFK --xmin=${XMIN}
#../browser/distout${ORD}FFK_werr_${Q2}.dat My_Calculation
#../browser/distout${ORD}KretzerFFK_kret.dat Kretzer00
#../browser/distout${ORD}DSSFFK.dat DSS07
#EOF

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
#--FFP --xmin=${XMIN}
#../browser/distout${ORD}FFP.dat My_Calculation
#../browser/distout${ORD}KretzerFFP_kret.dat Kretzer00
#../browser/distout${ORD}DSSFFP.dat DSS07
#EOF

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --DSSV --xmin=${XMIN} --fill --color1=2 --color2=1 \
# --shade1=3002 --shade2=3001 
#../browser/distout${ORD}DSSV_werr_kret.dat My_Calculation
#../browser/distoutNLOGRSV00.dat GRSV00
#../browser/distout${ORD}DSSV.dat DSSV08
#EOF

# --ymax=0.3
# --ymax=0.05 --ymin=-0.05
# --scale
#../browser/distout${ORD}DSSV_r0.5_kret.dat rescale_0.5
#../browser/distout${ORD}DSSV_r2.0_kret.dat rescale_2.0
#../browser/distout${ORD}DSSV_kret.dat My_Calculation

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --DSSV --xmin=${XMIN} --fill --color1=2 --color2=1 \
# --shade1=3002 --shade2=3001 --scale
#../browser/distout${ORD}DSSV_werr_kret.dat My_Calculation
#../browser/distout${ORD}DSSV_r0.5_kret.dat rescale_0.5
#../browser/distout${ORD}DSSV_r2.0_kret.dat rescale_2.0
#../browser/distoutNLOGRSV00.dat GRSV00
#../browser/distout${ORD}DSSV.dat DSSV08
#EOF

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --DSSV --xmin=${XMIN} --fill --color1=2 --color2=1 \
# --shade1=3002 --shade2=3001 
#../browser/distout${ORD}DSSV_werr_kret.dat My_Calculation
#../browser/distout${ORD}DSSV_r0.5_kret.dat rescale_0.5
#../browser/distout${ORD}DSSV_r2.0_kret.dat rescale_2.0
#EOF

# --ymax=0.3
# --ymax=0.05 --ymin=-0.05
# --scale
#../browser/distout${ORD}DSSV_r0.5_kret.dat rescale_0.5
#../browser/distout${ORD}DSSV_r2.0_kret.dat rescale_2.0
#../browser/distout${ORD}DSSV_kret.dat My_Calculation

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --DSSV --xmin=${XMIN} --fill --color1=2 --color2=1 \
# --shade1=3002 --shade2=3001 \
# --nolegend --noxtitle --noytitle
#../browser/distout${ORD}DSSV_werr_kret.dat My_Analysis
#../browser/distoutNLOGRSV00.dat GRSV00
#../browser/distout${ORD}DSSV.dat DSSV08
#EOF

# --shade1=3002 --shade2=3001 \
# --shade1=3002 --shade2=3001 --ymax=0.05 --ymin=-0.05 \

cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
 --DSSV --xmin=${XMIN} --fill --color1=2 --color2=1 \
 --setLC2=1 --setLC4=4 --setLC5=1 --shade1=3001 --shade2=3002 --scale \
 --addflegend1="Stat. Err. (1#sigma)" \
 --addflegend2="Syst. Err. (FF)" \
 --noautolegend2 --scaleLG1 \
 --nolegend --noxtitle --noytitle --setxTick=0.07 --setyTick=0.05 \
 --xlabelsize=0.05 --ylabelsize=0.045 \
 --zeroline --frameWidth=1 \
 --ymax=0.035 --ymin=-0.045 
../browser/distout${ORD}DSSV_werr_kret.dat Present_Analysis
../browser/distout${ORD}DSSV_FF_wg_scale1.dat ''
../browser/distout${ORD}DSSV_FF_wg_scale2.dat ''
EOF

#../browser/distoutNLOGRSV00.dat GRSV00
#../browser/distout${ORD}DSSV.dat DSSV08

# d-bar
# --ymax=0.035 --ymin=-0.045 
# u
# --ymax=0.38 --ymin=-0.04 

# w/ legend
# --noxtitle --noytitle --setxTick=0.07 --setyTick=0.05 \
# w/o legend
# --nolegend --noxtitle --noytitle --setxTick=0.07 --setyTick=0.05 \

#--flegend2 \
#Syst._Err._(FF)
#--setLC3=1 --setLC2=4 \
#--ymax=0.05 --ymin=-0.05

#--nolegend --noxtitle --noytitle
#--shade2=3001 \
#--scale \
#../browser/distout${ORD}DSSV_FF_wg_scale1.dat ''
#../browser/distout${ORD}DSSV_FF_wg_scale2.dat ''

#../browser/distout${ORD}DSSV_FF_scale1.dat ''
#../browser/distout${ORD}DSSV_FF_scale2.dat ''

# --shade1=3002 --shade2=3001 --ymax=0.05 --ymin=-0.05 \
# --shade1=3002 --shade2=3001 \

#../browser/distout${ORD}DSSV_FF.dat My_Calc_FF
#../browser/distout${ORD}DSSV_DSSFF_hfree.dat My_Calc_DSSFF
#../browser/distout${ORD}DSSV_KREFF_hfree.dat My_Calc_KREFF
#../browser/distout${ORD}DSSV_FF_hfree.dat My_Calc_FF

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --KretzerFF --xmin=${XMIN} --ymax=1.2 --fill --color1=2 --color2=1 \
# --shade1=3002 --shade2=3001
#../browser/distout${ORD}FF_werr.dat My_Calculation
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
#../browser/distout${ORD}DSSFF.dat DSS07
#EOF

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --KretzerFF --xmin=${XMIN} --ymax=1.2 --fill --color1=2 --color2=1 \
# --shade1=3001 --shade2=3002 --scale
#../browser/distout${ORD}FF_werr.dat My_Calculation
#../browser/distout${ORD}FF_r0.5.dat rescale_0.5
#../browser/distout${ORD}FF_r1.5.dat rescale_2.0
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
#../browser/distout${ORD}DSSFF.dat DSS07
#EOF

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --KretzerFF --xmin=${XMIN} --fill --color1=2 --color2=1 \
# --shade1=3002 --shade2=3001
#../browser/distout${ORD}FFK_werr.dat My_Calculation
#../browser/distout${ORD}KretzerFFK_kret.dat Kretzer00
#../browser/distout${ORD}DSSFFK.dat DSS07
#EOF

#../browser/distout${ORD}FFK_r0.5.dat rescale_0.5
#../browser/distout${ORD}FFK_r1.5.dat rescale_2.0

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --KretzerFF --xmin=${XMIN} --fill --color1=2 --color2=1 \
# --shade1=3002 --shade2=3001
#../browser/distout${ORD}FFP_werr.dat My_Calculation
#../browser/distout${ORD}KretzerFFP_kret.dat Kretzer00
#../browser/distout${ORD}DSSFFP.dat DSS07
#EOF

#../browser/distout${ORD}FFP_r0.5.dat rescale_0.5
#../browser/distout${ORD}FFP_r1.5.dat rescale_2.0

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --KretzerFF --xmin=${XMIN} --fill --color1=2 --color2=1 --ymax=1.2 \
# --shade1=3002 --shade2=3001
#../browser/distout${ORD}hadronFF_werr.dat My_Calculation
#../browser/distout${ORD}KretzerhadronFF_kret.dat Kretzer00
#../browser/distout${ORD}DSShadronFF.dat DSS07
#EOF

#--ymax=2.0 \
#../browser/distout${ORD}hadronFF.dat My_Calculation

#../browser/distout${ORD}FF_werr.dat My_Calculation
#../browser/distout${ORD}FF_r0.5.dat rescale_0.5
#../browser/distout${ORD}FF_r1.5.dat rescale_1.5
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer00
#../browser/distout${ORD}DSSFF.dat DSS07

#../browser/distout${ORD}FF_r2.0.dat rescale_2.0

#../browser/distout${ORD}KretzerFF_werr.dat My_Calculation
#../browser/distout${ORD}KretzerFF_r0.5.dat rescale_0.5
#../browser/distout${ORD}KretzerFF_r2.0.dat rescale_2.0

#../browser/distout${ORD}FF_werr.dat My_Calculation
#../browser/distout${ORD}FF_r0.5.dat rescale_0.5
#../browser/distout${ORD}FF_r2.0.dat rescale_2.0

#cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
#--Kretzer --HAD=1 --xmin=${XMIN}
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer_evo
#../browser/distout${ORD}KretzerFF_grid.dat Kretzer_grid
#EOF

#cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
#--DSSV --xmin=${XMIN} --gluon=0.8 --u=0.4 --d=0.8
#../browser/distout${ORD}DSSV.dat DSSV_evo
#../browser/distout${ORD}DSSV_grid.dat DSSV_grid
#EOF

#cat <<EOF | ./plotDST --sumplot --Q2=${Q2} --order=${ORD} \
#--MRST --xmin=${XMIN} --gluon=0.1
#../browser/distout${ORD}MRST.dat MRST_evo
#../browser/distout${ORD}MRST_grid.dat MRST_grid
#EOF

#cat <<EOF | ./plotDST --flavor=${iflav} --Q2=${Q2} --order=${ORD} \
# --FF
#../browser/distout${ORD}KretzerFFK_grid.dat Kretzer_grid
#../browser/distout${ORD}KretzerFFK_kret.dat Kretzer_evo
#../browser/distout${ORD}KretzerFFK.dat Kretzer_fit
#EOF

done
done
done

# MRST
#--MRST
#../browser/distout${ORD}MRST.dat MRST_evo
#../browser/distout${ORD}MRST_grid.dat MRST_grid

# DSSV
#--
#../browser/distout${ORD}DSSV.dat DSSV_evo
#../browser/distout${ORD}DSSV_grid.dat DSSV_grid

# Kretzer
# --FF
#../browser/distout${ORD}KretzerFF_werr.dat Kretzer_fit
#../browser/distout${ORD}KretzerFF_r0.25.dat Kretzer_fit_0.25
#../browser/distout${ORD}KretzerFF_r4.dat Kretzer_fit_4.0
#../browser/distout${ORD}KretzerFF_kret.dat Kretzer_evo
#../browser/distout${ORD}KretzerFF_grid.dat Kretzer_grid
#../browser/distout${ORD}KretzerFF.dat Kretzer_fit

#DSSFF
#../browser/distout${ORD}DSSFF.dat DSS_evo

# Kobayashi
# --FF
#/home/ykobayashi/QCD/user_program/s_free.dat S_Free
#/home/ykobayashi/QCD/user_program/Kretzer.dat Kretzer

#Our Fit
#../browser/distout${ORD}FF.dat Our_Fit
