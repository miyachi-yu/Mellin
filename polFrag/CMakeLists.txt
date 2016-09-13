add_library( polFrag Evolution.cc P.cc )

add_executable( polFF main.cc )
target_link_libraries( polFF
  polFrag
  Evolution
  PDFSET
  QCD
  Transform
  Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

