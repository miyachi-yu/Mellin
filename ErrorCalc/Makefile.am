# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
AM_CPPFLAGS = `@ROOTCONF@ --cflags`

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_STRFNC    = ../StrFncParam/libStrFncParam.la
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)
LIB_PDF_FF    = \
	../Fragmentation/libFragmentation.la  \
	../unpolPDF/libunpolPDF.la \
	../polPDF/libpolPDF.la \
	$(LIB_EVOLUTION)  $(LIB_PDFLIB)

LIB_EXPDATA   = ../ExpData/libExpData.la
LIB_XSEC      = ../Xsec/libXsec.la $(LIB_EXPDATA) $(LIB_PDF_FF)
LIB_FRAGXSEC  = ../FragXsec_T/libFragXsec_T.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libErrorCalc.la
libErrorCalc_la_SOURCES = ErrCalc.cc PDFCorrCalc.cc XsecCorrCalc.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #

noinst_PROGRAMS = testERR testRealFunc

testERR_SOURCES = testERR.cc
testERR_LDFLAGS = `@ROOTCONF@ --ldflags --libs`
testERR_LDADD   = libErrorCalc.la $(LIB_FRAGXSEC)

testRealFunc_SOURCES = testRealFunc.cc
testRealFunc_LDFLAGS = `@ROOTCONF@ --ldflags --libs`
testRealFunc_LDADD   = libErrorCalc.la $(LIB_FRAGXSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = ErrCalc.hh  PDFCorrCalc.hh XsecCorrCalc.hh 

install-data-local: @includedir@/ErrorCalc
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/ErrorCalc: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

