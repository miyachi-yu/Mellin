add_library( StrFncParam
  ALLM91.cc  F1Param.cc      F2dSMC.cc  PhotoAbsorbXsec.cc  RParam.cc
  ALLM97.cc  F2Param.cc      F2nSMC.cc  R1990.cc
  DoLara.cc  F2ParamBase.cc  F2pSMC.cc  R1998.cc )

install( TARGETS StrFncParam DESTINATION lib )
