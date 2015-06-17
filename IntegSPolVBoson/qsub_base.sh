#!/bin/bash
cd ${PBS_O_WORKDIR}

#time ./testSPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_cache.dat
#time ./testSPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_DSSV_MRST_cache.dat
time ./testSPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_DSSV_MRST_cache_n.dat
#time ./testSPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_BB_MRST_cache_n.dat
