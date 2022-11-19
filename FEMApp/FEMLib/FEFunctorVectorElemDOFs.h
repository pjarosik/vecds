#pragma once
#include "FEFunctorVector.h"
#include <valarray> 
#include "DOFs.h"

using namespace std;

class CFEFunctorVectorElemDOFs :
	public CFEFunctorVector
{
public:
	CFEFunctorVectorElemDOFs( CDOF* types[], unsigned n, const string &nm );
	virtual ~CFEFunctorVectorElemDOFs( void );
	CLONE_FN( CFEFunctorVectorElemDOFs )
	const CDOFVector& GetTypes( ) const { return types; }

protected:

	void AddDOF( unsigned type );

	virtual void Compute( CFiniteElementInstance *fe1 );
	CDOFVector types;

};


