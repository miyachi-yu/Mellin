# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)
LIB_POLPDF  = ../polPDF/libpolPDF.la $(LIB_EVOLUTION) $(LIB_PDFLIB)
LIB_UNPOLPDF  = ../unpolPDF/libunpolPDF.la $(LIB_EVOLUTION) $(LIB_PDFLIB)
LIB_XSEC      = ../IntegXsec/libIntegXsec.la ../Xsec/libXsec.la \
$(LIB_POLPDF) $(LIB_UNPOLPDF)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libIntegSPolVBoson.la
libIntegSPolVBoson_la_SOURCES = \
StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc COFbase.cc Charge.cc nXsecComp.cc \
nStrFunc.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testSPVB

testSPVB_SOURCES = testSPVB.cc
testSPVB_LDADD = libIntegSPolVBoson.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = \
COFbase.hh  CoeffFunc.hh    DYLogPlusFunc.hh  WC.hh	\
Charge.hh   DYDPlusFunc.hh  DYPlusFunc.hh     StrFunc.hh       XsecComp.hh \
nXsecComp.hh nStrFunc.hh

install-data-local: @includedir@/IntegSPolVBoson
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/IntegSPolVBoson: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

