# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_EVOLUTION = ../Evolution/libEvolution.la $(LIB_TRANSFORM) $(LIB_QCD)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libpolFrag.la
libpolFrag_la_SOURCES = Evolution.cc P.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main

main_SOURCES = main.cc
main_LDADD = libpolFrag.la $(LIB_EVOLUTION) $(LIB_UTILITY)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = Evolution.hh

install-data-local: @includedir@/polFrag
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/polFrag: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
