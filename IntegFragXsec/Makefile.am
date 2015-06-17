# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)
LIB_FF        = ../Fragmentation/libFragmentation.la $(LIB_EVOLUTION)
LIB_XSEC      = ../IntegXsec/libIntegXsec.la ../Xsec/libXsec.la \
$(LIB_FF) ../ExpData/libExpData.la

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libIntegFragXsec.la
libIntegFragXsec_la_SOURCES = \
StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc COFbase.cc Charge.cc nXsecComp.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testXsec

testXsec_SOURCES = testXsec.cc
testXsec_LDADD = libIntegFragXsec.la ../FragXsec_T/libFragXsec_T.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = COFbase.hh  Charge.hh  CoeffFunc.hh  StrFunc.hh  WC.hh  \
XsecComp.hh nXsecComp.hh

install-data-local: @includedir@/IntegFragXsec
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/IntegFragXsec: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

