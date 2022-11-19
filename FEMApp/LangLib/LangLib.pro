#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T21:20:35
#
#-------------------------------------------------

QT       -= core gui

TARGET = LangLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    SubSetList.cpp \
    Statement.cpp \
    NumberList.cpp \
    LangStream.cpp \
    LangException.cpp \
    FileLangStream.cpp

HEADERS += \
    SubSetList.h \
    Statement.h \
    statdef.h \
    ParamList3.h \
    ParamList2.h \
    NumberList.h \
    NamedParameter.h \
    multiread.h \
    ListParameterWhite.h \
    ListParameter.h \
    LangToken.h \
    LangStream.h \
    LangException.h \
    FileLangStream.h \
    CharValidators.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=
