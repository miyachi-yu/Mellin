# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../PDFSET/libPDFSET.la ../QCD/libQCD.la $(LIB_TRANSFORM)
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la $(LIB_QCD) $(LIB_PDFLIB) $(LIB_UTILITY)
LIB_UNPOLPDF  = ../unpolPDF/libunpolPDF.la $(LIB_EVOLUTION)
LIB_EXPDATA   = ../ExpData/libExpData.la
LIB_XSEC      = ../Xsec/libXsec.la $(LIB_EXPDATA) $(LIB_UNPOLPDF)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libunpolDISXsec.la
libunpolDISXsec_la_SOURCES = StrFunc.cc XsecComp.cc CoeffFunc.cc 	\
WC.cc WC2.cc xXsec.cc IMIntegrand.cc StrFuncSub.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main testF1

main_SOURCES = main.cc
main_LDADD = libunpolDISXsec.la $(LIB_XSEC)

testF1_SOURCES = testF1.cc
testF1_LDADD = libunpolDISXsec.la $(LIB_XSEC)


# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = WC.hh XsecComp.hh CoeffFunc.hh StrFunc.hh WC2.hh StrFuncSub.hh

install-data-local: @includedir@/unpolDISXsec
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/unpolDISXsec: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
