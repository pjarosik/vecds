#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T21:12:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = FEMAppLib
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += /opt/intel/mkl/include

DEFINES += USE_PARDISO

SOURCES += \
    TopOptFrame.cpp \
    StatementVariables.cpp \
    StatementTitle.cpp \
    StatementSpace.cpp \
    StatementSolve.cpp \
    StatementResults.cpp \
    StatementPlot.cpp \
    StatementOutput.cpp \
    StatementNodes.cpp \
    StatementModel.cpp \
    StatementMesh.cpp \
    StatementMaterial.cpp \
    StatementEquation.cpp \
    StatementElements.cpp \
    StatementElemDef.cpp \
    StatementBoundaryConditions.cpp \
    StatementAtoms.cpp \
    StatementAtom.cpp \
    StatamentDOFs.cpp \
    PostProcessingItemXMLOutput.cpp \
    operators.cpp \
    NodalSelectionReader.cpp \
    ModelObject.cpp \
    Model.cpp \
    FEMProject.cpp \
    FEMAppInterface.cpp \
    ElemSelectionReader.cpp \
    AtomAssignment.cpp \
    StatementFeap.cpp

HEADERS += \
    TopOptFrame.h \
    StatementVariables.h \
    StatementTitle.h \
    StatementSpace.h \
    StatementSolve.h \
    StatementResults.h \
    StatementPlot.h \
    StatementOutput.h \
    StatementNodes.h \
    StatementModel.h \
    StatementMesh.h \
    StatementMaterial.h \
    StatementEquation.h \
    StatementElements.h \
    StatementElemDef.h \
    StatementBoundaryConditions.h \
    StatementAtoms.h \
    StatementAtom.h \
    StatamentDOFs.h \
    PostProcessingItemXMLOutput.h \
    operators.h \
    NodalSelectionReader.h \
    ModelObject.h \
    Model.h \
    FEMProject.h \
    FEMAppInterface.h \
    ElemSelectionReader.h \
    AtomAssignment.h \
    StatementFeap.h \
    GraphData.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    user_init.f
