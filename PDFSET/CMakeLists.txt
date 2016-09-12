include_directories( ${LIBXMLXX_INCLUDE_DIRS} )
add_library( PDFSET
  BBParam.cc
  MRSTParam.cc  PdfMRST.cc  PdfMRSTXml.cc  ConfigMRST.cc
  KretzerParam.cc  KretzerFF.cc  KretzerFFXml.cc  ConfigKretzerFF.cc
  DSSFFParam.cc  DSSFF.cc  DSSFFXml.cc  ConfigDSSFF.cc
  DSSVParam.cc  PdfDSSV.cc  PdfDSSVXml.cc  ConfigDSSV.cc
  StatParam.cc  PdfStat.cc  PdfStatXml.cc  ConfigStat.cc
  Cteq6Param.cc  PdfCteq6.cc  PdfCteq6Xml.cc  ConfigCteq6.cc
  MSTWParam.cc )
