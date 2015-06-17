#!/bin/bash
rm -rf ./qsub_base.sh.*

OPTBASE="--unpolPDF=../browser/compare_unpolPDF.xml"
OPTBASE="${OPTBASE} --FF=../browser/compare_FF.xml"
#OPTBASE="${OPTBASE} --noCache --length=10.0 --wncalc"
OPTBASE="${OPTBASE} --noCache --length=10.0 --mxloop=2"

#for longshort in Short Long
for longshort in Short
  do
  
  for order in LO NLO
    do

    if [ ${longshort} = Short ]; then
	
	OPT="${OPTBASE} --ShortInteg --ngrid=8"
	FNAME="S${order}"
	
	if [ ${order} = LO ]; then
	    OPT="${OPT} --order=LO"
#	    QPAR="XXL"
	    QPAR="XL"
	else
	    OPT="${OPT} --order=NLO"
#	    QPAR="MC"
	    QPAR="XXL"
	fi
	
	export SIDIS_TEST="${OPT}"
	export TEE_FNAME="${FNAME}"
#	echo ${SIDIS_TEST}
#	echo ${TEE_FNAME} ${QPAR}
	/usr/local/bin/qsub -V -q${QPAR} qsub_base.sh

    else
	
	OPT="${OPTBASE} --ngrid=2"
	FNAME="L${order}"
	
	if [ ${order} = LO ]; then
	    OPT="${OPT} --order=LO"
	    QPAR="XXL"
	else
	    OPT="${OPT} --order=NLO"
	    QPAR="MC"
	fi
	
	export 	SIDIS_TEST="${OPT}"
	export TEE_FNAME="${FNAME}"
#	echo ${SIDIS_TEST}
#	echo ${TEE_FNAME} ${QPAR}
	/usr/local/bin/qsub -V -q${QPAR} qsub_base.sh
	
    fi
    
  done
  
done