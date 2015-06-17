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
LIB_XSEC      = ../Xsec/libXsec.la $(LIB_EXPDATA) $(LIB_PDF_FF)


# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libunpolSIDISXsec.la
libunpolSIDISXsec_la_SOURCES = \
StrFuncSub.cc StrFunc.cc \
XsecComp.cc CoeffFunc.cc WC.cc Charge.cc WC2.cc \
xXsec.cc xzXsec.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main testF1h testHERMESKIN

main_SOURCES = main.cc
main_LDADD = libunpolSIDISXsec.la ../unpolDISXsec/libunpolDISXsec.la $(LIB_XSEC)

testF1h_SOURCES = testF1h.cc
testF1h_LDADD = libunpolSIDISXsec.la ../unpolDISXsec/libunpolDISXsec.la $(LIB_XSEC)

testHERMESKIN_SOURCES = testHERMESKIN.cc
testHERMESKIN_CPPFLAGS = `@ROOTCONF@ --cflags`
testHERMESKIN_LDFLAGS = `@ROOTCONF@ --libs` 
testHERMESKIN_LDADD = \
libunpolSIDISXsec.la ../unpolDISXsec/libunpolDISXsec.la $(LIB_XSEC)

noinst_PROGRAMS += testSub 
testSub_SOURCES = testSub.cc
testSub_LDADD   = libunpolSIDISXsec.la ../unpolDISXsec/libunpolDISXsec.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = CoeffFunc.hh  StrFuncSub.hh  WC2.hh       xXsec.hh	\
Charge.hh   StrFunc.hh    WC.hh          XsecComp.hh  xzXsec.hh

install-data-local: @includedir@/unpolSIDISXsec
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/unpolSIDISXsec: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
