#ifndef MODEL_H
#define MODEL_H
#include "../MathLib/GeometricDomain.h"
#include "../MathLib/GeometricObjectPoint.h"
#include "../FEMLib/map_named.h"
#include "ModelObject.h"


class CModel
{
    CGeometricDomain GD;
    CModelObject object;
    double eps;

public:

    CModel( double e = 1.0E-06 );

    void SetEps( double e ) { eps = e; }
    const string& GetName() const { return object.GetName(); }
    CModelObject& GetObject() { return object; }
    CGeometricDomain& GetDomain() { return GD; }

    CGeometricObjectPoint* FindPoint( const mvector &X, bool ex = true );
    CGeometricObject* FindGeometricObject( const string &nm, bool ex=true );
    CModelObject* FindModelObject( const string &nm, bool ex=true );

    void StoreGeometricObject( CGeometricObject *go, const string &dn = "" ) { if ( !GD.GetGeometricDomain().Find( go ) ) GD.AddGeometricObject( go, dn ); }
    void StoreSpace( CSpace *space ) { GD.AddSpace( space ); }

    void DuplicateGeometricObject( CGeometricObject *go, const string &dn = "" );

};

#endif // MODEL_H
