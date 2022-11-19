#ifndef MODELOBJECT_H
#define MODELOBJECT_H
#include "../MathLib/GeometricObject.h"
#include "../MathLib/Space.h"

class CModel;
class CGeometricObjectPoint;

class CModelObject
{
    const string name;
    map< string, CModelObject* > modelobjects;
    map< string, CGeometricObject* > geomobjects;
    const CSpace *space;
    unsigned dim[ 4 ];
    CModel *root;

public:

    CModelObject( const string &nm, CModel *pr, const CSpace *sp = 0 );
    const string& GetName() const { return name; }
    const CSpace* GetSpace() const { return space; }

    bool AddModelObject( const string &nm, CModelObject *m, bool ex=true );
    bool AddGeomObject( const string &nm, CGeometricObject *m, bool ex=true );
    string CreateAutoName( unsigned dim );


    CGeometricObjectPoint* FindPoint( const string &nm, bool ex = true );
    CGeometricObject* FindGeometricObject( const string &nm, bool ex=true );
    CModelObject* FindModelObject( const string &nm, bool ex=true );


    void CloneObject( CGeometricObject &ego, const mvector &dx, CSpace *space );
    void ExtrudeObject( CGeometricObject &ego, const mvector &dx, CSpace *space );
};

#endif // MODELOBJECT_H
