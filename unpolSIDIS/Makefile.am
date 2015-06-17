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
LIB_PDF_FF    = ../Fragmentation/libFragmentation.la  \
	../unpolPDF/libunpolPDF.la \
	$(LIB_EVOLUTION) $(LIB_PDFLIB)
LIB_EXPDATA   = ../ExpData/libExpData.la
LIB_XSEC      = ../Xsec/libXsec.la $(LIB_EXPDATA) $(LIB_PDF_FF)


# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libunpolSIDIS.la
libunpolSIDIS_la_SOURCES = \
StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc Charge.cc 

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main testF1h testF1h_imazu

main_SOURCES = main.cc
main_LDADD = libunpolSIDIS.la $(LIB_XSEC)

testF1h_SOURCES = testF1h.cc
testF1h_LDADD = libunpolSIDIS.la $(LIB_XSEC)

testF1h_imazu_SOURCES = testF1h_imazu.cc
testF1h_imazu_LDADD = libunpolSIDIS.la $(LIB_XSEC)

noinst_PROGRAMS += hadron
hadron_SOURCES = hadron.cc
hadron_LDADD   = libunpolSIDIS.la $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = Charge.hh  CoeffFunc.hh  StrFunc.hh  WC.hh  XsecComp.hh

install-data-local: @includedir@/unpolSIDIS
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/unpolSIDIS: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
