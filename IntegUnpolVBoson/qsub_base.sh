#!/bin/bash
cd ${PBS_O_WORKDIR}

#time ./testUVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_MRST_cache.dat
time ./testUVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_MRST_cache.dat