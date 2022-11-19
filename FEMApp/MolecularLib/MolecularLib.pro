#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T21:29:00
#
#-------------------------------------------------

QT       -= core gui

TARGET = MolecularLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    MolecularStructure.cpp \
    Atom.cpp

HEADERS += \
    MolecularStructure.h \
    MException.h \
    Atom.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
