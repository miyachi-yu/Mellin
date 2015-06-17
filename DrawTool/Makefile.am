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
ROOTOBJ_CC  = RExpData.cc Manager.cc
ROOTOBJ_HH  = $(patsubst %.cc, %.hh, $(ROOTOBJ_CC) )
ROOTLINKDEF = brRootLinkDef.hh
ROOTDICT_CC = brRootObjDict.cc

## ----------------------------------------------------------------------- #
##                             Libraries                                   #
## ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libDrawTool.la
libDrawTool_la_SOURCES = $(ROOTDICT_CC) $(ROOTOBJ_CC)

## ----------------------------------------------------------------------- #
##                             Programs                                    #
## ----------------------------------------------------------------------- #
bin_PROGRAMS = drawTool

drawTool_SOURCES = test.cc
drawTool_LDFLAGS = `@ROOTCONF@ --glibs`
drawTool_LDADD = libDrawTool.la $(LIB_EXPDATA) $(LIB_STRFUNC) $(LIB_XSEC)

## ----------------------------------------------------------------------- #
##                   ROOT Object Dictionary Management                     #
## ----------------------------------------------------------------------- #
$(ROOTDICT_CC) : $(ROOTOBJ_HH) $(ROOTLINKDEF)
	@ROOTCINT@ -f $@ -c -I../ $(ROOTOBJ_HH) $(ROOTLINKDEF)

## ----------------------------------------------------------------------- #
##                       Local clean rule                                  #
## ----------------------------------------------------------------------- #
clean-local:
	/bin/rm -f $(ROOTDICT_CC) $(patsubst %.cc, %.h, $(ROOTDICT_CC))
