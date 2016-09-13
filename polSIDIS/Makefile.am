# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../PDFSET/libPDFSET.la ../QCD/libQCD.la $(LIB_TRANSFORM) 
LIB_STRFNC    = ../StrFncParam/libStrFncParam.la
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la $(LIB_QCD) $(LIB_UTILITY)
LIB_PDF_FF    = ../Fragmentation/libFragmentation.la  \
	../unpolPDF/libunpolPDF.la ../polPDF/libpolPDF.la \
	$(LIB_EVOLUTION)  $(LIB_PDFLIB)
LIB_EXPDATA   = ../ExpData/libExpData.la
LIB_XSEC      = ../Xsec/libXsec.la $(LIB_EXPDATA) $(LIB_PDF_FF)
LIB_UNPOLSIDIS = ../unpolSIDIS/libunpolSIDIS.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libpolSIDIS.la
libpolSIDIS_la_SOURCES = StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc \
../unpolSIDIS/Charge.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testG1h testPDF testG1h_imazu

testG1h_SOURCES = testG1h.cc
testG1h_LDADD = libpolSIDIS.la $(LIB_UNPOLSIDIS)

testG1h_imazu_SOURCES = testG1h_imazu.cc
testG1h_imazu_LDADD = libpolSIDIS.la $(LIB_UNPOLSIDIS)

testPDF_SOURCES = testPDF.cc
testPDF_LDADD = libpolSIDIS.la $(LIB_UNPOLSIDIS)

noinst_PROGRAMS += diff
diff_SOURCES = diff.cc
diff_LDADD = libpolSIDIS.la $(LIB_UNPOLSIDIS)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = CoeffFunc.hh  StrFunc.hh  WC.hh  XsecComp.hh

install-data-local: @includedir@/polSIDIS
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/polSIDIS: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
