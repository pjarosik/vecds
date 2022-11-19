/* 
 * File:   CFEFunctorMatrixTangentElem14.h
 * Author: piotrek
 *
 * Created on May 12, 2010, 3:19 PM
 */


#ifndef _CFEFunctorMatrixTangentElem14_H
#define	_CFEFunctorMatrixTangentElem14_H

#include "FEFunctorVector.h"
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixTangentElem14:
	public CFEFunctorMatrix {
public:
    
    CFEFunctorMatrixTangentElem14( CFEFunctorVector *qe, double d1, double d2 );
	CFEFunctorMatrixTangentElem14( const CFEFunctorMatrixTangentElem14 &mmf );
	virtual ~CFEFunctorMatrixTangentElem14( void ) { }
	CLONE_FN( CFEFunctorMatrixTangentElem14 )
        const mvector& GetResidualVector() const { return Rt; }

        
protected:

    mvector Rt;
    CFEFunctorVector *q;
    virtual void Compute( CFiniteElementInstance *fe1 );
    double d1,d2;

};

#endif	/* _CFEFunctorMatrixTangentElem14_H */

