include_directories( ${LIBXMLXX_INCLUDE_DIRS} )
add_library( QCD
  alpha.cc
  Flavor.cc
  Scheme.cc
  Parameter.cc
  PdfBase.cc
  PdfParametrization.cc
  ConfigPdfBase.cc
  ConfigFFbase.cc
  PdfSimpleXml.cc
  PdfSimple.cc
  FFSimple.cc
  FFSimpleXml.cc
  FFDisFavor.cc
  PdfErrMatrix.cc
  HadronParam.cc )

add_executable( pdfManip pdfManip.cc )
target_link_libraries( pdfManip QCD Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )
