# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
#AM_CPPFLAGS = 

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libEvolution.la
libEvolution_la_SOURCES = 						\
KernelBase.cc	KernelPdfLib.cc	Kernel.cc	PDFx.cc	PDF.cc		\
DGLAPMatrix.cc	DGLAPpdf.cc	AnomalousDimension.cc 			\
KernelSum.cc    KernelDiff.cc

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
MYHEADERS = 	\
AnomalousDimension.hh	\
DGLAPMatrix.hh	\
DGLAPpdf.hh	\
Kernel.hh	\
KernelBase.hh	\
KernelPdfLib.hh	\
KernelSum.hh	\
PDF.hh	\
PDFx.hh	\
WilsonCoefficient.hh

install-data-local: @includedir@/Evolution
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/Evolution: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
