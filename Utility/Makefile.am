# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
AM_CPPFLAGS   = @LIBXML_CFLAGS@

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES         = libUtility.la
libUtility_la_SOURCES   = Arguments.cc Config.cc Timer.cc LookupTableCtl.cc Thread.cc

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = testArguments testConfig testThread

testArguments_SOURCES = testArguments.cc
testArguments_LDADD   = libUtility.la @LIBXML_LIBS@

testConfig_SOURCES = testConfig.cc
testConfig_LDADD   = libUtility.la @LIBXML_LIBS@

testThread_SOURCES = testThread.cc
testThread_LDADD   = libUtility.la @LIBXML_LIBS@ -lpthread

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS =	\
Arguments.hh Config.hh Timer.hh LookupTable.hh LookupTableCtl.hh Thread.hh

install-data-local: @includedir@/Utility
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/Utility: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

