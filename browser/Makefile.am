# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
AM_CPPFLAGS = `@ROOTCONF@ --cflags`

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_STRFUNC   = ../StrFncParam/libStrFncParam.la
LIB_QCD       = ../QCD/libQCD.la ../PDFSET/libPDFSET.la
LIB_PDFLIB    = ../PDFLIB/libPDFLIB.la
LIB_EVOLUTION = ../Evolution/libEvolution.la \
	$(LIB_TRANSFORM) $(LIB_QCD) $(LIB_UTILITY)

LIB_PDF_FF    = \
	../Fragmentation/libFragmentation.la  \
	../unpolPDF/libunpolPDF.la \
	../polPDF/libpolPDF.la \
	$(LIB_EVOLUTION)  $(LIB_PDFLIB)

LIB_EXPDATA = ../ExpData/libExpData.la

LIB_XSEC      = \
	../FragXsec_T/libFragXsec_T.la	\
	../polDIS/libpolDIS.la		\
	../unpolDIS/libunpolDIS.la	\
	../unpolSIDIS/libunpolSIDIS.la	\
	../unpolDISXsec/libunpolDISXsec.la	\
	../unpolSIDISXsec/libunpolSIDISXsec.la	\
	../polSIDIS/libpolSIDIS.la	\
        ../Xsec/libXsec.la $(LIB_PDF_FF) $(LIB_EXPDATA)

LIB_INTEGXSEC  = \
	../IntegFragXsec/libIntegFragXsec.la \
	../IntegPolDIS/libIntegPolDIS.la \
	../IntegPolDrellYan/libIntegPolDrellYan.la \
	../IntegPolSIDIS/libIntegPolSIDIS.la \
	../IntegPolVBoson/libIntegPolVBoson.la \
	../IntegSPolVBoson/libIntegSPolVBoson.la \
	../IntegUnpolDIS/libIntegUnpolDIS.la \
	../IntegUnpolDrellYan/libIntegUnpolDrellYan.la \
	../IntegUnpolSIDIS/libIntegUnpolSIDIS.la \
	../IntegUnpolVBoson/libIntegUnpolVBoson.la \
	../IntegXsec/libIntegXsec.la

LIB_FIT = ../Fitting/libFitting.la
LIB_ERR = ../ErrorCalc/libErrorCalc.la

## ----------------------------------------------------------------------- #
##                   ROOT Object Dictionary Management                     #
## ----------------------------------------------------------------------- #
ROOTOBJ_CC  = Browser.cc PdfPanel.cc VerView.cc
ROOTOBJ_HH  = Browser.hh PdfPanel.hh VerView.hh
ROOTLINKDEF = brRootLinkDef.hh
ROOTDICT_CC = brRootObjDict.cc

## ----------------------------------------------------------------------- #
##                             Programs                                    #
## ----------------------------------------------------------------------- #
bin_PROGRAMS = mellin viewPDF viewFF viewFFq polPDFscale
#bin_PROGRAMS = viewPDF viewFF

mellin_SOURCES = mellin.cc				\
$(ROOTDICT_CC) $(ROOTOBJ_CC)				\
G1Panel.cc FragXsecPanel.cc ScaleFactor.cc PdfGraph.cc
mellin_LDFLAGS = `@ROOTCONF@ --glibs`
mellin_LDADD = $(LIB_EXPDATA) $(LIB_STRFUNC) $(LIB_XSEC)

bin_PROGRAMS     += parprint
parprint_SOURCES = parprint.cc TableMaker.cc
parprint_LDADD     = $(LIB_EXPDATA) $(LIB_STRFUNC) $(LIB_XSEC)

bin_PROGRAMS     += residual
residual_SOURCES = residual.cc
residual_LDFLAGS = `@ROOTCONF@ --libs`
residual_LDADD = $(LIB_XSEC)

bin_PROGRAMS     += residual_scale
residual_scale_SOURCES = residual_scale.cc
residual_scale_LDFLAGS = `@ROOTCONF@ --libs`
residual_scale_LDADD = $(LIB_XSEC)

bin_PROGRAMS     += residual_dist
residual_dist_SOURCES = residual_dist.cc
residual_dist_LDFLAGS = `@ROOTCONF@ --libs`
residual_dist_LDADD = $(LIB_XSEC) $(LIB_ERR)

bin_PROGRAMS     += dist_plot
dist_plot_SOURCES = dist_plot.cc
dist_plot_LDFLAGS = `@ROOTCONF@ --libs`
dist_plot_LDADD = $(LIB_XSEC) $(LIB_ERR)

ROOTCINT = `@ROOTCONF@ --prefix`/bin/@ROOTCINT@

## ----------------------------------------------------------------------- #
##                   ROOT Object Dictionary Management                     #
## ----------------------------------------------------------------------- #
$(ROOTDICT_CC) : $(ROOTOBJ_HH) $(ROOTLINKDEF)
	$(ROOTCINT) -f $@ -c $(ROOTOBJ_HH) $(ROOTLINKDEF)

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = main main_imazu viewExpData main_exp contour 	\
	viewMulti viewA1_h viewA1_h_M viewXsec readExp 		\
	Xsecplot testSPVB viewEmcMulti testMxManip		\
	hkns07

hkns07_SOURCES = hkns07.cc
hkns07_LDFLAGS = `@ROOTCONF@ --libs`
hkns07_LDADD = $(LIB_XSEC)

viewEmcMulti_SOURCES = viewEmcMulti.cc
viewEmcMulti_LDFLAGS = `@ROOTCONF@ --libs`
viewEmcMulti_LDADD = $(LIB_XSEC)

viewExpData_SOURCES = viewExpData.cc ExpDataPanel.cc
viewExpData_LDFLAGS = `@ROOTCONF@ --libs`
viewExpData_LDADD = $(LIB_XSEC) ${LIB_ERR}

viewMulti_SOURCES = viewMulti.cc ExpDataPanel.cc
viewMulti_LDFLAGS = `@ROOTCONF@ --libs`
viewMulti_LDADD = $(LIB_XSEC) ${LIB_ERR} 

viewXsec_SOURCES = viewXsec.cc ExpDataPanel.cc
viewXsec_LDFLAGS = `@ROOTCONF@ --libs`
viewXsec_LDADD = $(LIB_XSEC) ${LIB_ERR} 

viewA1_h_SOURCES = viewA1_h.cc ExpDataPanel.cc
viewA1_h_LDFLAGS = `@ROOTCONF@ --libs`
viewA1_h_LDADD = $(LIB_XSEC) ${LIB_ERR} 

viewA1_h_M_SOURCES = viewA1_h_M.cc ExpDataPanel.cc
viewA1_h_M_LDFLAGS = `@ROOTCONF@ --libs`
viewA1_h_M_LDADD = $(LIB_XSEC) ${LIB_ERR} 

main_imazu_SOURCES = main_imazu.cc
main_imazu_LDFLAGS = `@ROOTCONF@ --libs`
main_imazu_LDADD = $(LIB_XSEC) ${LIB_ERR} 

main_SOURCES = main.cc
main_LDFLAGS = `@ROOTCONF@ --libs`
main_LDADD = $(LIB_XSEC) ${LIB_ERR} 

testSPVB_SOURCES = testSPVB.cc
testSPVB_LDFLAGS = `@ROOTCONF@ --libs`
testSPVB_LDADD = $(LIB_XSEC) $(LIB_INTEGXSEC) ${LIB_ERR} 

polPDFscale_SOURCES = polPDFscale.cc
polPDFscale_LDFLAGS = `@ROOTCONF@ --libs`
polPDFscale_LDADD = $(LIB_XSEC) ${LIB_ERR} 

Xsecplot_SOURCES = Xsecplot.cc
Xsecplot_LDFLAGS = `@ROOTCONF@ --libs`
Xsecplot_LDADD = $(LIB_XSEC)

readExp_SOURCES = readExp.cc
readExp_LDFLAGS = `@ROOTCONF@ --libs`
readExp_LDADD = $(LIB_XSEC) ${LIB_ERR} 

main_exp_SOURCES = main_exp.cc
main_exp_LDFLAGS = `@ROOTCONF@ --libs`
main_exp_LDADD = $(LIB_XSEC) ${LIB_ERR} 

contour_SOURCES = contour.cc InverseMellinHelper.cc RealFunctionWrapper.cc
contour_CPPFLAGS = `@ROOTCONF@ --cflags`
contour_LDFLAGS = `@ROOTCONF@ --libs`
contour_LDADD = $(LIB_XSEC) 

viewPDF_SOURCES = viewPDF.cc
viewPDF_LDFLAGS = `@ROOTCONF@ --libs`
viewPDF_LDADD = $(LIB_XSEC)

viewFF_SOURCES = viewFF.cc
viewFF_LDFLAGS = `@ROOTCONF@ --libs`
viewFF_LDADD = $(LIB_XSEC)

viewFFq_SOURCES = viewFFq.cc Colors.cc
viewFFq_LDFLAGS = `@ROOTCONF@ --libs`
viewFFq_LDADD = $(LIB_XSEC)

testMxManip_SOURCES = testMxManip.cc
testMxManip_LDFLAGS = `@ROOTCONF@ --libs`

noinst_PROGRAMS += mrst2001
mrst2001_SOURCES = mrst2001.cc
mrst2001_LDFLAGS = `@ROOTCONF@ --libs`
mrst2001_LDADD = $(LIB_XSEC)

noinst_PROGRAMS += mstw2008
mstw2008_SOURCES = mstw2008.cc Colors.cc
mstw2008_LDFLAGS = `@ROOTCONF@ --libs`
mstw2008_LDADD = $(LIB_XSEC)

noinst_PROGRAMS += viewA1Exp
viewA1Exp_SOURCES = viewA1Exp.cc
viewA1Exp_LDFLAGS = `@ROOTCONF@ --libs` -lMinuit
viewA1Exp_LDADD = $(LIB_FIT) $(LIB_ERR) $(LIB_XSEC) 


## ----------------------------------------------------------------------- #
##                       Local clean rule                                  #
## ----------------------------------------------------------------------- #
clean-local:
	/bin/rm -f $(ROOTDICT_CC) $(patsubst %.cc, %.h, $(ROOTDICT_CC))
