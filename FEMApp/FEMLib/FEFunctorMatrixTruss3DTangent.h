/* 
 * File:   CFEFunctorMatrixTRUSS3DTangentMatrix.h
 * Author: piotrek
 *
 * Created on September 8, 2010, 5:17 PM
 */

#ifndef _CMATRIXFUNCTORTRUSS3DTANGENTMATRIX_H
#define	_CMATRIXFUNCTORTRUSS3DTANGENTMATRIX_H

#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"

class CFEFunctorMatrixTruss3DTangent : public CFEFunctorMatrix {
public:
    CFEFunctorMatrixTruss3DTangent(CFEFunctorVector *q);
    CFEFunctorMatrixTruss3DTangent(const CFEFunctorMatrixTruss3DTangent& orig);
    virtual ~CFEFunctorMatrixTruss3DTangent();

    CLONE_FN(CFEFunctorMatrixTruss3DTangent)
    
protected:
    virtual void Compute( CFiniteElementInstance *fe1 );
    CFEFunctorVector *qe;
};

#endif	/* _CMATRIXFUNCTORTRUSS3DTANGENTMATRIX_H */

