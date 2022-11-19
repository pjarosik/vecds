#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T21:29:49
#
#-------------------------------------------------

QT       -= core gui

TARGET = XMLLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    xml_text.cpp \
    xml_remark.cpp \
    xml_preprocessing.cpp \
    xml_parameter.cpp \
    xml_outputdevice.cpp \
    xml_node.cpp \
    xml_inputdevice.cpp \
    xml_header.cpp \
    xml_element.cpp \
    xml_doctype.cpp \
    xml_creator.cpp \
    xml_cdata.cpp \
    xml_analyser.cpp \
    XML.cpp

HEADERS += \
    xml_tokenizer.h \
    xml_text.h \
    xml_remark.h \
    xml_preprocessing.h \
    xml_parameter.h \
    xml_outputdevice.h \
    xml_node.h \
    xml_inputdevice.h \
    xml_header.h \
    xml_exceptions.h \
    xml_element.h \
    xml_doctype.h \
    xml_creator.h \
    xml_constant.h \
    xml_charvalidators.h \
    xml_cdata.h \
    xml_analyser.h \
    XML.h \
    iter_if.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=
