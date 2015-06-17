# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libXsec.la
libXsec_la_SOURCES = Coefficient.cc Xsec.cc XsecComp.cc Wilsons.cc	\
	CKernel.cc xXsec.cc Charge.cc xzXsec.cc PDF_SW.cc CKernelBase.cc \
	XsecCompBase.cc

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
MYHEADERS =	\
CKernel.hh	\
CKernelBase.hh	\
Charge.hh	\
Coefficient.hh	\
PDF_SW.hh	\
Wilsons.hh	\
Xsec.hh	\
XsecCompBase.hh	\
XsecComp.hh	\
xXsec.hh	\
xzXsec.hh


install-data-local: @includedir@/Xsec
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/Xsec: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
