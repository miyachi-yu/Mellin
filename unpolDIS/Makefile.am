# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la $(LIB_UTILITY)
LIB_QCD       = ../PDFSET/libPDFSET.la ../QCD/libQCD.la $(LIB_TRANSFORM)
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la $(LIB_QCD) $(LIB_PDFLIB) 
LIB_UNPOLPDF  = ../unpolPDF/libunpolPDF.la $(LIB_EVOLUTION) 
LIB_EXPDATA   = ../ExpData/libExpData.la
LIB_XSEC      = ../Xsec/libXsec.la $(LIB_EXPDATA) $(LIB_UNPOLPDF) 

LIB_STRFNC    = ../StrFncParam/libStrFncParam.la

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libunpolDIS.la
libunpolDIS_la_SOURCES = StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main testF1 testF1_imazu

main_SOURCES = main.cc
main_LDADD = libunpolDIS.la $(LIB_XSEC) $(LIB_STRFNC)
testF1_SOURCES = testF1.cc
testF1_LDADD = libunpolDIS.la $(LIB_XSEC) $(LIB_STRFNC)

testF1_imazu_SOURCES = testF1_imazu.cc
testF1_imazu_LDADD = libunpolDIS.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = CoeffFunc.hh StrFunc.hh WC.hh XsecComp.hh

install-data-local: @includedir@/unpolDIS
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/unpolDIS: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
