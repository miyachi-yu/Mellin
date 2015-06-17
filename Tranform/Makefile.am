# ----------------------------------------------------------------------- #
#                           common setting                                #
# ----------------------------------------------------------------------- #
LIB_UTILITY = ../Utility/libUtility.la @LIBXML_LIBS@

# ----------------------------------------------------------------------- #
#                          libTransform                                   #
# ----------------------------------------------------------------------- #
lib_LTLIBRARIES = libTransform.la
libTransform_la_SOURCES = 					\
	RealFunction.cc ComplexFunction.cc 			\
	CBeta.cc	ComplexBeta.cc 				\
	CGamma.cc	CLogGamma.cc 				\
	CPolyGamma.cc	Li2.cc	Li2Int.cc	Li2Int2.cc	\
	HyperGeomInt.cc	HyperGeom.cc 				\
	ConfluentHG.cc	ConfluentHG2.cc				\
	GeneralHarmonicTilde.cc	GeneralHarmonicPrime.cc 	\
	GeneralHarmonic.cc 					\
	IntegFunc.cc 						\
	Mellin.cc	MellinIntegrand.cc			\
	PartialInverseMellinIntegrand.cc 			\
	IntegPath.cc	SteepestDescent.cc	StraightLine.cc \
	InverseMellin.cc	InverseMellinIntegrand.cc 	\
	DInverseMellin.cc 					\
	DInverseMellinIntegInteg.cc				\
	DInverseMellinIntegrand.cc 				\
	GridIntegration.cc	TrapInt.cc	LegQuadrature.cc \
	Trapezoid.cc Rectangle.cc RungeKutta.cc \
	CKey.cc DataCache.cc DataCache2.cc \
	ErrorFunction.cc Laguerre.cc \
	PartialInteg.cc \
	WhittakerW.cc LaguerreQuadrature.cc \
	Romberg.cc PolyInt.cc \
	DbKey.cc DbKey2.cc \
	DataCacheInteg.cc DataCacheInteg2.cc MCInt.cc Pvalue.cc


# ----------------------------------------------------------------------- #
#                           Test programs                                 #
# ----------------------------------------------------------------------- #
noinst_PROGRAMS = \
cbeta	checkPartialInteg	confluent	doubleInversion	gamma	\
hyperGeom	lagQuad	laguerre	li2	matrix	minimum		\
polygamma	polyint	runge	sfunc	pvalue

cbeta_SOURCES = cbeta.cc
cbeta_LDADD   = libTransform.la  $(LIB_UTILITY)

checkPartialInteg_SOURCES = checkPartialInteg.cc
checkPartialInteg_LDADD   = libTransform.la $(LIB_UTILITY)

confluent_SOURCES = confluent.cc
confluent_LDADD	  = libTransform.la  $(LIB_UTILITY)

doubleInversion_SOURCES = doubleInversion.cc
doubleInversion_LDADD   = libTransform.la  $(LIB_UTILITY)

gamma_SOURCES = gamma.cc
gamma_LDADD   = libTransform.la $(LIB_UTILITY)

hyperGeom_SOURCES = hyperGeom.cc
hyperGeom_LDADD   = libTransform.la  $(LIB_UTILITY)

lagQuad_SOURCES = lagQuad.cc
lagQuad_LDADD   = libTransform.la  $(LIB_UTILITY)

laguerre_SOURCES = laguerre.cc
laguerre_LDADD   = libTransform.la  $(LIB_UTILITY)

li2_SOURCES = li2.cc
li2_LDADD   = libTransform.la $(LIB_UTILITY)

matrix_SOURCES = matrix.cc
matrix_LDADD   = libTransform.la $(LIB_UTILITY)

minimum_SOURCES = minimum.cc
minimum_LDADD   = libTransform.la $(LIB_UTILITY)

polygamma_SOURCES = polygamma.cc
polygamma_LDADD   = libTransform.la $(LIB_UTILITY)

polyint_SOURCES = polyint.cc
polyint_LDADD   = libTransform.la  $(LIB_UTILITY)

runge_SOURCES = runge.cc
runge_LDADD   = libTransform.la $(LIB_UTILITY)

sfunc_SOURCES = sfunc.cc
sfunc_LDADD   = libTransform.la $(LIB_UTILITY)

pvalue_SOURCES = pvalue.cc
pvalue_LDADD   = libTransform.la  $(LIB_UTILITY)

noinst_PROGRAMS += mcint
mcint_SOURCES = mcint.cc
mcint_LDADD   = libTransform.la $(LIB_UTILITY)


# ----------------------------------------------------------------------- #
#                                HEADERS                                  #
# ----------------------------------------------------------------------- #
MYHEADERS =	\
CBeta.hh	\
CGamma.hh	\
CKey.hh	\
CLogGamma.hh	\
CPolyGamma.hh	\
ComplexBeta.hh	\
ComplexFunction.hh	\
ConfluentHG.hh	\
ConfluentHG2.hh	\
DInverseMellin.hh	\
DInverseMellinIntegInteg.hh	\
DInverseMellinIntegrand.hh	\
DataCache.hh	\
DataCache2.hh	\
ErrorFunction.hh	\
GeneralHarmonic.hh	\
GeneralHarmonicPrime.hh	\
GeneralHarmonicTilde.hh	\
GridIntegration.hh	\
HyperGeom.hh	\
HyperGeomInt.hh	\
IntegFunc.hh	\
IntegPath.hh	\
Integration.hh	\
InverseMellin.hh	\
InverseMellinIntegrand.hh	\
Laguerre.hh	\
LaguerreQuadrature.hh	\
LegQuadrature.hh	\
Li2.hh	\
Li2Int.hh	\
Li2Int2.hh	\
Matrix.hh	\
Mellin.hh	\
MellinIntegrand.hh	\
PartialInteg.hh	\
PartialInverseMellinIntegrand.hh	\
PolyInt.hh	\
RealFunction.hh	\
Rectangle.hh	\
Romberg.hh	\
RungeKutta.hh	\
SteepestDescent.hh	\
StraightLine.hh	\
Transformer.hh	\
TrapInt.hh	\
Trapezoid.hh	\
WhittakerW.hh \
RealCompFunction.hh \
DbKey.hh \
DbKey2.hh \
DataCacheInteg.hh \
DataCacheInteg2.hh \
Pvalue.hh

install-data-local: @includedir@/Tranform
	$(INSTALL_DATA) $(MYHEADERS) $<

@includedir@/Tranform: @includedir@
	$(INSTALL) --directory $@

@includedir@:
	$(INSTALL) --directory $@
