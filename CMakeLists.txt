cmake_minimum_required (VERSION 2.6)
project (Mellin)

find_package(PkgConfig)
pkg_check_modules(LIBXMLXX REQUIRED libxml++-2.6)

find_library(DB_CXX_LIBRARY NAMES db_cxx)

add_subdirectory( Utility )
add_subdirectory( Tranform )
