
# -------------------------------------------------------------------
#
# Author: Toby D. Young (2010)
# Affiliation: Polish Academy of Sciences
#
# Copyright (C) 2010 The vecds authors
#
# This  program is  free  software: you  can  redistribute it  and/or
# modify  it under the  terms of  the GNU  General Public  License as
# published by the Free Software  Foundation, either version 3 of the
# License, or (at your option) any later version.
#  
# This program is distributed in the hope that it will be useful, but
# WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
# MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
# General Public License for more details.
#
# You should have  received a copy of the  GNU General Public License
# along      with      this      program.       If      not,      see
# <http://www.gnu.org/licenses/>.
#					 
# -------------------------------------------------------------------

# First some definitions about the vecds project:
TEMPLATE    = app
TARGET      = bin/vecds
TARGET_EXT  =

# Then output some information about the system we compile on:
message ( ------------------------------------------------------- )
#message ( Application: vecds-$$VERSION)
   message ( Making: Makefile on $$_DATE_)
   message ( On: $$QMAKE_HOST.name $$QMAKE_HOST.os \
             $$QMAKE_HOST.arch $$QMAKE_HOST.version)
   message ( Using: $$_PRO_FILE_)   
   message ( Qt version $$[QT_VERSION])
   message ( Qt installed in $$[QT_INSTALL_PREFIX])
   message ( Qt header files in $$[QT_INSTALL_HEADERS])
   message ( Qt libraries in $$[QT_INSTALL_LIBS])

# Shut up the compiler:
CONFIG += silent # debug_and_release

# This defines the way the documentation will be made:
dox.target   = doc
dox.commands = cd doc; doxygen doc.template;
dox.depends  = FORCE

# Additional control of the compilation process:
QMAKE_CXXFLAGS           += -Wno-ignored-qualifiers -fno-strict-aliasing
QMAKE_EXTRA_TARGETS      += dox
QMAKE_DISTCLEAN          += doc/html/* doc/latex/* source/qrc_vecds.cpp

# Additional includes and libraries needed to compile application:
INCLUDEPATH += .
INCLUDEPATH *= /usr/include/qwt5
LIBS        *= -lqwt -lgsl -lgslcblas -lm
LIBS        += -L/usr/local/lib -lGLU
#INCLUDEPATH *= /usr/include/qwt-qt4
#LIBS        *= -lqwt-qt4

# Add functionality of Qt needed by vecds
QT += opengl

# Location of header files and other resources needed for compilation:
HEADERS     = $$files(include/vecds/*.h)
SOURCES     = $$files(source/*.cpp)
RESOURCES   = $$files(include/vecds/*.qrc)
OBJECTS_DIR = lib
MOC_DIR     = moc 

# End of file
message ( -------------------------------------------------------)



