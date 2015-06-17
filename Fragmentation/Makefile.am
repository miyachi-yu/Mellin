# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_EVOLUTION = ../Evolution/libEvolution.la $(LIB_TRANSFORM) $(LIB_QCD) $(LIB_TRANSFORM)

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libFragmentation.la
libFragmentation_la_SOURCES = Evolution.cc P.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main
main_SOURCES    = main.cc
main_LDADD      = libFragmentation.la $(LIB_EVOLUTION) $(LIB_UTILITY)

noinst_PROGRAMS     += testFFSimple
testFFSimple_SOURCES = testFFSimple.cc
testFFSimple_LDADD   = libFragmentation.la $(LIB_EVOLUTION) $(LIB_UTILITY)

noinst_PROGRAMS += hadron
hadron_SOURCES   = hadron.cc
hadron_LDADD     = libFragmentation.la $(LIB_EVOLUTION) $(LIB_UTILITY)

noinst_PROGRAMS += diff
diff_SOURCES     = diff.cc
diff_LDADD       = libFragmentation.la $(LIB_EVOLUTION) $(LIB_UTILITY)

bin_PROGRAMS     = printFF
printFF_SOURCES  = printFF.cc
printFF_LDADD    = libFragmentation.la $(LIB_EVOLUTION) $(LIB_UTILITY)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = Evolution.hh

install-data-local: @includedir@/Fragmentation
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/Fragmentation: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
