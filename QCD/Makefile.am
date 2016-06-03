AM_CPPFLAGS = @LIBXML_CFLAGS@

# ----------------------------------------------------------------------- #
#                               libQCD                                    #
#
# **** order of Alpha.cc Flavor.cc Scheme.cc should be strictly in this 
# order. It will be studied sometime later, why the order is critical to
# create libarary. ( on the other hand, Alpha.cc should be in 3rd on Mac
# OS X system.... )
#
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libQCD.la
libQCD_la_SOURCES = 							\
Alpha.cc	Flavor.cc	Scheme.cc				\
Parameter.cc	PdfBase.cc	PdfParametrization.cc			\
ConfigPdfBase.cc	ConfigFFbase.cc					\
PdfSimpleXml.cc	PdfSimple.cc						\
FFSimple.cc	FFSimpleXml.cc						\
FFDisFavor.cc PdfErrMatrix.cc HadronParam.cc	

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #

bin_PROGRAMS = pdfManip
pdfManip_SOURCES = pdfManip.cc
pdfManip_LDADD   = libQCD.la ../Tranform/libTransform.la ../Utility/libUtility.la @LIBXML_LIBS@

noinst_PROGRAMS = testPdfBase alpha main
#noinst_PROGRAMS = main alpha

testPdfBase_SOURCES = testPdfBase.cc
testPdfBase_LDADD = libQCD.la ../Tranform/libTransform.la ../Utility/libUtility.la @LIBXML_LIBS@

alpha_SOURCES = alpha.cc
alpha_LDADD   = libQCD.la ../Tranform/libTransform.la ../Utility/libUtility.la @LIBXML_LIBS@

main_SOURCES = main.cc
main_LDADD   = libQCD.la ../Tranform/libTransform.la ../Utility/libUtility.la @LIBXML_LIBS@

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = 	\
Alpha.hh	\
ChargeBase.hh	\
ConfigFFbase.hh	\
ConfigPdfBase.hh	\
FFDisFavor.hh	\
FFSimple.hh	\
FFSimpleXml.hh	\
Flavor.hh	\
HadronParam.hh	\
Parameter.hh	\
PdfBase.hh	\
PdfErrMatrix.hh	\
PdfParametrization.hh	\
PdfSimple.hh	\
PdfSimpleXml.hh	\
Scheme.hh


install-data-local: @includedir@/QCD
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/QCD: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
