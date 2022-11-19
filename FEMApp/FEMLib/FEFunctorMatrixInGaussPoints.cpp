#include "FEFunctorMatrixInGaussPoints.h"
#include <sstream>
#include <iomanip>

CFEFunctorMatrixInGaussPoints::CFEFunctorMatrixInGaussPoints( const CIntegrationManager &im, CFEFunctorVector *x, CFEFunctorVector *i ):CFEFunctorMatrix("FEFunctorMatrixInGaussPoints"), IM( im ), xi(x), fn_vector( i )
{
    AddNode( x );
	AddNode( i );
	cols_names = fn_vector->GetNames();
    ComputeDependent = false;
}

CFEFunctorMatrixInGaussPoints::CFEFunctorMatrixInGaussPoints( const CFEFunctorMatrixInGaussPoints &mf ):CFEFunctorMatrix(mf),IM( mf.IM )
{
    CLONE_MEMBER( mf, xi )
	CLONE_MEMBER( mf, fn_vector )
}

CFEFunctorMatrixInGaussPoints::~CFEFunctorMatrixInGaussPoints( void )
{

}

void CFEFunctorMatrixInGaussPoints::Compute( CFiniteElementInstance *fe )
{
    const CSpaceShape &sf = fe->GetShapeFunctions();
    const CIntegrationPoints *ip = IM.GetIntegrationPoints( sf.GetShapeType(), sf.Getxdim(), sf.GetDegree() );
    unsigned ipdim = ip->N( );
    fv.SetDim( ipdim, cols_names.size() );
    unsigned k, l;
    if ( rows_names.size() != ip->N( ) )
    {
        rows_names.clear();
        for (k=0; k<ip->N(); k++)
        {
            stringstream ss;
            ss << "gp:"<< k << ", ";
            for (l=0; l<ip->GetPoints()[ k ].GetDim(); l++ )
                ss << "xi" << l << "=" << setw( 9 ) << ip->GetPoints()[ k ][ l ] << ", ";
            rows_names.push_back( ss.str() );
        }
    }
	for (k=0; k<ip->N(); k++)
	{
        *xi = ip->GetPoints()[ k ];
        (*fn_vector)(fe);
		const mvector &V = *fn_vector;
		for (l=0; l<fn_vector->GetDim(); l++)
			fv( k, l ) = V[ l ];
	}

}

void CFEFunctorMatrixInGaussPoints::ManageDOFs(CFiniteElementInstance *fe1)
{
	fn_vector->ManageDOFs(fe1);
}
