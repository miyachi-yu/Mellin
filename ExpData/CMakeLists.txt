include_directories( ${LIBXMLXX_INCLUDE_DIRS} )

add_definitions( -DDATA_DIR_PATH="${PROJECT_SOURCE_DIR}/Data" )

add_library( ExpData
  DataParser.cc HERMES.cc ExpData.cc ConfigDataSet.cc DataSet.cc DataPoint.cc Kinematic.cc )

add_executable( table_g1
  table_g1.cc hermes.cc e155.cc e154.cc e143.cc e142.cc smc.cc emc.cc compass.cc )
target_link_libraries( table_g1
  StrFncParam ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_fragXsec
  table_fragXsec.cc hadron.cc HERMES_multiplicities.cc EMC_Multi.cc )
target_link_libraries( table_fragXsec
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_kretzer
  table_kretzer.cc hadron.cc HERMES_MultiZ.cc HERMES_MultiQ2.cc EMC_Multi.cc )
target_link_libraries( table_kretzer
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_A1h table_A1h.cc COMPASS_PLB680_217.cc )
target_link_libraries( table_A1h
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_A1
  table_A1.cc
  A1_Data.cc E143_A1.cc E155_A1.cc EMC_A1.cc HERMES_A1.cc
  SMC_PhysRevD58_A1.cc COMPASS_PLB680_217.cc COMPASS_PLB690_466.cc )
target_link_libraries( table_A1
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_Multi
  table_Multi.cc HERMES_MultiZ.cc HERMES_MultiQ2.cc EMC_Multi.cc )
target_link_libraries( table_Multi
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_Multi_K 
  table_Multi_K.cc HERMES_MultiZ.cc HERMES_MultiQ2.cc EMC_Multi.cc )
target_link_libraries( table_Multi_K
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_Xsec table_Xsec.cc hadron.cc )
target_link_libraries( table_Xsec
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( changeDataNorm changeDataNorm.cc )
target_link_libraries( changeDataNorm
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_PIpm table_PIpm.cc PIpm_Data.cc )
target_link_libraries( table_PIpm
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_Kpm table_Kpm.cc Kpm_Data.cc )
target_link_libraries( table_Kpm
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_hpm table_hpm.cc hpm_Data.cc )
target_link_libraries( table_hpm
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_P table_P.cc P_Data.cc )
target_link_libraries( table_P
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( table_PI0 table_PI0.cc PI0_Data.cc )
target_link_libraries( table_PI0
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( mkExpData
  mkExpData.cc PI0_Data.cc PIpm_Data.cc Kpm_Data.cc P_Data.cc hpm_Data.cc
  EMC_Multi.cc Belle.cc HERMES_multiplicities.cc
  A1_Data.cc E143_A1.cc E155_A1.cc EMC_A1.cc HERMES_A1.cc
  SMC_PhysRevD58_A1.cc COMPASS_PLB680_217.cc COMPASS_PLB690_466.cc )
target_link_libraries( mkExpData
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( expManip expManip.cc )
target_link_libraries( expManip
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( e866 e866.cc )
target_link_libraries( e866
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( seaQuestPseudoData seaQuestPseudoData.cc )
target_link_libraries( seaQuestPseudoData
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

add_executable( hermesMultiplicities hermesMultiplicities.cc HERMES_multiplicities.cc )
target_link_libraries( hermesMultiplicities
  ExpData Transform Utility ${LIBXMLXX_LIBRARIES} ${DB_CXX_LIBRARY} )

install( TARGETS ExpData  DESTINATION lib )
install( TARGETS expManip DESTINATION bin )

