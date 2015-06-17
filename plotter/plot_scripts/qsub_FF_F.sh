#!/bin/bash
cd ${PBS_O_WORKDIR}
rm -rf ./qsub_FF_F.sh.*

EXEC=/home/imazu/work/QCD/Mellin/browser/main
#IDIR=/home/imazu/work/QCD/tmpFit

Q2SCALE=10

for SORT in FF 
do

for ORD in NLO
do
    
for FIX in dynamic 
do
      
for OUTPUT in org
do

### Rescale ###
### ${SCALE} ###

for FNAME in `ls ${DIR_V} | grep ${KEY_V}`
do

  if [ ! -z ${FNAME} ]; then

      for SCALE in ${Q2list_V}
      do

	if [ ! -z `echo ${FNAME} | grep FFr${SCALE}_` ]; then
	    
	    OPT="--dtype=${SORT} --otype=${OUTPUT} \
		--nf=${FIX} --order=${ORD} \
		--${SORT}=${DIR_V}/${FNAME} \
		--rescaleFF=${SCALE} "
	    if [ ! -z "${ErrOpt_V}" ]; then
		OPT="${OPT} --comp --wError"
	    fi

	    time  ${EXEC} ${OPT}
	fi

      done

  fi

done

done
done
done
done
