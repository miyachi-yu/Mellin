############################################################
#
#
#
#
############################################################
cmake_minimum_required( VERSION 2.6 )

###  Project name
project( Mellin )

###  Required external libraries
find_package( PkgConfig )
pkg_check_modules( LIBXMLXX REQUIRED libxml++-2.6 )

find_library( DB_CXX_LIBRARY  NAMES db_cxx )
find_library( MATH_LIBRARY    NAMES m )
find_library( FORTRAN_LIBRARY NAMES gfortran )

###  Installation directory
#-  Installation prefix
set( CMAKE_INSTALL_PREFIX "$ENV{HOME}/mellin" )

file( GLOB XMLFILES "${PROJECT_SOURCE_DIR}/config/*.xml" )
install( FILES ${XMLFILES} DESTINATION config )

###  compiler options
include_directories( ${PROJECT_SOURCE_DIR} )
set( BUILD_SHARED_LIBS ON )

###  subdirectories
add_subdirectory( Utility )
add_subdirectory( Tranform )
add_subdirectory( StrFncParam )
add_subdirectory( QCD )
add_subdirectory( PDFSET )
add_subdirectory( PDFLIB )
add_subdirectory( ExpData )
add_subdirectory( Evolution )
add_subdirectory( unpolPDF )
add_subdirectory( polPDF )
add_subdirectory( Fragmentation )
add_subdirectory( polFrag )
add_subdirectory( Xsec )
add_subdirectory( unpolDIS )
add_subdirectory( unpolDISXsec )
#add_subdirectory( unpolSIDIS )
#add_subdirectory( unpolSIDISXsec )
#add_subdirectory( polDIS )
#add_subdirectory( polSIDIS )
#add_subdirectory( FragXsec_T )
#add_subdirectory( ErrorCalc )
#add_subdirectory( IntegXsec )
#add_subdirectory( IntegFragXsec )
#add_subdirectory( IntegUnpolDIS )
#add_subdirectory( IntegUnpolSIDIS )
#add_subdirectory( IntegPolDIS )
#add_subdirectory( IntegPolSIDIS )
#add_subdirectory( IntegUnpolDrellYan )
#add_subdirectory( IntegPolDrellYan )
#add_subdirectory( IntegUnpolVBoson )
#add_subdirectory( IntegPolVBoson )
#add_subdirectory( IntegSPolVBoson )
#add_subdirectory( Fitting )
#add_subdirectory( browser )
#add_subdirectory( plotter )
#add_subdirectory( script )
#add_subdirectory( DrawTool )
