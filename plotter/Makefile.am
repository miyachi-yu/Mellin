# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
AM_CPPFLAGS = `@ROOTCONF@ --cflags`

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)

LIB_PDF_FF    = \
	../Fragmentation/libFragmentation.la  \
	../unpolPDF/libunpolPDF.la \
	../polPDF/libpolPDF.la \
	$(LIB_EVOLUTION)  $(LIB_PDFLIB)

LIB_EXPDATA   = ../ExpData/libExpData.la

LIB_XSEC      = \
	../FragXsec_T/libFragXsec_T.la	\
	../polDIS/libpolDIS.la		\
	../unpolDIS/libunpolDIS.la	\
	../unpolSIDIS/libunpolSIDIS.la	\
	../unpolDISXsec/libunpolDISXsec.la \
	../unpolSIDISXsec/libunpolSIDISXsec.la \
	../polSIDIS/libpolSIDIS.la	\
	../Xsec/libXsec.la $(LIB_EXPDATA) $(LIB_PDF_FF)

LIB_ERRCALC = ../ErrorCalc/libErrorCalc.la

# ----------------------------------------------------------------------- #
#                             Programs                                    #
# ----------------------------------------------------------------------- #
bin_PROGRAMS = LMmanip chi2_plot error_MX_hist view_HERMES view_emc

LMmanip_SOURCES = LMmanip.cc
LMmanip_LDADD = $(LIB_UTILITY)

chi2_plot_SOURCES = chi2_plot.cc TGraVmk.cc
chi2_plot_CPPFLAGS = `@ROOTCONF@ --cflags`
chi2_plot_LDFLAGS = `@ROOTCONF@ --libs` -lMathMore
chi2_plot_LDADD = $(LIB_UTILITY)

view_HERMES_SOURCES = view_HERMES.cc
view_HERMES_CPPFLAGS = `@ROOTCONF@ --cflags`
view_HERMES_LDFLAGS = `@ROOTCONF@ --libs`
view_HERMES_LDADD = $(LIB_XSEC)

view_emc_SOURCES = view_emc.cc
view_emc_CPPFLAGS = `@ROOTCONF@ --cflags`
view_emc_LDFLAGS = `@ROOTCONF@ --libs`
view_emc_LDADD = $(LIB_XSEC)

error_MX_hist_SOURCES = error_MX_hist.cc
error_MX_hist_LDFLAGS = `@ROOTCONF@ --libs`
error_MX_hist_LDADD = $(LIB_XSEC)

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = example plotDST plotXS correlation VBPplot \
	scale_plotDST multiPanel multiPanelHERMES \
	multiSummary

multiSummary_SOURCES = MultiFF.cc MultiPanel.cc multiSummary.cc
multiSummary_LDFLAGS = `@ROOTCONF@ --libs`
multiSummary_LDADD = $(LIB_XSEC)

multiPanelHERMES_SOURCES = multiPanelHERMES.cc
multiPanelHERMES_LDFLAGS = `@ROOTCONF@ --libs`
multiPanelHERMES_LDADD = $(LIB_XSEC)

multiPanel_SOURCES = multiPanel.cc
multiPanel_LDFLAGS = `@ROOTCONF@ --libs`
multiPanel_LDADD = $(LIB_XSEC)

example_SOURCES = example.cc
example_LDFLAGS = `@ROOTCONF@ --libs`
example_LDADD = $(LIB_XSEC)

plotDST_SOURCES = plotDST.cc
plotDST_CPPFLAGS = `@ROOTCONF@ --cflags`
plotDST_LDFLAGS = `@ROOTCONF@ --libs`
plotDST_LDADD = $(LIB_PDF_FF)

VBPplot_SOURCES = VBPplot.cc
VBPplot_CPPFLAGS = `@ROOTCONF@ --cflags`
VBPplot_LDFLAGS = `@ROOTCONF@ --libs`
VBPplot_LDADD = $(LIB_XSEC)

plotXS_SOURCES = plotXS.cc
plotXS_CPPFLAGS = `@ROOTCONF@ --cflags`
plotXS_LDFLAGS = `@ROOTCONF@ --libs`
plotXS_LDADD = $(LIB_XSEC) ../StrFncParam/libStrFncParam.la

#get_SOURCES = get.cc
#get_CPPFLAGS = `@ROOTCONF@ --cflags`
#get_LDFLAGS = `@ROOTCONF@ --libs`
#get_LDADD = $(LIB_XSEC)

correlation_SOURCES = correlation.cc
correlation_CPPFLAGS = `@ROOTCONF@ --cflags`
correlation_LDFLAGS = `@ROOTCONF@ --libs`
correlation_LDADD = $(LIB_XSEC)

scale_plotDST_SOURCES = scale_plotDST.cc
scale_plotDST_CPPFLAGS = `@ROOTCONF@ --cflags`
scale_plotDST_LDFLAGS = `@ROOTCONF@ --libs`
scale_plotDST_LDADD = $(LIB_XSEC) $(LIB_ERRCALC)
