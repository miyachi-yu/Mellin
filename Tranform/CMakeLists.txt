include_directories( ${PROJECT_SOURCE_DIR} )
add_library( Transform
  RealFunction.cc ComplexFunction.cc
  CBeta.cc	ComplexBeta.cc
  CGamma.cc	CLogGamma.cc
  CPolyGamma.cc	Li2.cc	Li2Int.cc	Li2Int2.cc
  HyperGeomInt.cc	HyperGeom.cc
  ConfluentHG.cc	ConfluentHG2.cc
  GeneralHarmonicTilde.cc	GeneralHarmonicPrime.cc
  GeneralHarmonic.cc
  CZeta.cc	CPolyLog.cc
  Binomial.cc	Eulerian.cc
  IntegFunc.cc
  Mellin.cc	MellinIntegrand.cc
  PartialInverseMellinIntegrand.cc
  IntegPath.cc	SteepestDescent.cc	StraightLine.cc
  InverseMellin.cc	InverseMellinIntegrand.cc
  DInverseMellin.cc
  DInverseMellinIntegInteg.cc
  DInverseMellinIntegrand.cc
  GridIntegration.cc	TrapInt.cc	LegQuadrature.cc
  RTransform.cc
  Trapezoid.cc Rectangle.cc RungeKutta.cc
  CKey.cc DataCache.cc DataCache2.cc
  ErrorFunction.cc Laguerre.cc
  PartialInteg.cc
  WhittakerW.cc LaguerreQuadrature.cc
  Romberg.cc PolyInt.cc
  DbKey.cc DbKey2.cc
  DataCacheInteg.cc DataCacheInteg2.cc MCInt.cc Pvalue.cc )

add_executable( cbeta cbeta.cc )
target_link_libraries( cbeta
  Transform Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} ${MATH_LIBRARY} )

add_executable( gamma gamma.cc )
target_link_libraries( gamma
  Transform Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} ${MATH_LIBRARY} )

install( TARGETS Transform DESTINATION lib )
