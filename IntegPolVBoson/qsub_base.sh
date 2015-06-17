#!/bin/bash
cd ${PBS_O_WORKDIR}

#time ./testPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_cache.dat
#time ./testPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_BB_cache_n.dat
time ./testPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_DSSV_cache_n.dat
