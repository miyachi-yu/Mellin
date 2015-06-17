#!/bin/bash
### pion ###
../Xsecplot --expdata=expdata_direct_werr_pion.xml  --logy --ymin=0.0001 --sumplot=4,3,2,1 --ymax=1000000 --xmax=0.9 --format=eps

### pi0 ###
../Xsecplot --expdata=expdata_direct_werr_pion0.xml  --logy --ymin=0.001 --sumplot=3,4,5,1 --ymax=100000 --xmax=0.9 --format=eps

### kaon ###
../Xsecplot --expdata=expdata_direct_werr_kaon.xml  --logy --ymin=0.0001 --sumplot=5,4,3,2 --ymax=20000 --xmax=0.9 --format=eps

### proton ###
../Xsecplot --expdata=expdata_direct_werr_proton.xml  --logy --ymin=0.00001 --sumplot=4,3,2,6 --ymax=50000 --xmax=0.9 --format=eps

### hadron ###
../Xsecplot --expdata=expdata_direct_werr_hadron.xml  --logy --ymin=0.0001 --sumplot=5,4,1,2 --ymax=1000000 --xmax=1.0 --format=eps
