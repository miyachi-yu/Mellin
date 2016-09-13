add_library( unpolDIS StrFunc.cc XsecComp.cc CoeffFunc.cc WC.cc )

add_executable( unpoldis main.cc )
target_link_libraries( unpoldis
  unpolDIS
  Xsec
  ExpData
  unpolPDF
  Evolution
  PDFSET
  QCD
  Transform
  Utility
  PDFLIB
  ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} ${FORTRAN_LIBRARY} )
  
  
