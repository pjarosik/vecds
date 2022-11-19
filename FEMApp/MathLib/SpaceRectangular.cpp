#include "SpaceRectangular.h"


CSpaceRectangular::CSpaceRectangular( const mvector &x, CSpace *sp ):CSpace( x.GetDim(), x.GetDim(), sp ), xs( x )
{

}

void CSpaceRectangular::transform( const mvector &x,  mvector &X ) const
{
   X = xs + x;
}

void CSpaceRectangular::jacobian( const mvector &x,  matrix &j ) const
{
    unsigned k;
    j.SetDim( xs.GetDim(), xs.GetDim()  );
    for (k=0; k<xs.GetDim(); k++) j(k,k) = 1.0;
}
