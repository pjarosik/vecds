#include "GeometricObjectPoint.h"
#include "GeometricDomain.h"

CGeometricObjectPoint::CGeometricObjectPoint( const mvector &a, CSpace *s ):CGeometricObject(  ), x( a )
{
    sp = s;
}

bool CGeometricObjectPoint::IsApproximatellyEqual( const CGeometricObject &cp, double eps ) const
{
    if ( !IsPoint() ) return false;
    const CGeometricObjectPoint *pt = dynamic_cast< const CGeometricObjectPoint* >( &cp );
    mvector X1;
    pt->GetX( X1 );
    return IsApproximatellyEqual( X1, eps );
}

bool CGeometricObjectPoint::IsApproximatellyEqual( const mvector &X2, double eps ) const
{
    mvector X1;
    GetX( X1 );

    if ( X1.GetDim() != X2.GetDim() ) return false;

    unsigned k;
    for (k=0; k<X1.GetDim(); k++ )
        if ( fabs( X1[ k ] - X2[ k ] ) > eps ) return false;

    return true;
}

void CGeometricObjectPoint::TransformObject( const CSpace *spt  )
{
    mvector X;
    spt->transform( x, X );
    x = X;
}
