include_directories( ${LIBXMLXX_INCLUDE_DIRS} )

add_library( Utility 
  Arguments.cc Config.cc Timer.cc LookupTableCtl.cc Thread.cc )

add_executable( testArguments testArguments.cc )
target_link_libraries( testArguments Utility ${LIBXMLXX_LIBRARIES} )

install( TARGETS Utility DESTINATION lib )
