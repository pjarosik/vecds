#pragma once

#include "FiniteElementInstance.h"
#include "../MathLib/LargeMatrix.h"
#include "../MathLib/Progress.h"

class CLargeMatrix;
class CFEFunctorMatrix;

class CFEMatrix
{

public:

    CFEMatrix( CLargeMatrix *mt = 0, unsigned nt = 1 ):Matrix( mt ? mt->Clone() : 0 ) ,nthreads( nt ) { int a=0;  }
	~CFEMatrix( void );
    void SetMatrix( CLargeMatrix *mt ) { delete Matrix; Matrix = mt; }
    void SetNumberOfThreads( unsigned nt ) { nthreads = nt; }
	void AssignFiniteElements( CFEFunctorMatrix* mf, const vector< CFiniteElementInstance* > &fes );
	void CreateMatrix(unsigned dim);
	void Aggregate( CProgress *prg=0 );
	void CreateDOFs();
	void ApplyFixedBoundaryConditions( mvector &disp );
	void Print() const { Matrix->print(); }
	CLargeMatrix& GetMatrixObject( ) { return *Matrix; }

protected:

    unsigned nthreads;
	map< CFEFunctorMatrix* , vector< CFiniteElementInstance* > > MF;
	CLargeMatrix *Matrix;

};
