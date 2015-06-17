# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
AM_CPPFLAGS = -Df2cFortran

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libPDFLIB.la
libPDFLIB_la_SOURCES =				\
	CTEQ6.cc GRSV2000.cc Package.cc		\
	Cteq6Pdf-2004.f grsv2000pdf_g1.f		\
	LSS.cc LSS2006pdf_g1.f LSS2001.f  LSS2005pdf_g1.f dfint.f	\
	AAC.cc aac08.f BB.cc bb_ppdf.f		\
	dlib.f kkp.f pkhff.f grille_had_charged.f		\
	polin2.f polint.f locate.f FF.cc		\
	MRST2001.cc mrst2001E.f 			\
	mstwpdf.cc MSTW2008.cc			\
	hknsff07.f HKNS07.cc

# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
#noinst_PROGRAMS = main
#main_SOURCES = main.cc
#main_LDADD   = 	libPDFLIB.la 			\
#		../QCD/libQCD.la		\
#		../Tranform/libTransform.la	\
#		../Utility/libUtility.la	\
#		@LIBXML_LIBS@

noinst_PROGRAMS = hkns
hkns_SOURCES = hkns.cc
hkns_LDADD   = libPDFLIB.la 		\
	../QCD/libQCD.la	\
	../Tranform/libTransform.la	\
	../Utility/libUtility.la	\
	@LIBXML_LIBS@


# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = 	\
AAC.hh BB.hh CTEQ6.hh FF.hh GRSV2000.hh LSS.hh MRST2001.hh MSTW2008.hh Package.hh HKNS07.hh

TABLE = aac08_table bb_table cteq6_table ff_tables grsv2000_table lss06_table \
	mrst2001_table mstw2008_table hkns07_table

install-data-local: @includedir@/PDFLIB
	$(INSTALL_DATA) $(MYHEADERS) $<
	-@/bin/cp -r $(TABLE) $</.

@includedir@ @includedir@/PDFLIB:
	$(INSTALL) --directory $@

