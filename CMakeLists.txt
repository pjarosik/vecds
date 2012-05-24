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
## Project definitions
set (VECDS_NAME               "vecds")
set (VECDS_PACKAGE_NAME       "visualisation of crystal defects")
set (VECDS_PACKAGE_BUGREPORT  "vecds-users@lists.sourceforge.net")
set (VECDS_VERSION_MAJOR      "0")
set (VECDS_VERSION_MINOR      "4")
set (VECDS_VERSION_TWEAK      "2")
set (VECDS_VERSION_PATCH      "dev")
set (VECDS_VERSION            ${VECDS_VERSION_MAJOR}.${VECDS_VERSION_MINOR}.${VECDS_VERSION_TWEAK}.${VECDS_VERSION_PATCH})
set (VECDS_PACKAGE_STRING     ${VECDS_PACKAGE_NAME}\\${VECDS_VERSION})
## Base structure
set (VECDS_PATH               ${CMAKE_SOURCE_DIR})
## Surface structure
set (VECDS_BIN                ${VECDS_PATH}/bin)
set (VECDS_INCLUDES           ${VECDS_PATH}/include)
set (VECDS_SHARES             ${VECDS_PATH}/share)
## Deep structure
set (VECDS_DOCS               ${VECDS_SHARES}/vecds/doc)
set (VECDS_SCRIPTS            ${VECDS_SHARES}/vecds/scripts)
set (VECDS_INTERNALS          ${VECDS_SHARES}/vecds/internals)

## set (CMAKE_INSTALL_PREFIX     ${VECDS_BIN})
set (CMAKE_CXX_FLAGS_DEBUG    "-O0 -g -DDEBUG -Wall")
set (CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")  
if (CMAKE_BUILD_TYPE STREQUAL "")
  set (CMAKE_BUILD_TYPE "Unknown")
endif ()

## Debug messages...
message (STATUS "Found ${VECDS_NAME}: ${CMAKE_BUILD_TYPE} build type")
message (STATUS "Found ${VECDS_NAME}: ${VECDS_NAME} (found version \"${VECDS_VERSION}\")")
message (STATUS "The CMake host system is: ${CMAKE_SYSTEM_NAME} (\"${CMAKE_SYSTEM}\")")
message (STATUS "The CMake generator identification is: ${CMAKE_GENERATOR}")
message (STATUS "The CMake module path is: ${CMAKE_MODULE_PATH}")
message (STATUS "Found CMake: ${CMAKE_COMMAND} (found version \"${CMAKE_VERSION}\")")

##############################################
message (STATUS "Configuring system packages")
##############################################
include (FindPackageHandleStandardArgs)
find_package (Doxygen)
if (DOXYGEN_FOUND)
  add_definitions ("-DHAVE_DOXYGEN_H") 
endif ()

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

## Create a config.h file (-DHAVE_CONFIG_H)
configure_file (${VECDS_INCLUDES}/vecds/config.h.in  ${VECDS_INCLUDES}/vecds/config.h)

## Create a doxygen template file for producing user manual
## documentation (-DHAVE_DOXYGEN)
configure_file (${VECDS_SCRIPTS}/doxygen.template.in ${VECDS_SCRIPTS}/doxygen.template)

include_directories (
  ${VECDS_INCLUDES}
  ${GSL_INCLUDES}
  ${GSBLASL_INCLUDES}
  ${QT_INCLUDES}
  ${QWT_INCLUDES}
  ${QWTPLOT3D_INCLUDES}
  )

set (vecds_sources
  ${VECDS_PATH}/source/dialogs/monolog_base
  ${VECDS_PATH}/source/dialogs/dialog_base
  ${VECDS_PATH}/source/dialogs/doc_browser
  ${VECDS_PATH}/source/dialogs/help_browser
  ${VECDS_PATH}/source/dialogs/question
  ${VECDS_PATH}/source/dialogs/question_form
  ##
  ${VECDS_PATH}/source/arc_ball
  ${VECDS_PATH}/source/internal
  ${VECDS_PATH}/source/main
  ${VECDS_PATH}/source/main_viewer
  ${VECDS_PATH}/source/main_window
  )

## Note, header files *only* determine which files will be passed over
## for moc generated code; thus, we do not need them all (especially,
## we should never need "main.*")
set (vecds_headers
  ${VECDS_INCLUDES}/vecds/dialogs/monolog_base.h
  ${VECDS_INCLUDES}/vecds/dialogs/dialog_base.h
  ${VECDS_INCLUDES}/vecds/dialogs/doc_browser.h
  ${VECDS_INCLUDES}/vecds/dialogs/help_browser.h
  ${VECDS_INCLUDES}/vecds/dialogs/question.h
  ${VECDS_INCLUDES}/vecds/dialogs/question_form.h
  ##
  ${VECDS_INCLUDES}/vecds/arc_ball.h
  ${VECDS_INCLUDES}/vecds/internal.h
  ${VECDS_INCLUDES}/vecds/main_viewer.h
  ${VECDS_INCLUDES}/vecds/main_window.h
)

qt4_wrap_cpp (vecds_headers_moc ${vecds_headers})
add_executable (${VECDS_NAME} ${vecds_sources} ${vecds_headers_moc})

## We're not actually going the resources files at the moment, but
## if/when we do, this is how we would do it:
##
## set (vecds_resources ${VECDS_INCLUDES}/vecds.qrc)
## qt4_add_resources (vecds_resources_rcc ${vecds_resources})

target_link_libraries (${VECDS_NAME}
  ${GSL_LIBRARIES} 
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES} 
  ${QWT_LIBRARIES}
  ${QWTPLOT3D_LIBRARIES}
  )

## Dcoumentation via doxygen.
if (DOXYGEN_FOUND)
  add_custom_target (doc
    ${DOXYGEN}
    ${VECDS_SCRIPTS}/doxygen.template
    DEPENDS ${VECDS_SCRIPTS}
    DEPENDS ${VECDS_SCRIPTS}/doxygen.template
    DEPENDS ${VECDS_DOCS}
    )
else ()
  message (STATUS "Doxygen not found - documentation will not be built")
endif ()

set (EXECUTABLE_OUTPUT_PATH ${VECDS_BIN})