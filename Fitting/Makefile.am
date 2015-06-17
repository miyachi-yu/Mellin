# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
AM_CPPFLAGS = `@ROOTCONF@ --cflags`

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la  ../PDFSET/libPDFSET.la
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)

LIB_PDF_FF    = \
	../Fragmentation/libFragmentation.la  \
	../unpolPDF/libunpolPDF.la \
	../polPDF/libpolPDF.la \
	$(LIB_EVOLUTION)  $(LIB_PDFLIB)

LIB_XSEC      = \
	../FragXsec_T/libFragXsec_T.la	\
	../polDIS/libpolDIS.la		\
	../polSIDIS/libpolSIDIS.la	\
	../unpolDIS/libunpolDIS.la	\
	../unpolSIDIS/libunpolSIDIS.la	\
	../unpolDISXsec/libunpolDISXsec.la	\
	../unpolSIDISXsec/libunpolSIDISXsec.la	\
	../Xsec/libXsec.la $(LIB_PDF_FF)

LIB_ERRCALC = ../ErrorCalc/libErrorCalc.la

LIB_EXPDATA = ../ExpData/libExpData.la

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libFitting.la
libFitting_la_SOURCES = Fitting.cc A1Data.cc MultiData.cc FragData.cc \
	FitData.cc TMatrix.cc 

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #

#bin_PROGRAMS = fitFragXsec
#fitFragXsec_SOURCES = FragXsecFit.cc testFragXsecFit.cc
#fitFragXsec_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
#fitFragXsec_LDADD   = \
#	$(LIB_ERRCALC) $(LIB_EXPDATA)				\
#	../FragXsec_T/libFragXsec_T.la ../Xsec/libXsec.la	\
#	$(LIB_PDF_FF) libFitting.la

#bin_PROGRAMS += fitG1
#fitG1_SOURCES = G1Fit.cc testG1Fit.cc
#fitG1_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
#fitG1_LDADD   = 					\
#	$(LIB_ERRCALC) $(LIB_EXPDATA)			\
#	../polDIS/libpolDIS.la ../Xsec/libXsec.la	\
#	$(LIB_PDF_FF) libFitting.la

bin_PROGRAMS =  fitMultiFrag
fitMultiFrag_SOURCES = MultiFragFit.cc testMultiFragFit.cc
fitMultiFrag_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
fitMultiFrag_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC) libFitting.la

bin_PROGRAMS +=  fitMultiFragLM
fitMultiFragLM_SOURCES = MultiFragFit.cc MultiFragFitLM.cc \
	testMultiFragFitLM.cc 
fitMultiFragLM_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
fitMultiFragLM_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC) libFitting.la

bin_PROGRAMS +=  fitMultiFrag_A1hLM
fitMultiFrag_A1hLM_SOURCES = MultiFragFit.cc A1hFit.cc \
	MultiFrag_A1hFitLM.cc testMultiFrag_A1hFitLM.cc 
fitMultiFrag_A1hLM_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
fitMultiFrag_A1hLM_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC) libFitting.la

bin_PROGRAMS +=  fitA1h
fitA1h_SOURCES = A1hFit.cc testA1hFit.cc 
fitA1h_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
fitA1h_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC) libFitting.la

bin_PROGRAMS +=  fitA1hLM
fitA1hLM_SOURCES = A1hFit.cc A1hFitLM.cc \
	testA1hFitLM.cc 
fitA1hLM_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
fitA1hLM_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC) libFitting.la


bin_PROGRAMS +=  fitUDratio
fitUDratio_SOURCES = UDratioFit.cc UDratioData.cc testUDratioFit.cc 
fitUDratio_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
fitUDratio_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC) libFitting.la

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
# noinst_PROGRAMS = testFragXsecFit testG1Fit

#bin_PROGRAMS += testFragXsecFit
#testFragXsecFit_SOURCES  = testFragXsecFit.cc FragXsecFit.cc Fitting.cc FragData.cc FitData.cc
##testFragXsecFit_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
#testFragXsecFit_LDFLAGS  = `@ROOTCONF@ --libs` -lMinuit
#testFragXsecFit_LDADD    = \
#	$(LIB_ERRCALC)	$(LIB_EXPDATA)				\
#	../FragXsec_T/libFragXsec_T.la ../Xsec/libXsec.la	\
#	$(LIB_PDF_FF)

#bin_PROGRAMS += testG1Fit
#testG1Fit_SOURCES  = G1Fit.cc testG1Fit.cc
#testG1Fit_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
#testG1Fit_LDFLAGS  = `@ROOTCONF@ --libs` -lMinuit
#testG1Fit_LDADD    = 					\
#	$(LIB_ERRCALC)	$(LIB_EXPDATA)			\
#	../polDIS/libpolDIS.la ../Xsec/libXsec.la	\
#	$(LIB_PDF_FF)

bin_PROGRAMS += testMultiFragFit
testMultiFragFit_SOURCES = testMultiFragFit.cc MultiFragFit.cc Fitting.cc \
MultiData.cc FragData.cc FitData.cc
testMultiFragFit_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
testMultiFragFit_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
testMultiFragFit_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC)

bin_PROGRAMS += testMultiFragFitLM
testMultiFragFitLM_SOURCES = testMultiFragFitLM.cc MultiFragFit.cc \
MultiFragFitLM.cc Fitting.cc MultiData.cc FragData.cc FitData.cc TMatrix.cc
testMultiFragFitLM_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
testMultiFragFitLM_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
testMultiFragFitLM_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC)

bin_PROGRAMS += testMultiFrag_A1hFitLM
testMultiFrag_A1hFitLM_SOURCES = testMultiFrag_A1hFitLM.cc \
MultiFrag_A1hFitLM.cc \
MultiFragFit.cc MultiData.cc FragData.cc A1hFit.cc A1Data.cc \
Fitting.cc FitData.cc TMatrix.cc
testMultiFrag_A1hFitLM_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
testMultiFrag_A1hFitLM_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
testMultiFrag_A1hFitLM_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC)

bin_PROGRAMS += testA1hFit 
testA1hFit_SOURCES = testA1hFit.cc A1hFit.cc Fitting.cc A1Data.cc FitData.cc
testA1hFit_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
testA1hFit_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
testA1hFit_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC)

bin_PROGRAMS += testA1hFitLM
testA1hFitLM_SOURCES = testA1hFitLM.cc A1hFit.cc \
A1hFitLM.cc Fitting.cc A1Data.cc FitData.cc TMatrix.cc
testA1hFitLM_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
testA1hFitLM_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
testA1hFitLM_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC)

bin_PROGRAMS += MultiFragScale
MultiFragScale_SOURCES = MultiFragScale.cc MultiFragFit.cc Fitting.cc MultiData.cc FragData.cc FitData.cc
MultiFragScale_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
MultiFragScale_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
MultiFragScale_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC)

bin_PROGRAMS += A1hScale 
A1hScale_SOURCES = A1hScale.cc A1hFit.cc Fitting.cc A1Data.cc FitData.cc
A1hScale_CPPFLAGS = `@ROOTCONF@ --cflags` -DDEBUG_FITTING
A1hScale_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
A1hScale_LDADD   = 					\
	$(LIB_XSEC) $(LIB_EXPDATA) $(LIB_ERRCALC)

# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = 	\
Fitting.hh A1Data.hh MultiData.hh FragData.hh FitData.hh TMatrix.hh

install-data-local: @includedir@/Fitting
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/Fitting: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@

