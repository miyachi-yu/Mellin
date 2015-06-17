# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)
LIB_PDF_FF    = ../Fragmentation/libFragmentation.la ../polPDF/libpolPDF.la \
	$(LIB_EVOLUTION)
LIB_EXPDATA   = ../ExpData/libExpData.la
LIB_XSEC      = ../IntegXsec/libIntegXsec.la ../Xsec/libXsec.la \
	$(LIB_EXPDATA) $(LIB_PDF_FF)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libIntegPolSIDIS.la
libIntegPolSIDIS_la_SOURCES = \
StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc COFbase.cc Charge.cc nXsecComp.cc \
PStrFunc.cc 

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testG1h checkPartialInteg testG1h_xz

testG1h_SOURCES = testG1h.cc
testG1h_LDADD = libIntegPolSIDIS.la ../polSIDIS/libpolSIDIS.la $(LIB_XSEC)

checkPartialInteg_SOURCES = checkPartialInteg.cc
checkPartialInteg_LDADD = libIntegPolSIDIS.la $(LIB_XSEC)

testG1h_xz_SOURCES = testG1h_xz.cc
testG1h_xz_LDADD = libIntegPolSIDIS.la ../polSIDIS/libpolSIDIS.la \
$(LIB_XSEC) 

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = COFbase.hh  CoeffFunc.hh     StrFunc.hh  XsecComp.hh	\
Charge.hh   PartialInteg.hh  WC.hh  nXsecComp.hh  PStrFunc.hh

install-data-local: @includedir@/IntegPolSIDIS
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/IntegPolSIDIS: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

