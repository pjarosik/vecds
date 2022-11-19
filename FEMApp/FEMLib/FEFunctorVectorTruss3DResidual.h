/* 
 * File:   CFEFunctorVectorTRUSS3DResidualVector.h
 * Author: piotrek
 *
 * Created on September 8, 2010, 5:54 PM
 */

#ifndef _CMATRIXFUNCTORTRUSS3DRESIDUALVECTOR_H
#define	_CMATRIXFUNCTORTRUSS3DRESIDUALVECTOR_H

#include "FEFunctorVector.h"

class CFEFunctorVectorTruss3DResidual : public CFEFunctorVector {
public:
    CFEFunctorVectorTruss3DResidual(CFEFunctorVector *q);
    CFEFunctorVectorTruss3DResidual(const CFEFunctorVectorTruss3DResidual& orig);
    virtual ~CFEFunctorVectorTruss3DResidual();
    CLONE_FN( CFEFunctorVectorTruss3DResidual )

protected:

    virtual void Compute( CFiniteElementInstance *fe1 );
    CFEFunctorVector *qe;

};

#endif	/* _CMATRIXFUNCTORTRUSS3DRESIDUALVECTOR_H */

