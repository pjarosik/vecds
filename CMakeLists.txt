## -------------------------------------------------------------------
## Copyright 2012 vecds authors. All rights reserved.
##
## Author: Toby D. Young
## -------------------------------------------------------------------

cmake_minimum_required (VERSION 2.8)

#######################################
message (STATUS "Preconfiguring vecds")
#######################################
project (VECDS)
set (VECDS_NAME               "vecds")
set (VECDS_PACKAGE_NAME       "visualisation of crystal defects")
set (VECDS_PACKAGE_BUGREPORT  "vecds-users@lists.sourceforge.net")
set (VECDS_VERSION_MAJOR      "1")
set (VECDS_VERSION_MINOR      "0")
set (VECDS_VERSION_TWEAK      "0")
set (VECDS_VERSION_PATCH      "dev")
set (VECDS_VERSION            ${VECDS_VERSION_MAJOR}.${VECDS_VERSION_MINOR}.${VECDS_VERSION_TWEAK}.${VECDS_VERSION_PATCH})
set (VECDS_PACKAGE_STRING     ${VECDS_PACKAGE_NAME}\\${VECDS_VERSION})
set (VECDS_PATH               ${CMAKE_SOURCE_DIR})
set (VECDS_DOCS               ${VECDS_PATH}/doc)
set (VECDS_INCLUDES           ${VECDS_PATH}/include/vecds)
set (VECDS_LIBRARIES          ${VECDS_PATH}/lib)
set (VECDS_SHARES             ${VECDS_PATH}/share)
set (VECDS_BIN                ${VECDS_PATH}/bin)
set (LIBRARY_OUTPUT_PATH      ${VECDS_LIBRARIES})
set (CMAKE_MODULE_PATH        ${VECDS_PATH}/CMakeModules)
set (CMAKE_INSTALL_PREFIX     ${VECDS_LIBRARIES})
set (CMAKE_CXX_FLAGS_DEBUG    "-O0 -g -DDEBUG -Wall")
set (CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG") 

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
include (FindPackageHandleStandardArgs)
find_package (Curses)
##############################################
#find_package (GSL)
find_library (GSL_LIBRARIES
  NAMES gsl
  PATHS 
  /usr/local/lib 
  /usr/lib
)

find_package_handle_standard_args ("GSL includes" REQUIRED_VARS GSL_LIBRARIES)

find_path (GSL_INCLUDES
  ## Look for gsl version and math header.
  NAMES gsl_version.h gsl_math.h
  PATHS
  /usr/local/include/gsl
  /usr/include/gsl
)

find_package_handle_standard_args ("GSL includes" REQUIRED_VARS GSL_INCLUDES)

find_library (GSL_CBLAS_LIBRARIES
  NAMES gslcblas
  PATHS 
  /usr/local/lib 
  /usr/lib
)

find_package_handle_standard_args ("GSL C Blas includes" REQUIRED_VARS GSL_CBLAS_LIBRARIES)

find_path (GSL_CBLAS_INCLUDES
  ## Look for gsl version and math header.
  NAMES gsl_cblas.h
  PATHS
  /usr/local/include/gsl
  /usr/include/gsl
)

find_package_handle_standard_args ("GSL C Blas includes" REQUIRED_VARS GSL_CBLAS_INCLUDES)

find_package (OpenGL REQUIRED)
find_package_handle_standard_args ("OpenGL libraries" REQUIRED_VARS OPENGL_LIBRARIES)
find_package_handle_standard_args ("OpenGL includes"  REQUIRED_VARS OPENGL_INCLUDE_DIR)

##############################################
find_package (Qt4 REQUIRED COMPONENTS QtCore QtOpenGL)
set (QT_USE_QTCORE TRUE)
set (QT_USE_QTOPENGL TRUE)
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})
##############################################
#find_package (Qwt)

find_library (QWT_LIBRARIES
  NAMES qwt6 qwt5 qwt
  PATHS 
  /usr/local/lib 
  /usr/lib
) 

find_package_handle_standard_args ("Qwt libraries" REQUIRED_VARS QWT_LIBRARIES)

find_path (QWT_INCLUDES
  ## Look for qwt header.
  NAMES qwt.h
  PATHS
  /usr/local/include/qwt-qt4
  /usr/local/include/qwt6
  /usr/local/include/qwt5
  /usr/local/include/qwt
  /usr/include/qwt-qt4
  /usr/include/qwt6
  /usr/include/qwt5
  /usr/include/qwt
)

find_package_handle_standard_args ("Qwt includes"  REQUIRED_VARS QWT_INCLUDES)
##############################################
#find_package (QwtPlot3d)

find_library (QWTPLOT3D_LIBRARIES
  NAMES qwtplot3d qwtplot3d-qt4
  PATHS 
  /usr/local/lib 
  /usr/lib
) 

find_package_handle_standard_args ("Qwt3d libraries" REQUIRED_VARS QWTPLOT3D_LIBRARIES)

find_path (QWTPLOT3D_INCLUDES
  ## Look for qwt data types.
  NAMES qwt3d_types.h 
  PATHS
  /usr/local/include/qwtplot3d-qt4
  /usr/local/include/qwtplot3d
  /usr/include/qwtplot3d-qt4
  /usr/include/qwtplot3d
)

find_package_handle_standard_args ("Qwt3d includes"  REQUIRED_VARS QWTPLOT3D_INCLUDES)

##############################################
## Note: It is a complete mystery as to why we need to include the Qwt
## Polar libraries. It seems something "Extra" gets triggered in the
## Qwt on some distributions of Linux since, without it installed, we
## get error messages of the type:
##
## fatal error: qwt_scale_engine.h: no such file or directory
##
## So there...
find_library (QWTPOLAR_LIBRARIES
  NAMES qwtpolar 
  PATHS 
  /usr/local/lib 
  /usr/lib
) 

find_package_handle_standard_args ("QwtPolar libraries" REQUIRED_VARS QWTPOLAR_LIBRARIES)

find_path (QWTPOLAR_INCLUDES
  ## Look for qwt polar header.
  NAMES qwt_polar.h
  PATHS
  /usr/local/include/qwtpolar
  /usr/include/qwtpolar
)

find_package_handle_standard_args ("QwtPolar includes"  REQUIRED_VARS QWTPOLAR_INCLUDES)
##############################################

#########################################
message (STATUS "Configuring build type")
#########################################
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  ############################################
  if (MSVC OR MSVC_IDE OR MSVC60 OR MSVC70 OR MSVC71 OR MSVC80 OR MSVC90 OR MSVC10)
    ## Microsoft Visual Compilers go out on limb to not document their
    ## compiler flags in an understandable way. Accordingly, if any of
    ## those compilers show up, do nothing and hope for the best.
    add_definitions ()
  else ()
    ## Random guesses...
    add_definitions ("-march=native")
    
    ## Optimizations (no warnings!):
    add_definitions ("-fdata-sections -ffunction-sections -findirect-inlining -fforce-addr -fstrict-aliasing -ftemplate-depth-128 -funroll-loops")
  endif ()
  
  ############################################
else ()
  if (MSVC OR MSVC_IDE OR MSVC60 OR MSVC70 OR MSVC71 OR MSVC80 OR MSVC90 OR MSVC10)
    ## See note above...
    add_definitions ()
  else ()
    ## Warnings:
    add_definitions ("-Wextra -Wpointer-arith -Wsign-compare -Wswitch -Wwrite-strings")
     
    ## Optimizations (basically none):
    add_definitions ("-ftemplate-depth-128")
  endif ()
endif ()



################################################
message (STATUS "Configuring build environment")
################################################
include_directories (
  ${VECDS_INCLUDES}
  ${GSL_INCLUDES}
  ${GSBLASL_INCLUDES}
  ${QT_INCLUDES}
  ${QWT_INCLUDES}
  ${QWTPLOT3D_INCLUDES}
  )

set (vecds_sources
  ${VECDS_PATH}/source/arcball
  ${VECDS_PATH}/source/dialogs
  ${VECDS_PATH}/source/help_browser
  ${VECDS_PATH}/source/internal
  ${VECDS_PATH}/source/main
  ${VECDS_PATH}/source/main_viewer
  ${VECDS_PATH}/source/main_window
  )

set (vecds_headers
  ${VECDS_INCLUDES}/arcball.h
  ${VECDS_INCLUDES}/dialogs.h
  ${VECDS_INCLUDES}/help_browser.h
  ${VECDS_INCLUDES}/internal.h
  ##  ${VECDS_INCLUDES}/main.h
  ${VECDS_INCLUDES}/main_viewer.h
  ${VECDS_INCLUDES}/main_window.h
)


qt4_wrap_cpp (vecds_headers_moc ${vecds_headers})
add_executable (VECDS ${vecds_sources} ${vecds_headers_moc})

## We're not actually using the resources file at the moment, but
## if/when we do, this is how we would do it:
##
## set (vecds_resources ${VECDS_INCLUDES}/vecds.qrc)
## qt4_add_resources (vecds_resources_rcc ${vecds_resources})

target_link_libraries (VECDS 
  ${GSL_LIBRARIES} 
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES} 
  ${QWT_LIBRARIES}
  ${QWTPLOT3D_LIBRARIES}
  )