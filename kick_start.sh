#!/bin/sh
##
## kick_start.sh
## 
## Made by Yoshiyuki Miyachi
## Login   <miyachi@yoshiyuki-desktop>
## 
## Started on  Thu Sep 25 17:40:52 2008 Yoshiyuki Miyachi
## Last update Mon Sep 29 16:51:23 2008 Yoshiyuki Miyachi
##

SYSTEM=`uname -s`

echo "--> aclocal"
aclocal

echo "--> libtoolize --force --copy"
libtoolize --force --copy

echo "--> autoheader"
autoheader

echo "--> automake --copy --add-missing --force-missing"
automake --copy --add-missing --force-missing

echo "--> autoconf"
autoconf

echo "--> configure"
if [ ${SYSTEM} = "Darwin" ]
then
    CPPFLAGS=-I/sw/include LDFLAGS=-L/sw/lib ./configure
else
    ./configure
fi
