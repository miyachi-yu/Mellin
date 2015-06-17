#!/bin/bash
rm -rf ./testSPVB_quebas.*

for error in 1
#for error in 0
do

for iflav in 100
#for iflav in 2 1 0 -1 -2
do

#for vboson in WM
for vboson in WP WM Z0 
do

#OPTBASE="--polPDF=../browser/compare_polPDF.xml"
#OPTBASE="${OPTBASE} --unpolPDF=../browser/compare_unpolPDF.xml"
#OPTBASE="${OPTBASE} --noCache --length=10.0 --wxcalc --xShortInteg --mxloop=2"
#OPTBASE="--DSSV=DSSV08.xml --MRST=MRST2001.xml"
#OPTBASE="--BB=polPDF_BB.xml --MRST=MRST2001.xml"
#OPTBASE="${OPTBASE} --noCache --length=10.0 --wxcalc --xShortInteg --mxloop=2"
OPTBASE="--DSSV=../browser/polPDF_Final/A1h_pPDF_NLO_0.8-0.2_werr_FBB.xml" 
OPTBASE="${OPTBASE} --MRST=MRST2001.xml"
OPTBASE="${OPTBASE} --noCache --length=10.0 --wxcalc --xShortInteg --mxloop=2"
#OPTBASE="${OPTBASE} --noCache --length=10.0 --mxloop=2"
OPTBASE="${OPTBASE} --GBscale"

    if [ ${vboson} != Z0 ]; then
	OPTBASE="${OPTBASE} --${vboson}BPro"
    fi

    if [ ${iflav} != 100 ]; then
	OPTBASE="${OPTBASE} --Flav1=${iflav}"
    fi

    if [ ${error} != 0 ]; then
	OPTBASE="${OPTBASE} --wError"
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

		if [ ${error} != 0 ]; then
		    FNAME="${FNAME}_wErr"
		fi
		
		if [ ${order} = LO ]; then
		    OPT="${OPT} --order=LO"
#	    QPAR="XXL"
#           QPAR="L"
		    QPAR="MC"
		else
		    OPT="${OPT} --order=NLO"
#	    QPAR="XXL"
#	    QPAR="L"
		    QPAR="MC"
		fi
		
		export SIDIS_TEST="${OPT}"
		export TEE_FNAME="${FNAME}"
#	echo ${SIDIS_TEST}
#	echo ${TEE_FNAME} ${QPAR}
		/usr/local/bin/qsub -V -q${QPAR} testSPVB_quebase.sh
		
	    else
		
		OPT="${OPTBASE} --ngrid=2"
		FNAME="L${order}_${vboson}"

		if [ ${iflav} != 100 ]; then
		    FNAME="${FNAME}_F${iflav}"
		fi
		
		if [ ${error} != 0 ]; then
		    FNAME="${FNAME}_wErr"
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
		/usr/local/bin/qsub -V -q${QPAR} testSPVB_quebase.sh
		
	    fi
	    
	done
	
    done

done

done

done