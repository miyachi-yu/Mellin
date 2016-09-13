add_library( unpolPDF Evolution.cc P.cc CTEQ6pdf.cc )

add_executable( main main.cc )
target_link_libraries( main
  unpolPDF
  Evolution
  PDFSET
  QCD
  Transform
  Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( bs15 bs15.cc )
target_link_libraries( bs15
  unpolPDF
  Evolution
  PDFSET
  QCD
  Transform
  Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY}  ${MATH_LIBRARY} )

#add_executable( testCteq6 testCteq6.cc )
#target_link_libraries( testCteq6
#  unpolPDF Evolution PDFSET QCD Transform PDFLIB Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( dbar_ubar dbar_ubar.cc )
target_link_libraries( dbar_ubar
  unpolPDF
  Evolution
  PDFSET
  QCD
  Transform
  Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

