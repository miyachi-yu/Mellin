add_library( polPDF Evolution.cc P.cc )
add_executable( polpdf main.cc )
target_link_libraries( polpdf
  polPDF
  Evolution
  PDFSET
  QCD
  Transform
  Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )
