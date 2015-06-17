# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #

LIB_UTILITY   = ../Utility/libUtility.la @LIBXML_LIBS@
LIB_TRANSFORM = ../Tranform/libTransform.la
LIB_STRFUNC   = ../StrFncParam/libStrFncParam.la $(LIB_TRANSFORM)

DATADIR = -DDATA_DIR_PATH=\"@abs_top_srcdir@/Data/\"

# ----------------------------------------------------------------------- #
#                            libUtility                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libExpData.la
libExpData_la_SOURCES   = 					\
DataParser.cc HERMES.cc						\
ExpData.cc ConfigDataSet.cc DataSet.cc DataPoint.cc Kinematic.cc
libExpData_la_CPPFLAGS = $(DATADIR) @LIBXML_CFLAGS@

# ----------------------------------------------------------------------- #
#                             programs                                    #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS  = loadTest
loadTest_SOURCES = loadTest.cc
loadTest_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)

bin_PROGRAMS = table_g1
table_g1_SOURCES = \
table_g1.cc hermes.cc e155.cc e154.cc e143.cc e142.cc smc.cc emc.cc compass.cc
table_g1_LDADD   = libExpData.la $(LIB_STRFUNC) $(LIB_UTILITY)
table_g1_CPPFLAGS = $(DATADIR)	

bin_PROGRAMS += table_fragXsec
table_fragXsec_SOURCES = table_fragXsec.cc hadron.cc HERMES_multiplicities.cc EMC_Multi.cc
table_fragXsec_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_fragXsec_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_kretzer
table_kretzer_SOURCES = table_kretzer.cc hadron.cc HERMES_MultiZ.cc HERMES_MultiQ2.cc EMC_Multi.cc
table_kretzer_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_kretzer_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_A1h
table_A1h_SOURCES = table_A1h.cc COMPASS_PLB680_217.cc
table_A1h_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_A1h_CPPFLAGS = $(DATADIR)


A1DATASRC = A1_Data.cc E143_A1.cc E155_A1.cc EMC_A1.cc HERMES_A1.cc \
	SMC_PhysRevD58_A1.cc COMPASS_PLB680_217.cc COMPASS_PLB690_466.cc

bin_PROGRAMS += table_A1
table_A1_SOURCES = table_A1.cc $(A1DATASRC)
table_A1_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_A1_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_Multi
table_Multi_SOURCES = table_Multi.cc HERMES_MultiZ.cc HERMES_MultiQ2.cc EMC_Multi.cc
table_Multi_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_Multi_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_Multi_K 
table_Multi_K_SOURCES = table_Multi_K.cc HERMES_MultiZ.cc HERMES_MultiQ2.cc EMC_Multi.cc
table_Multi_K_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_Multi_K_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_Xsec
table_Xsec_SOURCES = table_Xsec.cc hadron.cc
table_Xsec_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_Xsec_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += changeDataNorm
changeDataNorm_SOURCES = changeDataNorm.cc
changeDataNorm_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)

bin_PROGRAMS += table_PIpm
table_PIpm_SOURCES = table_PIpm.cc PIpm_Data.cc
table_PIpm_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_PIpm_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_Kpm
table_Kpm_SOURCES = table_Kpm.cc Kpm_Data.cc
table_Kpm_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_Kpm_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_hpm
table_hpm_SOURCES = table_hpm.cc hpm_Data.cc
table_hpm_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_hpm_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_P
table_P_SOURCES = table_P.cc P_Data.cc
table_P_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_P_CPPFLAGS = $(DATADIR)

bin_PROGRAMS += table_PI0
table_PI0_SOURCES = table_PI0.cc PI0_Data.cc
table_PI0_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
table_PI0_CPPFLAGS = $(DATADIR)	

bin_PROGRAMS      += mkExpData
mkExpData_SOURCES  = mkExpData.cc \
	PI0_Data.cc PIpm_Data.cc Kpm_Data.cc P_Data.cc hpm_Data.cc \
	EMC_Multi.cc \
	Belle.cc HERMES_multiplicities.cc\
	$(A1DATASRC)
mkExpData_LDADD    = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
mkExpData_CPPFLAGS = $(DATADIR)

bin_PROGRAMS      += expManip
expManip_SOURCES = expManip.cc
expManip_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
expManip_CPPFLAGS = $(DATADIR)	

bin_PROGRAMS      += e866
e866_SOURCES = e866.cc
e866_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
e866_CPPFLAGS = $(DATADIR)	

bin_PROGRAMS      += seaQuestPseudoData
seaQuestPseudoData_SOURCES = seaQuestPseudoData.cc
seaQuestPseudoData_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
seaQuestPseudoData_CPPFLAGS = $(DATADIR)	

bin_PROGRAMS      += hermesMultiplicities
hermesMultiplicities_SOURCES = hermesMultiplicities.cc HERMES_multiplicities.cc
hermesMultiplicities_LDADD   = libExpData.la $(LIB_TRANSFORM) $(LIB_UTILITY)
hermesMultiplicities_CPPFLAGS = $(DATADIR)	


# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS = 	\
ConfigDataSet.hh	\
DataParser.hh DataPoint.hh HERMES.hh	\
DataSet.hh	\
ExpData.hh	\
Kinematic.hh

install-data-local: @includedir@/ExpData
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/ExpData: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
