add_library( Fragmentation Evolution.cc P.cc )

add_executable( fragmentation main.cc )
target_link_libraries( fragmentation
  Fragmentation
  Evolution
  PDFSET
  QCD
  Transform
  Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( printFF printFF.cc )
target_link_libraries( printFF
  Fragmentation
  Evolution
  PDFSET
  QCD
  Transform
  Utility
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

install( TARGETS Fragmentation DESTINATION lib )
