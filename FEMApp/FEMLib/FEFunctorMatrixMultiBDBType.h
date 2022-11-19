#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorMatrixColDOF.h"

class CFEFunctorMatrixMultiBDBType :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixMultiBDBType( const vector< CFEFunctorMatrixColDOF* > &b1, CFEFunctorMatrix *d );
	CFEFunctorMatrixMultiBDBType( const CFEFunctorMatrixMultiBDBType &mmf );
	virtual ~CFEFunctorMatrixMultiBDBType( void );

protected:

	virtual void Compute( CFiniteElementInstance *fe1 );

    unsigned dim;
    vector< valarray< unsigned > > alv;

	matrix J;
	CFEFunctorMatrix *D;
	vector< CFEFunctorMatrixColDOF* > B;

};
