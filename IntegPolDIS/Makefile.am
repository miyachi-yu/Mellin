# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_STRFNC    = ../StrFncParam/libStrFncParam.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)
LIB_POLPDF    = ../polPDF/libpolPDF.la $(LIB_EVOLUTION)
LIB_XSEC      = ../IntegXsec/libIntegXsec.la ../Xsec/libXsec.la \
$(LIB_POLPDF)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libIntegPolDIS.la
libIntegPolDIS_la_SOURCES = StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc \
COFbase.cc nXsecComp.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testG1

testG1_SOURCES = testG1.cc
testG1_LDADD = libIntegPolDIS.la ../polDIS/libpolDIS.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = COFbase.hh  CoeffFunc.hh  StrFunc.hh  WC.hh  XsecComp.hh \
nXsecComp.hh

install-data-local: @includedir@/IntegPolDIS
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/IntegPolDIS: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

