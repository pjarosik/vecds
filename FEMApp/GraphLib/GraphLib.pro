#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T21:19:40
#
#-------------------------------------------------

QT       -= core gui

TARGET = GraphLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    ValueIndicator.cpp \
    string_tool.cpp \
    rect.cpp \
    Point2D.cpp \
    PlotXY.cpp \
    PlotLine.cpp \
    pen.cpp \
    PaintFrame.cpp \
    MultiBoxPlot.cpp \
    HistogramPlot.cpp \
    Histogram.cpp \
    GraphReceiverXML.cpp \
    GraphObjectGroup.cpp \
    GraphObject.cpp \
    Graphics.cpp \
    GraphDeviceXML.cpp \
    GraphDeviceSVG.cpp \
    GraphDeviceSizer.cpp \
    GraphDevicePS.cpp \
    GraphDevicePrm.cpp \
    GraphDevice.cpp \
    GraphDebugClass.cpp \
    color.cpp \
    brush.cpp \
    BoxPlot.cpp \
    BarPlot.cpp

HEADERS += \
    XMLDocument.h \
    ValueIndicator.h \
    string_tool.h \
    rect.h \
    Point2D.h \
    PlotXY.h \
    PlotLine.h \
    pen.h \
    PaintFrame.h \
    MultiBoxPlot.h \
    HistogramPlot.h \
    Histogram.h \
    GraphReceiverXML.h \
    GraphObjectGroup.h \
    GraphObject.h \
    Graphics.h \
    GraphDeviceXML.h \
    GraphDeviceSVG.h \
    GraphDeviceSizer.h \
    GraphDevicePS.h \
    GraphDevicePrm.h \
    GraphDevice.h \
    GraphDebugClass.h \
    graph_constants.h \
    color.h \
    brush.h \
    BoxPlot.h \
    BarPlot.h \
    GraphData.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=
