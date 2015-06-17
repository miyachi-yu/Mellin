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
LIB_EXPDATA   = ../ExpData/libExpData.la
LIB_XSEC      = ../Xsec/libXsec.la $(LIB_EXPDATA) $(LIB_POLPDF)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libpolDIS.la
libpolDIS_la_SOURCES = StrFunc.cc XsecComp.cc CoeffFunc.cc Wilson.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testG1 interpolation testG1_imazu

testG1_SOURCES = testG1.cc
testG1_LDADD = libpolDIS.la $(LIB_XSEC)

testG1_imazu_SOURCES = testG1_imazu.cc
testG1_imazu_LDADD = libpolDIS.la $(LIB_XSEC)

interpolation_SOURCES = interpolation.cc
interpolation_LDADD = libpolDIS.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = CoeffFunc.hh  StrFunc.hh  Wilson.hh  XsecComp.hh

install-data-local: @includedir@/polDIS
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/polDIS: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
