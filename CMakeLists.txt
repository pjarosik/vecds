######################################################################
# Copyright (c) 2015, the VECDS authors
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Author: Toby D. Young 2015.

######################################################################
# REQUIRE a reasonably recent version of CMake.
cmake_minimum_required (VERSION 2.8.12)
project (vecds)
set (TARGET vecds)
set (VECDS_INCLUDES ${CMAKE_SOURCE_DIR}/include)

include (FindPackageHandleStandardArgs)

######################################################################
# Qt specific - build meta-object code
# set (CMAKE_AUTOMOC ON)
# set (CMAKE_INCLUDE_CURRENT_DIRECTORY ON)

######################################################################
# find required system packages.
find_package (OpenSceneGraph 3.2.0 REQUIRED osg osgDB osgGA osgManipulator osgSim osgText osgUtil osgViewer osgWidget osgQt)
find_package (Qt4 4.8.6 REQUIRED QtGui QtOpenGL)

######################################################################
# Include the subdirectory containing header files.
include_directories (${VECDS_INCLUDES})
include_directories (${OPENSCENEGRAPH_INCLUDE_DIRS})

######################################################################
# Manually add the sources; TODO: Alternatively we could have done
# "add_subdirectory (source)" and done things in a CMakeFile.txt
# there.
set (SOURCES
  source/adds.cpp   
  source/calc.cpp     
  source/gener.cpp        
  source/internal.cpp  
  source/main.cpp        
  source/miscfunc.cpp  
  source/osgv.cpp     
  source/simplepad.cpp
  source/atoms.cpp  
  source/dialogs.cpp  
  source/helpbrowser.cpp  
  source/lattice.cpp   
  source/mainwindow.cpp  
  source/osgscene.cpp  
  source/picking.cpp
)

######################################################################
# Set up a bunch of targets for the meta-object code generator
set (VECDS_INCLUDES
#  include/adds.h   
#  include/calc.h     
#  include/gener.h        
#  include/internal.h  
#  include/miscfunc.h  
  include/osgv.h     
  include/simplepad.h
#  include/atoms.h  
  include/dialogs.h  
  include/helpbrowser.h  
#  include/lattice.h   
  include/mainwindow.h  
  include/osgscene.h  
#  include/picking.h
)
qt4_wrap_cpp (VECDS_INCLUDES_MOC ${VECDS_INCLUDES})


######################################################################
# Indicate the entry point of the excecutable and libraries to include
# during the link process.
add_executable(${TARGET} ${SOURCES} ${VECDS_INCLUDES_MOC})
target_link_libraries(${TARGET} ${OPENSCENEGRAPH_LIBRARIES} Qt4::QtGui Qt4::QtOpenGL)
