# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../PDFSET/libPDFSET.la ../QCD/libQCD.la $(LIB_TRANSFORM)
LIB_EVOLUTION = ../Evolution/libEvolution.la $(LIB_QCD)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libpolPDF.la
libpolPDF_la_SOURCES = Evolution.cc P.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main

main_SOURCES = main.cc
main_LDADD = libpolPDF.la $(LIB_EVOLUTION) $(LIB_UTILITY)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = Evolution.hh

install-data-local: @includedir@/polPDF
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/polPDF: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

