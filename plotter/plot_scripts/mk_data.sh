#!/bin/bash

CUR_DIR=`pwd`
SC_DIR=${CUR_DIR}/plot_scripts
SCRIPT=qsub_FF_F.sh
#QUEUE=S
QUEUE=L

#Q2list="0.25 0.5 1 2 4"
Q2list="1.19 1.41 1.68 2.38 2.83 3.37 4.16 5.66 6.74 8 \
        0.84 0.71 0.59 0.41 0.35 0.30 0.21 0.18 0.15 0.125"

if [ ! -z "$1" ]; then

    [ ! -e "$1/distout_dat/" ] && mkdir -p $1/distout_dat/
    cd $1/distout_dat/
    
    export DIR_V="../"
    if [ -z "$2" ]; then
	export KEY_V="FFr"
    else
	export KEY_V=$2
    fi

    if [ ! -z "$3" ]; then
	export ErrOpt_V=$3
    fi
    export Q2list_V=${Q2list}

    qsub -V -q ${QUEUE} ${SC_DIR}/${SCRIPT}

fi