#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include <vector>
using namespace std;

class CFEFunctorMatrixTabElemVectors :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixTabElemVectors( const vector< mvector > &pts, CFEFunctorVector *x, const string &name );
	virtual ~CFEFunctorMatrixTabElemVectors( void ); 
	CFEFunctorMatrixTabElemVectors( const CFEFunctorMatrixTabElemVectors &mf );

	virtual void ManageDOFs(CFiniteElementInstance *fe1) { for(unsigned k=0; k<vfs.size(); k++) vfs[k]->ManageDOFs(fe1); }
	CFEFunctorMatrixTabElemVectors* operator<<( CFEFunctorVector *mf );

protected:

	virtual void Compute(CFiniteElementInstance *fe1);

	CFEFunctorVector *xi;
	const vector< mvector > pts;
	vector< CFEFunctorVector* > vfs;
};
