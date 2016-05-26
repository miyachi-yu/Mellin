AM_CPPFLAGS = @LIBXML_CFLAGS@

# ----------------------------------------------------------------------- #
#                               libPDFSET                                 #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libPDFSET.la
libPDFSET_la_SOURCES = 							\
BBParam.cc								\
MRSTParam.cc	PdfMRST.cc	PdfMRSTXml.cc	ConfigMRST.cc		\
KretzerParam.cc	KretzerFF.cc	KretzerFFXml.cc	ConfigKretzerFF.cc	\
DSSFFParam.cc	DSSFF.cc	DSSFFXml.cc	ConfigDSSFF.cc		\
DSSVParam.cc	PdfDSSV.cc	PdfDSSVXml.cc	ConfigDSSV.cc		\
StatParam.cc	PdfStat.cc	PdfStatXml.cc	ConfigStat.cc		\
Cteq6Param.cc	PdfCteq6.cc	PdfCteq6Xml.cc	ConfigCteq6.cc		\
MSTWParam.cc

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
#noinst_PROGRAMS =


# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = 								\
BBParam.hh								\
DSSFF.hh	DSSFFParam.hh	DSSFFXml.hh	ConfigDSSFF.hh		\
PdfDSSV.hh	DSSVParam.hh	PdfDSSVXml.hh	ConfigDSSV.hh		\
PdfStat.hh	StatParam.hh	PdfStatXml.hh	ConfigStat.hh		\
KretzerFF.hh	KretzerParam.hh	KretzerFFXml.hh	ConfigKretzerFF.hh	\
PdfMRST.hh	MRSTParam.hh	PdfMRSTXml.hh	ConfigMRST.hh		\
PdfCteq6.hh	Cteq6Param.hh	PdfCteq6Xml.hh	ConfigCteq6.hh		\
MSTWParam.hh

install-data-local: @includedir@/QCD
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/QCD: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
