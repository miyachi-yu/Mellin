# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libIntegXsec.la
libIntegXsec_la_SOURCES = 					\
Charge.cc      ConvIntegrand.cc IntegXsecComp.cc  Wilsons.cc	\
XsecBase.cc    CKernel.cc       XsecIntegrand.cc  xXsec.cc	\
xzXsecInteg.cc xzXsec.cc        

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
# bin_PROGRAMS = 

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
# noinst_PROGRAMS =

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = CKernel.hh PlusFunc.hh xXsec.hh Charge.hh	\
DeltaFunc.hh Wilsons.hh xzXsec.hh ConvIntegPDF.hh IntegXsecComp.hh	\
XsecBase.hh xzXsecInteg.hh ConvIntegrand.hh  LogPlusFunc.hh XsecIntegrand.hh

install-data-local: @includedir@/IntegXsec
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/IntegXsec: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

