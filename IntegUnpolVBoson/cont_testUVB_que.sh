#!/bin/bash
rm -rf ./qsub_base.sh.*

for vboson in WP WM Z0 
do

#OPTBASE="--unpolPDF=../browser/compare_unpolPDF.xml"
#OPTBASE="${OPTBASE} --noCache --length=10.0 --wxcalc --xShortInteg --mxloop=2"
OPTBASE="--MRST=MRST2001.xml"
OPTBASE="${OPTBASE} --noCache --length=10.0 --wxcalc --xShortInteg --mxloop=2"
OPTBASE="${OPTBASE} --GBscale"
    
    if [ ${vboson} != Z0 ]; then
	OPTBASE="${OPTBASE} --${vboson}BPro"
    fi

#for longshort in Short Long
    for longshort in Short
    do
	
#  for order in LO
#  for order in LO NLO
	for order in NLO
	do
	    
	    if [ ${longshort} = Short ]; then
		
		OPT="${OPTBASE} --ShortInteg --ngrid=8"
		FNAME="S${order}_${vboson}"
		
		if [ ${order} = LO ]; then
		    OPT="${OPT} --order=LO"
#	    QPAR="XXL"
		    QPAR="XL"
		else
		    OPT="${OPT} --order=NLO"
#	    QPAR="XXL"
		    QPAR="XL"
		fi
		
		export SIDIS_TEST="${OPT}"
		export TEE_FNAME="${FNAME}"
#	echo ${SIDIS_TEST}
#	echo ${TEE_FNAME} ${QPAR}
		/usr/local/bin/qsub -V -q${QPAR} qsub_base.sh
		
	    else
		
		OPT="${OPTBASE} --ngrid=2"
		FNAME="L${order}_${vboson}"
		
		if [ ${order} = LO ]; then
		    OPT="${OPT} --order=LO"
#	    QPAR="XXL"
		    QPAR="XL"
		else
		    OPT="${OPT} --order=NLO"
#	    QPAR="MC"
		    QPAR="XXL"
		fi
		
		export 	SIDIS_TEST="${OPT}"
		export TEE_FNAME="${FNAME}"
#	echo ${SIDIS_TEST}
#	echo ${TEE_FNAME} ${QPAR}
		/usr/local/bin/qsub -V -q${QPAR} qsub_base.sh
		
	    fi
	    
	done
	
    done

done