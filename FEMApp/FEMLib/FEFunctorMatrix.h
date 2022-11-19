#pragma once

//#include <boost/numeric/ublas/matrix.hpp>
#include "../MathLib/matrix.h"
//#include "../MathLib/LargeMatrix.h"
#include "FEFunctor.h"
#include "FEAllocator.h"
#include <string>
using namespace std;



class CFEFunctorMatrix : public CFEFunctor< CFiniteElementInstance >
{
public:

    CFEFunctorMatrix( const string &nm );
    CFEFunctorMatrix( const CFEFunctorMatrix &mf );

	using CFEFunctor< CFiniteElementInstance >::operator();
	friend CLargeMatrix& operator << ( CLargeMatrix &L, const CFEFunctorMatrix &mf );

	virtual ~CFEFunctorMatrix( void );
	virtual CFEFunctorMatrix* Clone()=0;
    virtual void ManageDOFs( CFiniteElementInstance *p )
    {
        alr.CollectDOFs( p );
        alc.CollectDOFs( p );
    }

    operator matrix&() { return fv; }
    operator const matrix&() const { return fv; }
    matrix& operator=( const matrix &v ) { InvalidateUsedByObjects(); fv = v; return fv; }

	const vector<string>& GetRowsNames() const { return rows_names; }
	const vector<string>& GetColsNames() const { return cols_names; }

	const CFEAllocator& GetRowAllocator( ) const { return alr; }
	const CFEAllocator& GetColAllocator( ) const { return alc; }
	void GetAllocationVectors( valarray< unsigned > &ar, valarray< unsigned > &ac ) const;
	
	ostream& PrintFunctor( ostream &out ) const;
	virtual void DebugPrint( ) const;

	double operator() (unsigned i, unsigned j) const { return fv(i,j); }
    const matrix& GetValue() const { return fv; }
	
protected:

	CFEAllocator alr, alc;
    matrix fv;
	vector< string > rows_names, cols_names;

};


CLargeMatrix& operator << ( CLargeMatrix &L, const CFEFunctorMatrix &mf );
ostream& operator << ( ostream &out, const CFEFunctorMatrix &mf );



