set( CMAKE_CXX_FLAGS "-Df2cFortran ${CMAKE_CXX_FLAGS}" )

add_library( PDFLIB
  CTEQ6.cc GRSV2000.cc Package.cc Cteq6Pdf-2004.f grsv2000pdf_g1.f
  LSS.cc LSS2006pdf_g1.f LSS2001.f  LSS2005pdf_g1.f dfint.f
  AAC.cc aac08.f BB.cc bb_ppdf.f dlib.f kkp.f pkhff.f grille_had_charged.f
  polin2.f polint.f locate.f FF.cc MRST2001.cc mrst2001E.f
  mstwpdf.cc MSTW2008.cc hknsff07.f HKNS07.cc )


