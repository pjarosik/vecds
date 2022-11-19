#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T21:14:51
#
#-------------------------------------------------

QT       -= core gui

TARGET = FEMGraphLib
TEMPLATE = lib
CONFIG += staticlib

DEFINES += FEMGRAPHLIB_LIBRARY

SOURCES += \
    PostProcessingItemPlotGraph.cpp \
    PostProcessingItemPlot.cpp \
    PaintSetup.cpp \
    PaintFrameFECanvas.cpp \
    GraphObject.cpp \
    GraphNode.cpp \
    GraphModel.cpp \
    GraphFE.cpp \
    GraphFace.cpp \
    GraphEdge.cpp \
    Contour.cpp

HEADERS += \
    PostProcessingItemPlotGraph.h \
    PostProcessingItemPlot.h \
    PaintSetup.h \
    PaintFrameFECanvas.h \
    GraphObject.h \
    GraphNode.h \
    GraphModel.h \
    GraphFE.h \
    GraphFace.h \
    GraphEdge.h \
    GraphAlgorithms.h \
    Contour.h \
    CGraphObject.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
