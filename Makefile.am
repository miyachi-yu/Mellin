SUBDIRS = \
Utility Tranform StrFncParam QCD PDFSET PDFLIB		\
ExpData	\
Evolution unpolPDF polPDF Fragmentation polFrag		\
Xsec unpolDIS unpolDISXsec unpolSIDIS unpolSIDISXsec	\
polDIS polSIDIS FragXsec_T ErrorCalc			\
IntegXsec IntegFragXsec IntegUnpolDIS IntegUnpolSIDIS	\
IntegPolDIS IntegPolSIDIS IntegUnpolDrellYan IntegPolDrellYan	\
IntegUnpolVBoson IntegPolVBoson IntegSPolVBoson \
Fitting browser plotter script DrawTool

ACLOCAL_AMFLAGS = -I m4

# ----------------------------------------------------------------------- #
#                       headers to be installed                           #
# ----------------------------------------------------------------------- #
nobase_include_HEADERS = config.h

# ------------------------------------------------------------------------- #
#                                Local rules                                #
# ------------------------------------------------------------------------- #
.PHONY: doc
doc: Doxyfile
	@DOXYGEN@ Doxyfile

install-data-local: @prefix@/config
	$(INSTALL_DATA) config/*.xml @prefix@/config

@prefix@/config:
	$(INSTALL) --directory @prefix@/config

