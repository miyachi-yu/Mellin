# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libStrFncParam.la
libStrFncParam_la_SOURCES   = \
ALLM91.cc  F1Param.cc      F2dSMC.cc  PhotoAbsorbXsec.cc  RParam.cc	\
ALLM97.cc  F2Param.cc      F2nSMC.cc  R1990.cc				\
DoLara.cc  F2ParamBase.cc  F2pSMC.cc  R1998.cc  GD07P.cc GD11P.cc

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main
main_SOURCES = main.cc
main_LDADD   =	libStrFncParam.la		\
		../Tranform/libTransform.la	\
		../Utility/libUtility.la	\
		@LIBXML_LIBS@

noinst_PROGRAMS += allm97
allm97_SOURCES = allm97.cc
allm97_LDADD   =	libStrFncParam.la		\
			../Tranform/libTransform.la	\
			../Utility/libUtility.la	\
			@LIBXML_LIBS@

noinst_PROGRAMS += all3
all3_SOURCES = all3.cc
all3_LDADD   =	libStrFncParam.la		\
			../Tranform/libTransform.la	\
			../Utility/libUtility.la	\
			@LIBXML_LIBS@

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = 	\
ALLM91.hh	\
ALLM97.hh	\
DoLara.hh	\
F1Param.hh	\
F2Param.hh	\
F2ParamBase.hh	\
F2dSMC.hh	\
F2nSMC.hh	\
F2pSMC.hh	\
PhotoAbsorbXsec.hh	\
R1990.hh	\
R1998.hh	\
RParam.hh	\
GD07P.hh	\
GD11P.hh

install-data-local: @includedir@/StrFncParam
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/StrFncParam: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
