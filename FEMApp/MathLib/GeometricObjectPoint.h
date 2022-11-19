#ifndef GEOMETRICOBJECTPOINT_H
#define GEOMETRICOBJECTPOINT_H
#include "Space.h"
#include "GeometricObject.h"
#include "SpaceShapeApproxPtr.h"
#include "GeometricObjectShape.h"

class CGeometricObject;

class CGeometricObjectPoint : public CGeometricObject
{
public:

    CGeometricObjectPoint( const mvector &a, CSpace *sp = 0 );
    CGeometricObjectPoint( const CGeometricObjectPoint &cp ):CGeometricObject(cp),x( cp.x ) { }
    CLONE_OBJECT( CGeometricObjectPoint )
    virtual void TransformObject( const CSpace *spt );
    virtual void GetX( mvector &X ) const { if ( sp ) sp->Transform(x,X); else X=x; }
    const mvector& Getx() const { return x; }
    virtual double operator[](unsigned i) const { return x[i]; }
    virtual operator const mvector&() const { return x; }
    size_t GetDim() const { return x.GetDim(); }
    size_t GetObjectDim() const { return sp ? sp->GetXdim() : x.GetDim(); }
    virtual bool IsApproximatellyEqual( const CGeometricObject &cp, double eps = 1E-06 ) const ;
    virtual bool IsApproximatellyEqual( const mvector &Xp, double eps = 1E-06 ) const;

protected:

    mvector x;

};

typedef CSpaceShapeApproxPtr< CGeometricObjectPoint > CSpaceShapeApproxPoint;
//typedef CGeometricObjectShape< CGeometricObjectPoint > CGeometricObjectPointBasedShape;

#endif // GEOMETRICOBJECTPOINT_H
