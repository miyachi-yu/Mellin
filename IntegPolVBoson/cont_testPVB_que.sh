#!/bin/bash
rm -rf ./qsub_base.sh.*

#for iflav in 100
#for iflav in 1 0 -1
#for iflav in 2 1 0 -1 -2
for iflav in -2
do

#for vboson in WP
#for vboson in Z0
for vboson in WP WM Z0 
do

#OPTBASE="--polPDF=../browser/compare_polPDF.xml"
#OPTBASE="${OPTBASE} --noCache --length=10.0 --wxcalc --xShortInteg --mxloop=2"
OPTBASE="--DSSV=DSSV08.xml"
#OPTBASE="--BB=polPDF_BB.xml"
#OPTBASE="${OPTBASE} --noCache --length=10.0 --wxcalc --xShortInteg --mxloop=2"
OPTBASE="${OPTBASE} --noCache --length=10.0 --mxloop=2"
OPTBASE="${OPTBASE} --GBscale"

    if [ ${vboson} != Z0 ]; then
	OPTBASE="${OPTBASE} --${vboson}BPro"
    fi

    if [ ${iflav} != 100 ]; then
	OPTBASE="${OPTBASE} --Flav1=${iflav}"
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

		if [ ${iflav} != 100 ]; then
		    FNAME="${FNAME}_F${iflav}"
		fi
		
		if [ ${order} = LO ]; then
		    OPT="${OPT} --order=LO"
#	    QPAR="XXL"
		    QPAR="M"
		else
		    OPT="${OPT} --order=NLO"
#	    QPAR="XXL"
		    QPAR="M"
		fi
		
		export SIDIS_TEST="${OPT}"
		export TEE_FNAME="${FNAME}"
#	echo ${SIDIS_TEST}
#	echo ${TEE_FNAME} ${QPAR}
		/usr/local/bin/qsub -V -q${QPAR} qsub_base.sh
		
	    else
		
		OPT="${OPTBASE} --ngrid=2"
		FNAME="L${order}_${vboson}"

		if [ ${iflav} != 100 ]; then
		    FNAME="${FNAME}_F${iflav}"
		fi
		
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

done