#!/bin/bash
cd ${PBS_O_WORKDIR}

#time ./testSPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_OURS_MRST_cache_n.dat
time ./testSPVB ${SIDIS_TEST} | tee tmp${TEE_FNAME}_OURS_MRST_cache_x_MC.dat
