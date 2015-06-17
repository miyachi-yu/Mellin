#!/bin/bash

SC_DIR=`pwd`/plot_scripts
SCRIPT=DSTplot_FF_F.sh

if [ ! -z "$1" ]; then

    cd $1
    export DIR_V="./distout_dat"
    export Q2list_V="1.0 10.0 30.0"

    ${SC_DIR}/${SCRIPT}

fi