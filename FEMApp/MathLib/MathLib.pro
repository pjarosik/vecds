#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T21:28:19
#
#-------------------------------------------------

QT       -= core gui

TARGET = MathLib
TEMPLATE = lib
CONFIG += staticlib

DEFINES += USE_PARDISO

INCLUDEPATH += /opt/intel/mkl/include

SOURCES += \
    VectorMaxMin.cpp \
    VariableGroup.cpp \
    Variable.cpp \
    Timer.cpp \
    SVM.cpp \
    SubspaceIndexer.cpp \
    SpaceTranslation.cpp \
    SpaceSpherical.cpp \
    SpaceShapeTriangle3.cpp \
    SpaceShapeLagrange4N_3D_64.cpp \
    SpaceShapeLagrange4N_2D_16.cpp \
    SpaceShapeLagrange4N_1D_4.cpp \
    SpaceShapeLagrange4N.cpp \
    SpaceShapeLagrange3N_3D_27.cpp \
    SpaceShapeLagrange3N_2D_9.cpp \
    SpaceShapeLagrange3N_1D_3.cpp \
    SpaceShapeLagrange3N.cpp \
    SpaceShapeLagrange2N_3D_8.cpp \
    SpaceShapeLagrange2N_2D_4.cpp \
    SpaceShapeLagrange2N_1D_2.cpp \
    SpaceShapeLagrange2N.cpp \
    SpaceShapeLagrange.cpp \
    SpaceShapeApproxPtr.cpp \
    SpaceShapeApprox.cpp \
    SpaceShape.cpp \
    SpaceRotation3D.cpp \
    SpaceRectangular.cpp \
    SpaceProjection.cpp \
    SpacePolar.cpp \
    SpacePerspective.cpp \
    SpaceMorph1D.cpp \
    SpaceLine.cpp \
    SpaceExtend2D3D.cpp \
    SpaceCylindrical.cpp \
    SpaceAxonometry.cpp \
    Space.cpp \
    Setup.cpp \
    Scaler.cpp \
    ScalarFunctionPolylinear.cpp \
    ScalarFunction.cpp \
    ProgressConsole.cpp \
    Progress.cpp \
    NCounter.cpp \
    MaxMin.cpp \
    MatrixMemoryManager.cpp \
    MatrixMaxMin.cpp \
    MatrixAllocators.cpp \
    matrix.cpp \
    LargeMatrixSparseCR_PCG.cpp \
    LargeMatrixSparseCRJacoby.cpp \
    LargeMatrixSparseCR_GaussSeidel.cpp \
    LargeMatrixSparseCR_CG.cpp \
    LargeMatrixSparseCR.cpp \
    LargeMatrixSkyLineCholesky2.cpp \
    LargeMatrixSkyLineCholesky.cpp \
    LargeMatrixSkyLine2.cpp \
    LargeMatrixSkyLine.cpp \
    LargeMatrixAsymmetricalSkyLine.cpp \
    LargeMatrixAsymmetricalCrout.cpp \
    LargeMatrix.cpp \
    LagrangeInterpolator.cpp \
    GeometricObjectShape.cpp \
    GeometricObjectPoint.cpp \
    GeometricObjectMorph.cpp \
    GeometricObject.cpp \
    GeometricDomain.cpp \
    ExpressionVariable.cpp \
    ExpressionTanh.cpp \
    ExpressionTangens.cpp \
    ExpressionSum.cpp \
    ExpressionSubtraction.cpp \
    ExpressionSquareroot.cpp \
    ExpressionSinus.cpp \
    ExpressionSinh.cpp \
    ExpressionSgn.cpp \
    ExpressionRand.cpp \
    ExpressionQuotient.cpp \
    ExpressionProduct.cpp \
    ExpressionPower.cpp \
    ExpressionPI.cpp \
    ExpressionNegative.cpp \
    ExpressionLog.cpp \
    ExpressionLn.cpp \
    ExpressionHeavisite.cpp \
    ExpressionExp.cpp \
    ExpressionE.cpp \
    ExpressionCosinus.cpp \
    ExpressionCosh.cpp \
    ExpressionConstant.cpp \
    ExpressionBracket.cpp \
    ExpressionAtan.cpp \
    ExpressionAsin.cpp \
    ExpressionAcos.cpp \
    ExpressionAbs.cpp \
    Expression.cpp \
    ERF.cpp \
    DomainContainer.cpp \
    DBNode.cpp \
    arrays.cpp \
    ScalarShapeSpaceApprox.cpp \
    LargeMatrixSparseStream.cpp \
    LargeMatrixSparseStream_PCG.cpp \
    LargeMatrixSparseIncompleteLU.cpp \
    LargeMatrixSparseCR_Pardiso.cpp

HEADERS += \
    VectorMaxMin.h \
    var_types.h \
    VariableVectorTemplate.h \
    VariableTemplate.h \
    VariableGroup.h \
    Variable.h \
    Timer.h \
    threads.h \
    SVM.h \
    SubspaceIndexer.h \
    SpaceTranslation.h \
    SpaceSpherical.h \
    SpaceShapeTriangle3.h \
    SpaceShapeLagrange4N_3D_64.h \
    SpaceShapeLagrange4N_2D_16.h \
    SpaceShapeLagrange4N_1D_4.h \
    SpaceShapeLagrange4N.h \
    SpaceShapeLagrange3N_3D_27.h \
    SpaceShapeLagrange3N_2D_9.h \
    SpaceShapeLagrange3N_1D_3.h \
    SpaceShapeLagrange3N.h \
    SpaceShapeLagrange2N_3D_8.h \
    SpaceShapeLagrange2N_2D_4.h \
    SpaceShapeLagrange2N_1D_2.h \
    SpaceShapeLagrange2N.h \
    SpaceShapeLagrange.h \
    SpaceShapeApproxPtr.h \
    SpaceShapeApprox.h \
    SpaceShape.h \
    SpaceRotation3D.h \
    SpaceRectangular.h \
    SpaceProjection.h \
    SpacePolar.h \
    SpacePerspective.h \
    SpaceMorph1D.h \
    SpaceLine.h \
    SpaceExtend2D3D.h \
    SpaceCylindrical.h \
    SpaceAxonometry.h \
    Space.h \
    SimpleVariables.h \
    Setup.h \
    Scaler.h \
    ScalarFunctionPolylinear.h \
    ScalarFunction.h \
    ProgressConsole.h \
    Progress.h \
    pointer.h \
    NCounter.h \
    MaxMin.h \
    MatrixMemoryManager.h \
    MatrixMaxMin.h \
    MatrixAllocators.h \
    matrix.h \
    MathException.h \
    LargeMatrixSparseCR_PCG.h \
    LargeMatrixSparseCRJacoby.h \
    LargeMatrixSparseCR_GaussSeidel.h \
    LargeMatrixSparseCR_CG.h \
    LargeMatrixSparseCR.h \
    LargeMatrixSkyLineCholesky2.h \
    LargeMatrixSkyLineCholesky.h \
    LargeMatrixSkyLine2.h \
    LargeMatrixSkyLine.h \
    LargeMatrixAsymmetricalSkyLine.h \
    LargeMatrixAsymmetricalCrout.h \
    LargeMatrix.h \
    LagrangeInterpolator.h \
    GeometricObjectShape.h \
    GeometricObjectPoint.h \
    GeometricObjectMorph.h \
    GeometricObject.h \
    GeometricDomain.h \
    ExpressionVariable.h \
    ExpressionTanh.h \
    ExpressionTangens.h \
    ExpressionSum.h \
    ExpressionSubtraction.h \
    ExpressionSquareroot.h \
    ExpressionSinus.h \
    ExpressionSinh.h \
    ExpressionSgn.h \
    ExpressionRand.h \
    ExpressionQuotient.h \
    ExpressionProduct.h \
    ExpressionPower.h \
    ExpressionPI.h \
    ExpressionNegative.h \
    ExpressionLog.h \
    ExpressionLn.h \
    ExpressionHeavisite.h \
    ExpressionExp.h \
    ExpressionE.h \
    ExpressionCosinus.h \
    ExpressionCosh.h \
    ExpressionConstant.h \
    ExpressionBracket.h \
    ExpressionAtan.h \
    ExpressionAsin.h \
    ExpressionAcos.h \
    ExpressionAbs.h \
    Expression.h \
    ERF.h \
    DomainContainer.h \
    DBNode.h \
    CSpaceShapeLagrange.h \
    constants.h \
    Cloner.h \
    Clonable.h \
    arrays.h \
    AlgorithmsMath.h \
    ScalarShapeSpaceApprox.h \
    LargeMatrixSparseStream.h \
    LargeMatrixSparseStream_PCG.h \
    LargeMatrixSparseIncompleteLU.h \
    LargeMatrixSparseCR_Pardiso.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=
