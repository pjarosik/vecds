// LargeMatrix.cpp: implementation of the CLargeMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "matrix.h"
#include "LargeMatrix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLargeMatrix::CLargeMatrix():total_elem_number( 0 ), dim( 0 ), factor_c( 0.0 ),section( createSection() )
{

}

CLargeMatrix::CLargeMatrix( const CLargeMatrix &lm ):total_elem_number( lm.total_elem_number ),dim( lm.dim ),  factor_c( lm.factor_c ), section( createSection() )
{

}

CLargeMatrix::~CLargeMatrix()
{
    freeMutex( section );
}


matrix operator *( const CLargeMatrix &LM, const mvector &v)
{
    mvector ret( v.GetDim() );
    LM.MultiplyVector( v, ret );
    return ret;
}
