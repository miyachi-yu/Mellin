#!/bin/sh

# File Name
export NAME=NAME
export YEAR=1999
export DATA=$NAME\_$YEAR.dat # read a file under Mellin/Data/

# Parametrs
export X=s/\/\/xmean//g
export XMAX=s/\/\/xmax//g
export XMIN=s/\/\/xmin//g
export Q2=s/\/\/q2mean//g
export Q2MAX=s/\/\/q2max//g
export Q2MIN=s/\/\/q2min//g
export E=s/\/\/E//g
export SYS=s/\/\/sys//g
export MC=s/\/\/mc//g

# write informations about data
export INFO="HERMESS,1999,A1"
export REF="Phys.Lett.B464(1999)123"      

sed -e s/XYZ/$NAME/g -e s/ABC.dat/$DATA/g -e s/INFO/$INFO/ -e s/REF/$REF/ XYZ.hh |tee $NAME.hh 


#sed -e $X -e $Q2 -e $SYS $NAME.hh |tee $NAME.hh

#sed s/XYZ/$NAME/g table_XYZ.cc|tee table_$NAME.cc 


#sed s/XYZ/$NAME/g Makefile.am|tee Makefile.am





#./table_$NAME


#rm table_$NAME

