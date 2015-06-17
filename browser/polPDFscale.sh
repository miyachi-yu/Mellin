#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./polPDFscale.sh.*

source ~/mellin/bin/setup.sh

[ ! -e "./dist_momnet" ] && mkdir -p ./dist_moment
cd dist_moment

FFBASE=/home/imazu/work/QCD/tmpFit/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_wo_dcd_Final
#FFBASE2=/home/imazu/work/QCD/tmpFit/Scale_PI_FF_Final/FF_FULL_W_NORM_tmp/FF_w_ef

xmax=0.8
for xmin in 0.2 0.05
do

for MOM in 1 2
do

time polPDFscale --moment=${MOM} --partial=${xmin},${xmax} --imax=5 \
 --DSSFF=DSS05_FF.xml

time polPDFscale --moment=${MOM} --partial=${xmin},${xmax} --imax=5 \
 --KretzerFF=KretzerNLO.xml

time polPDFscale --moment=${MOM} --partial=${xmin},${xmax} --imax=5 --werror \
 --FF=${FFBASE}/FF_FULL_W_NORM_MultConstNf_W_Ns/100802070516_Multi_FF_NLO_0.05-0.8_werr.xml
# --FF=${FFBASE}/FF_FULL_MultConstNf_W_Ns/100628014844_Multi_FF_NLO_0.05-0.8_werr.xml 

# --FF=${FFBASE}/FF_FULL_MultConstNf/100319223731_Multi_FF_NLO_0.05-0.8_werr.xml 
# --FF=${FFBASE}/FF_PART_cdfx_cfree_MultConstNf/100319062740_Multi_FF_NLO_0.05-0.8_werr.xml
#

# --FF=${FFBASE2}/FF_FULL_wuef/100321101518_Multi_FF_NLO_0.05-0.8_werr.xml
# --FF=${FFBASE}/FF_PART_cdfx_dfree/100319040826_Multi_FF_NLO_0.05-0.8_werr.xml 
# --FF=${FFBASE}/FF_PART_cdfx_cfree/100319040825_Multi_FF_NLO_0.05-0.8_werr.xml
# --FF=${FFBASE}/FF_FULL/091217104813_Multi_FF_NLO_0.05-0.8_werr.xml
# --FF=${FFBASE}/FF_PART_cdfx/091217104955_Multi_FF_NLO_0.05-0.8_werr.xml 

done

done

cd ../