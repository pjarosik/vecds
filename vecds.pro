QT       += core gui
QT       += opengl widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ./bin/vecds
TEMPLATE = app

message($$PWD)



HEADERS      = $$files(./include/*.h)
SOURCES      = $$files(./source/*.cpp)
RESOURCES    = ./source/vecds.qrc

OBJECTS_DIR  = tmp
MOC_DIR      = tmp
#RCC_DIR = tmp
#UI_DIR = tmp

CONFIG += qt warn_on

QMAKE_LFAGS += -O2

win32 {
  OSG_HOME = C:/Opt/lib/OpenSceneGraph-3.01-vs-qt-x86
  INCLUDEPATH += $$OSG_HOME/include
  LIBS += /LIBPATH:$$OSG_HOME/lib \
    osgQt.lib osgWidget.lib osgViewer.lib osgText.lib osgGA.lib osgDB.lib \
    osgUtil.lib osg.lib OpenThreads.lib
}
unix:LIBS += -losgQt -losgWidget -losgViewer -losgManipulator -losgText -losgGA -losgDB -losgUtil -losgSim -losg -lOpenThreads -L/usr/local/lib -lGLU -lgsl -lgslcblas -lm
macx:LIBS += -framework osgQt -framework osgWidget -frameworks osgViewer -framework osgText \
        -framework osgGA -framework osgDB -framework osgUtil \
        -losg \ #-framework osg
        -lOpenThreads #libOpenThreads.dylib

INCLUDEPATH += $$PWD/FEMApp/FEMAppLib
INCLUDEPATH += $$PWD/FEMApp/FEMGraphLib
INCLUDEPATH += $$PWD/FEMApp/FEMLib
INCLUDEPATH += $$PWD/FEMApp/CoupledFields
INCLUDEPATH += $$PWD/FEMApp/GraphLib
INCLUDEPATH += $$PWD/FEMApp/XMLLib
INCLUDEPATH += $$PWD/FEMApp/MolecularLib
INCLUDEPATH += $$PWD/FEMApp/LangLib
INCLUDEPATH += $$PWD/FEMApp/MathLib

QMAKE_LIBDIR+= $$PWD/FEMApp/Release

LIBS += -L$$PWD/FEMApp/FEMAppLib/ -lFEMAppLibRelease
LIBS += -L$$PWD/FEMApp/FEMGraphLib/ -lFEMGraphLibRelease
LIBS += -L$$PWD/FEMApp/FEMLib/ -lFEMLibRelease
LIBS += -L$$PWD/FEMApp/CoupledFields/ -lCoupledFieldsRelease
LIBS += -L$$PWD/FEMApp/GraphLib/ -lGraphLibRelease
LIBS += -L$$PWD/FEMApp/XMLLib/ -lXMLLibRelease
LIBS += -L$$PWD/FEMApp/MolecularLib/ -lMolecularLibRelease
LIBS += -L$$PWD/FEMApp/LangLib/ -lLangLibRelease
LIBS += -L$$PWD/FEMApp/MathLib/ -lMathLibRelease

LIBS +=  -ldl -lX11 -llapack -lpthread -lgfortran -lm -fopenmp

