/* 
 * File:   CFEFunctorVectorResidualElem14.h
 * Author: piotrek
 *
 * Created on September 10, 2010, 11:22 AM
 */

#ifndef _CVECTORFUNCTORRESIDUALELEM14_H
#define	_CVECTORFUNCTORRESIDUALELEM14_H
#include "FEFunctorVector.h"
#include "FEFunctorMatrixTangentElem14.h"

class CFEFunctorVectorResidualElem14 :
    public  CFEFunctorVector

{
public:
    CFEFunctorVectorResidualElem14(CFEFunctorVector *k , double b, double c);
	CFEFunctorVectorResidualElem14( const CFEFunctorVectorResidualElem14 &mmf );
	virtual ~CFEFunctorVectorResidualElem14( void ) { }
	CLONE_FN( CFEFunctorVectorResidualElem14 )

protected:

        CFEFunctorVector *q;
        virtual void Compute( CFiniteElementInstance *fe1 );
        double d1,d2;
};

#endif	/* _CVECTORFUNCTORRESIDUALELEM14_H */

