#!/bin/bash

#for FNAME in "`ls $1 | grep FFr`"
for FNAME in `ls $1 | grep FFr`
do
  echo ${FNAME}
done