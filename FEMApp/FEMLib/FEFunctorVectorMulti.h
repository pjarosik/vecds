#pragma once
#include "FEFunctorVector.h"

class CFEFunctorVectorMulti:
	public CFEFunctorVector {
public:
    CFEFunctorVectorMulti( );
	CFEFunctorVectorMulti( const CFEFunctorVectorMulti &vf );
	virtual ~CFEFunctorVectorMulti( void ) { }
	CLONE_FN( CFEFunctorVectorMulti )

	void Add( CFEFunctorVector *vf );

protected:
	vector<CFEFunctorVector *> VF;
	virtual void Compute( CFiniteElementInstance *fe1 );


};
 
