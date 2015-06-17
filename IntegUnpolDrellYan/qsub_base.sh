#!/bin/bash
cd ${PBS_O_WORKDIR}

time ./testUDY ${SIDIS_TEST} | tee tmp${TEE_FNAME}_cache_2.dat