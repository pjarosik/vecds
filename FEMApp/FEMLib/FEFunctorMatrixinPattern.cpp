#include "FEFunctorMatrixinPattern.h"
#include <sstream>

CFEFunctorMatrixInPattern::CFEFunctorMatrixInPattern( CFEFunctorMatrix *p, CFEFunctorVector *a, CFEFunctorVector *f ):CFEFunctorMatrix("Pattern"), pattern( p ), xi( a ), fn_vector( f )
{
    AddNode( p );
    AddNode( a );
    AddNode( f );
    cols_names = fn_vector->GetNames();
}

CFEFunctorMatrixInPattern::CFEFunctorMatrixInPattern( const CFEFunctorMatrixInPattern &mf ):CFEFunctorMatrix( mf )
{
    CLONE_MEMBER( mf, xi )
    CLONE_MEMBER( mf, pattern )
    CLONE_MEMBER( mf, fn_vector )
}

CFEFunctorMatrixInPattern::~CFEFunctorMatrixInPattern( void )
{

}

void CFEFunctorMatrixInPattern::Compute( CFiniteElementInstance *fe )
{

    const CSpaceShape &sf = fe->GetShapeFunctions();
    const matrix &xp = pattern->GetValue();
    fv.SetDim( xp.GetRows(), fn_vector->GetDim() );
    unsigned k, l;

    if ( rows_names.size() == 0 )
        for (k=0; k<xp.GetRows(); k++)
        {
            stringstream ss;
            ss << "pt:"<< k << ", ";
            for (l=0; l<xp.GetCols(); l++ )
                ss << "xi" << l << "=" << xp(k,l) << " ";

        }

    for (k=0; k<xp.GetRows(); k++)
    {
        for (l=0; l<xi->GetDim(); l++)
            (*xi)[l] = xp(k,l);
        (*fn_vector)(fe);
        const mvector &V = *fn_vector;
        for (l=0; l<fn_vector->GetDim(); l++)
            fv( k, l ) = V[ l ];
    }

}



