
#include "FEFunctorMatrix.h"
#include <iomanip>




CFEFunctorMatrix::CFEFunctorMatrix( const string &nm ):CFEFunctor< CFiniteElementInstance >(nm)
{
}

CFEFunctorMatrix::CFEFunctorMatrix( const CFEFunctorMatrix &mf ):CFEFunctor< CFiniteElementInstance >( mf ),alr(mf.alr),alc(mf.alc),fv(mf.fv),
	rows_names(mf.rows_names),cols_names(mf.cols_names)
{

}


CFEFunctorMatrix::~CFEFunctorMatrix(void)
{
}

void CFEFunctorMatrix::GetAllocationVectors( valarray<unsigned> &ar, valarray<unsigned> &ac ) const
{
	alr.GetAllocationVector( ar );
	alc.GetAllocationVector( ac );
}

ostream& CFEFunctorMatrix::PrintFunctor( ostream &out ) const
{
	out.setf( ios::scientific );
	out.precision( 10 );
	unsigned k,l;
	for (l=0; l<fv.GetCols(); l++)
	{
		for (k=0; k<fv.GetRows(); k++)
			out << setw( 20 ) << fv(k,l);
		out << endl;
	}
	return out;
}


void CFEFunctorMatrix::DebugPrint( ) const
{
	fv.print( objectname );
}



CLargeMatrix& operator << ( CLargeMatrix &L, const CFEFunctorMatrix &mf )
{
	valarray< unsigned > alc, alr;
	mf.GetAllocationVectors( alr, alc );
	L.AllocateMatrix( alr, alc, mf );
	return L;
}


ostream& operator << ( ostream &out, const CFEFunctorMatrix &mf )
{
	return mf.PrintFunctor( out );
}
