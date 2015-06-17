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
LIB_FF        = ../Fragmentation/libFragmentation.la $(LIB_EVOLUTION)
LIB_XSEC      = ../Xsec/libXsec.la $(LIB_FF) ../ExpData/libExpData.la

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libFragXsec_T.la
libFragXsec_T_la_SOURCES = \
XFragXsec.cc FragXsec.cc XsecComp.cc CoeffFunc.cc Charge.cc WC.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testXsec testCharge testXsec_comp testInversion

testXsec_SOURCES = testXsec.cc
testXsec_LDADD = libFragXsec_T.la $(LIB_XSEC)

testCharge_SOURCES = testCharge.cc
testCharge_LDADD = libFragXsec_T.la $(LIB_XSEC)

testXsec_comp_SOURCES = testXsec_comp.cc
testXsec_comp_LDADD = libFragXsec_T.la $(LIB_XSEC)

testInversion_SOURCES = testInversion.cc
testInversion_LDADD = libFragXsec_T.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = CoeffFunc.hh WC.hh XsecComp.hh Charge.hh FragXsec.hh XFragXsec.hh

install-data-local: @includedir@/FragXsec_T
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/FragXsec_T: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
