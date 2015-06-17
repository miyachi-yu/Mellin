#!/bin/bash

EXEC=/home/imazu/work/QCD/Mellin/plotter/scale_plotDST

###### base dat ######
for SCALE in ${SCALE_V}
  do
  FNAME=`ls | grep FFr${SCALE}_`
  if [ ! -z "${FNAME}" ]; then
      echo "${FNAME} ${SCALE}" >> tmp.dat

      RCHI2=`grep "reduced chi2" ${FNAME} | sed -e 's/reduced chi2//g'`
      DOF=`grep "d.o.f" ${FNAME} | sed -e 's/d.o.f//g'`
      echo "${SCALE} ${DOF} ${RCHI2}" >> chi2.dat
  fi
done

###### chi2 plot ######
${EXEC} --rchi2=chi2.dat --werror --fill=3006 \
 --legend2=" Stat. Err. (1#sigma)" --flegend2 \
 --setLC1=2 --setLC2=2 --setLS1=1 --setLS2=1 --setLW1=3 --setLW2=2 \
 --addhline=1.0 --addhl_LC=1 --addhl_LS=2 --addhl_LW=2

###### moment plot ######
for Q2 in ${Q2list_V}
  do

for iflav in ${IFLAV_V}
  do

cat tmp.dat | ${EXEC} --rN=${MOM_V} --rQ2=${Q2} --rflavor=${iflav} \
 --SORT=${FF_V} --HAD=1 --werror --partial=0.05,0.8 --fill=3006 \
 --legend2=" Stat. Err. (1#sigma)" --flegend2 \
 --setLC1=2 --setLC2=2 --setLS1=1 --setLS2=1 --setLW1=3 --setLW2=2

#--partial=0.05,0.8
#--linear

done

done

###### moment table ######
Q2list_V=`echo ${Q2list_V} | sed -e 's/\ /\,/g'`
IFLAV_V=`echo ${IFLAV_V} | sed -e 's/\ /\,/g'`

cat tmp.dat | ${EXEC} --sumtable --rN=${MOM_V} --SORT=${FF_V} --HAD=1 \
--werror --partial=0.05,0.8 --q2list=${Q2list_V} --flavlist=${IFLAV_V}

/bin/rm -rf tmp.dat
/bin/rm -rf chi2.dat
