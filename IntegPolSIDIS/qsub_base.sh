#!/bin/bash
cd ${PBS_O_WORKDIR}

time ./testG1h ${SIDIS_TEST} | tee tmp${TEE_FNAME}_cache.dat