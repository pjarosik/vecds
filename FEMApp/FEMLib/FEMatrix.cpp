
#include "FEMatrix.h"
#include "FEFunctorMatrix.h"
#include "FiniteElementInstance.h"
#include <algorithm>
#include "FEAlgorithms.h"
#include "FEFunctors.h"


CFEMatrix::~CFEMatrix( void )
{
//	for_each( MF.begin(), MF.end(), DeleteFunctorStructure );
	delete Matrix;
}

void CFEMatrix::AssignFiniteElements( CFEFunctorMatrix *mf, const vector< CFiniteElementInstance* > &fes )
{
	vector< CFiniteElementInstance* >::const_iterator i;
    CFEFunctorMatrix *m = dynamic_cast< CFEFunctorMatrix* >( mf->CloneTree() );
	MF[ m ] = fes;
}

void CFEMatrix::CreateMatrix( unsigned dim )
{
	map< CFEFunctorMatrix*, vector< CFiniteElementInstance* > >::iterator i;
	valarray< unsigned > alr, alc;
	Matrix->Create( dim );
	for ( i = MF.begin(); i!= MF.end(); i++ )
	{
		vector< CFiniteElementInstance* > &fes = i->second;
		CFEFunctorMatrix &mf = *i->first;
		vector< CFiniteElementInstance* >::iterator j;
		for ( j = fes.begin(); j != fes.end(); j++ )
		{
			mf.ManageDOFs( *j );
			mf.GetAllocationVectors( alr, alc );
			Matrix->UpdateProfile( alr, alc );
		}
	}
	Matrix->CreateMatrix();
}

void CFEMatrix::Aggregate( CProgress *prg )
{
	Matrix->SetZero();
    Section sc = createSection();
    MatrixAggregator< CLargeMatrix, CFiniteElementInstance, CFEFunctorMatrix > ag( *Matrix, &sc );
    aggregate( MF.begin(), MF.end(), ag, nthreads, prg );
	Matrix->FindFactorC();
    freeMutex(sc);
}


void CFEMatrix::CreateDOFs( )
{
	map< CFEFunctorMatrix*, vector< CFiniteElementInstance* > >::iterator i;
	for ( i = MF.begin(); i!= MF.end(); i++ )
	{
		vector< CFiniteElementInstance* > &fes = i->second;
		CFEFunctorMatrix &mf = *i->first;
		vector< CFiniteElementInstance* >::iterator j;
		for ( j = fes.begin(); j != fes.end(); j++ )
			mf.ManageDOFs( *j );
	}
}
