#ifndef CMATRIXFUNCTORCOUPLEDFIELDSNODALRESULTS_H
#define CMATRIXFUNCTORCOUPLEDFIELDSNODALRESULTS_H

#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"

class CFEFunctorMatrixCoupledFieldsNodalResults : public CFEFunctorVector
{
    public:
        CFEFunctorMatrixCoupledFieldsNodalResults( CFEFunctorVector *qe, const vector<string> &nms );
        CFEFunctorMatrixCoupledFieldsNodalResults( const CFEFunctorMatrixCoupledFieldsNodalResults &mf );
        virtual ~CFEFunctorMatrixCoupledFieldsNodalResults();
        CLONE_FN(CFEFunctorMatrixCoupledFieldsNodalResults)
    
        protected:

    CFEFunctorVector *q;

    virtual void Compute( CFiniteElementInstance *fe1 );

};

#endif // CMATRIXFUNCTORCOUPLEDFIELDSNODALRESULTS_H
