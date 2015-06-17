#!/bin/bash

SC_DIR=`pwd`/plot_scripts
SCRIPT=Chi2plot_FF_F.sh

Q2list="1.0 10.0"
#Q2list="1.0"

#SCALE="0.25 0.5 1 2 4"
#SCALE="0.125 0.15 0.18 0.21 0.25 0.30 0.35 0.41 0.5 0.59 0.71 0.84 \
#       1 1.19 1.41 1.68 2 2.38 2.83 3.37 4 4.16 5.66 6.74 8"

#SCALE="0.18 0.35 0.5 0.71 1 1.19 2.38 3.37 4 4.16 5.66"
#SCALE="0.18 0.35 0.5 0.71 1 1.19 1.68 2 2.83 3.37 4 4.16 5.66"
SCALE="0.5 0.71 0.84 1 1.19 1.41 1.68 2"

#IFLAV="0 1 2 4 5"
IFLAV="0 1 2 4"
#IFLAV="0"

FF="FF"
#FF="KretzerFF"
#FF="DSSFF"

for MOM in 1 2
do

if [ ! -z "$1" ]; then

    cd $1
    export Q2list_V=${Q2list}
    export SCALE_V=${SCALE}
    export IFLAV_V=${IFLAV}
    export MOM_V=${MOM}
    export FF_V=${FF}
    
    ${SC_DIR}/${SCRIPT}

fi

done
