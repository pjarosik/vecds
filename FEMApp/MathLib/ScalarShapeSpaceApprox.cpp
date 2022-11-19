#include "ScalarShapeSpaceApprox.h"

CScalarShapeSpaceApprox::CScalarShapeSpaceApprox(const CSpace &s, const vector<double> &v, const string &nm ):ss(s), values(v), name(nm)
{
    if ( ss.GetXdim() != values.size() ) throw CMathException("CScalarShapeSpaceApprox::CScalarShapeSpaceApprox : space output dimension diferent from values vector size");
}


double CScalarShapeSpaceApprox::interpolate( const mvector &x ) const
{
    unsigned k;
    mvector N( ss.GetXdim() );
    ss.transform( x, N );
    double res = 0;
    for ( k = 0; k < N.GetDim(); k++ )
        res += N[ k ] * values[ k ];
    return res;
}
