# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_STRFNC    = ../StrFncParam/libStrFncParam.la
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)
LIB_UNPOLPDF  = ../unpolPDF/libunpolPDF.la $(LIB_EVOLUTION) $(LIB_PDFLIB)
LIB_XSEC      = ../IntegXsec/libIntegXsec.la ../Xsec/libXsec.la \
	$(LIB_UNPOLPDF)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libIntegUnpolDIS.la
libIntegUnpolDIS_la_SOURCES = StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc \
COFbase.cc nXsecComp.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main testF1

main_SOURCES = main.cc
main_LDADD = libIntegUnpolDIS.la $(LIB_XSEC)

testF1_SOURCES = testF1.cc
testF1_LDADD = libIntegUnpolDIS.la ../unpolDIS/libunpolDIS.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = COFbase.hh  CoeffFunc.hh  StrFunc.hh  WC.hh  XsecComp.hh \
nXsecComp.hh

install-data-local: @includedir@/IntegUnpolDIS
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/IntegUnpolDIS: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

