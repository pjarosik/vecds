## -------------------------------------------------------------------
## Copyright 2012 vecds authors. All rights reserved.
##
## Author: Toby D. Young
## -------------------------------------------------------------------

cmake_minimum_required (VERSION 2.8)

###################################################
message (STATUS "Preconfiguring vecds")
###################################################
project (VECDS)
set (VECDS_NAME               "vecds")
set (VECDS_PACKAGE_NAME       "namespace vecds")
set (VECDS_PACKAGE_BUGREPORT  "vecds-users@lists.sourceforge.net")
set (VECDS_VERSION_MAJOR      "1")
set (VECDS_VERSION_MINOR      "0")
set (VECDS_VERSION_TWEAK      "0")
set (VECDS_VERSION_PATCH      "dev")
set (VECDS_VERSION            ${VECDS_VERSION_MAJOR}.${VECDS_VERSION_MINOR}.${VECDS_VERSION_TWEAK}.${VECDS_VERSION_PATCH})
set (VECDS_PACKAGE_STRING     ${VECDS_PACKAGE_NAME}\\${VECDS_VERSION})
set (VECDS_PATH               ${CMAKE_SOURCE_DIR})
set (VECDS_DOCS               ${VECDS_PATH}/doc)
set (VECDS_INCLUDES           ${VECDS_PATH}/include)
set (VECDS_LIBRARIES          ${VECDS_PATH}/lib)
set (VECDS_SHARES             ${VECDS_PATH}/share)
set (LIBRARY_OUTPUT_PATH      ${VECDS_LIBRARIES})
set (CMAKE_MODULE_PATH        ${VECDS_PATH}/CMakeModules)
set (CMAKE_INSTALL_PREFIX     ${VECDS_LIBRARIES})
set (CMAKE_CXX_FLAGS_DEBUG    "-O0 -g -DDEBUG -Wall")
set (CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG") 
set (CMAKE_VERBOSE_MAKEFILE   ON)

## Debug messages...
message (STATUS "The CMake build type is: ${CMAKE_BUILD_TYPE}")
message (STATUS "The CMake host system is: ${CMAKE_SYSTEM_NAME} (\"${CMAKE_SYSTEM}\")")
message (STATUS "The CMake generator identification is: ${CMAKE_GENERATOR}")
message (STATUS "The CMake library output path is: ${LIBRARY_OUTPUT_PATH}")
message (STATUS "The CMake module path is: ${CMAKE_MODULE_PATH}")
message (STATUS "Found CMake: ${CMAKE_COMMAND} (found version \"${CMAKE_VERSION}\")")


##############################################
message (STATUS "Configuring system packages")
##############################################
find_package (Curses)
find_package (Doxygen)
find_package (MPI)
find_package (Perl)
find_package (PNG)
find_package (PythonLibs)
find_package (Threads)
find_package (ZLIB)

