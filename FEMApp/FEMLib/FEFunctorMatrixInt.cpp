#include "FEFunctorMatrixInt.h"

CFEFunctorMatrixInt::CFEFunctorMatrixInt( const CIntegrationManager *im , CFEFunctorVector *x, CFEFunctorMatrix *i, CFEFunctorMatrix *j ):CFEFunctorMatrix("CFEFunctorMatrixInt"), ip(0), IM( im ), integrand( i ), Jacoby( j ), xi( x )
{
	AddNode( x );
	AddNode( i );
	AddNode( j );
	alc.Init( i->GetColAllocator() );
	alr.Init( i->GetRowAllocator() );
	ComputeDependent = false;
}

CFEFunctorMatrixInt::CFEFunctorMatrixInt( const CIntegrationPoints *ip , CFEFunctorVector *x, CFEFunctorMatrix *i, CFEFunctorMatrix *j ):CFEFunctorMatrix("CFEFunctorMatrixInt"), IM(0), ip( ip ), integrand( i ), Jacoby( j ), xi( x )
{
	ComputeDependent = false;
	AddNode( x );
	AddNode( i );
	AddNode( j );
	alc.Init( i->GetColAllocator() );
	alr.Init( i->GetRowAllocator() );
}

CFEFunctorMatrixInt::CFEFunctorMatrixInt( const CFEFunctorMatrixInt &mf ):CFEFunctorMatrix( mf ), IM( mf.IM ), ip( mf.ip )
{
	ComputeDependent = false;
	CLONE_MEMBER( mf, xi )
	CLONE_MEMBER( mf, integrand )
	CLONE_MEMBER( mf, Jacoby )
}

CFEFunctorMatrixInt::~CFEFunctorMatrixInt( void )
{

}

void CFEFunctorMatrixInt::Compute( CFiniteElementInstance *fe )
{
	const CIntegrationPoints *ipt = ip ? ip : fe->GetIntegrationPoints( *IM );
	matrix &J = (*Jacoby);
	unsigned k;
	ManageDOFs( fe );
	fv.SetDim( alr.GetDim(), alc.GetDim() );
    volume = 0;
	for (k=0; k<ipt->N(); k++)
	{
		*xi = ipt->GetPoints()[ k ];
		(*Jacoby)( fe );
		(*integrand)( fe );
        double det = fabs( J.Det() );
        fv += ipt->GetWeight( k ) * det * ( *integrand );
        volume += ipt->GetWeight( k ) * det;
	}
}

