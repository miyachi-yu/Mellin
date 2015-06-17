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
LIB_PDF_FF    = ../Fragmentation/libFragmentation.la  ../unpolPDF/libunpolPDF.la \
	$(LIB_EVOLUTION) $(LIB_PDFLIB)
LIB_EXPDATA   = ../ExpData/libExpData.la
LIB_XSEC      = ../IntegXsec/libIntegXsec.la ../Xsec/libXsec.la \
	$(LIB_EXPDATA) $(LIB_PDF_FF)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libIntegUnpolSIDIS.la
libIntegUnpolSIDIS_la_SOURCES = \
StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc COFbase.cc Charge.cc nXsecComp.cc \
PStrFunc.cc 

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testF1h checkPartialInteg testF1h_xz

testF1h_SOURCES = testF1h.cc
testF1h_LDADD = libIntegUnpolSIDIS.la ../unpolSIDIS/libunpolSIDIS.la \
$(LIB_XSEC) 

checkPartialInteg_SOURCES = checkPartialInteg.cc
checkPartialInteg_LDADD = libIntegUnpolSIDIS.la $(LIB_XSEC)

testF1h_xz_SOURCES = testF1h_xz.cc
testF1h_xz_LDADD = libIntegUnpolSIDIS.la ../unpolSIDIS/libunpolSIDIS.la \
$(LIB_XSEC) 

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = COFbase.hh  CoeffFunc.hh     StrFunc.hh  XsecComp.hh	\
Charge.hh   PartialInteg.hh  WC.hh  nXsecComp.hh  PStrFunc.hh

install-data-local: @includedir@/IntegUnpolSIDIS
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/IntegUnpolSIDIS: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

