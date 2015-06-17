#!/bin/bash

for SCALE in 0.25 0.5 1 2 4
do

  FNAME=`ls | grep FFr${SCALE}`
  if [ ! -z ${FNAME} ]; then
#      echo ${FNAME}
      parprint --pdf=${FNAME}
  fi

done