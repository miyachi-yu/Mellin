add_library( unpolDISXsec
  StrFunc.cc XsecComp.cc CoeffFunc.cc
  WC.cc WC2.cc xXsec.cc IMIntegrand.cc StrFuncSub.cc )

add_executable( unpoldisxsec main.cc )
target_link_libraries( unpoldisxsec
  unpolDISXsec
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
  
install( TARGETS unpolDISXsec DESTINATION lib )
