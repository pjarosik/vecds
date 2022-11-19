#ifndef FEFUNCTORMATRIXDEFORMATIONGRAD_H
#define FEFUNCTORMATRIXDEFORMATIONGRAD_H
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"

class CFEFunctorMatrixDeformationGrad : public CFEFunctorMatrix
   {
    public:
        CFEFunctorMatrixDeformationGrad( CFEFunctorMatrix *du );
        CFEFunctorMatrixDeformationGrad( const CFEFunctorMatrixDeformationGrad &mf );
        virtual ~CFEFunctorMatrixDeformationGrad(void);
        CLONE_FN( CFEFunctorMatrixDeformationGrad )
        virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

    protected:
        CFEFunctorMatrix *du;
        virtual void Compute( CFiniteElementInstance *fe1 );
};

#endif // FEFUNCTORMATRIXDEFORMATIONGRAD_H
