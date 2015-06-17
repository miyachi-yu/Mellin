# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
		$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_PDFLIB)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libunpolPDF.la
libunpolPDF_la_SOURCES = Evolution.cc P.cc CTEQ6pdf.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main testCteq6

main_SOURCES = main.cc
main_LDADD = libunpolPDF.la $(LIB_EVOLUTION) $(LIB_UTILITY)

testCteq6_SOURCES = testCteq6.cc
testCteq6_LDADD = libunpolPDF.la $(LIB_EVOLUTION) $(LIB_UTILITY)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = 	\
CTEQ6pdf.hh	\
Evolution.hh

install-data-local: @includedir@/unpolPDF
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/unpolPDF: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
