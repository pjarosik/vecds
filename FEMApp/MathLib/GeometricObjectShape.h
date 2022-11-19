#ifndef GEOMETRICOBJECTSHAPE_H
#define GEOMETRICOBJECTSHAPE_H

#include "GeometricObject.h"
#include "GeometricObjectPoint.h"
#include "SpaceShape.h"
#include "SpaceShapeApproxPtr.h"
#include "GeometricDomain.h"

template <class T>
class CGeometricObjectShape : public CGeometricObject
{
public:
    CGeometricObjectShape( CGeometricDomain &gd, const CSpaceShapeApproxPtr< T > *s );
    CGeometricObjectShape( const CGeometricObjectShape &gs );
    virtual ~CGeometricObjectShape() { }

    void GetX( const mvector &x, mvector &X ) const { return main_shape->Transform(x,X); }
    const CSpaceShapeApproxPtr< T >* GetShapeSpace() const { return main_shape; }

    CLONE_OBJECT( CGeometricObjectShape<T> )

protected:
    const CSpaceShapeApproxPtr< T > *main_shape;
    CGeometricObject* AddSubObjects( CGeometricDomain &gd, const vector< unsigned > &ids, const CSpaceShape&, const vector< T* > &pts );
};

template <class T>
CGeometricObjectShape<T>::CGeometricObjectShape( CGeometricDomain &gd, const CSpaceShapeApproxPtr< T > *s ):CGeometricObject( ), main_shape( s )
{
    const CSpaceShape *ss = &( s->GetShapeSpace() );
    const CSpace *s1 = s;
    sp = const_cast< CSpace* >( s1 );
    object_dim = ss->Getxdim();
    const vector< T* > &points = s->GetPoints();
    unsigned k;
    for (k=0; k<points.size(); k++)
        SetOwnershipOf( points[ k ] );

    const vector< vector< unsigned > > &ei = ss->GetEdgeInds(), &fi = ss->GetFaceInds();
    const vector< CSpaceShape* > &es = ss->GetEdgeShapes(), &fs = ss->GetFaceShapes();

    vector< vector< unsigned > >::const_iterator i;
    vector< CSpaceShape* >::const_iterator i1 = es.begin();
    for ( i = ei.begin(); i!= ei.end(); i++, i1++ )
        AddSubObjects( gd, *i, **i1, points );

    vector< vector< unsigned > >::const_iterator j;
    vector< CSpaceShape* >::const_iterator j1 = fs.begin();
    for ( j = fi.begin(); j!= fi.end(); j++, j1++ )
        AddSubObjects( gd, *j, **j1, points );
}

template <class T>
CGeometricObjectShape<T>::CGeometricObjectShape( const CGeometricObjectShape &gs ):CGeometricObject( gs ), main_shape( gs.main_shape )
{

}

template <class T>
CGeometricObject* CGeometricObjectShape<T>::AddSubObjects( CGeometricDomain &gd, const vector< unsigned > &ids, const CSpaceShape &os, const vector< T* > &points )
{
    CGeometricObject *res = 0;
    vector< CGeometricObjectPoint* > objectspoints;
    vector< T* > templatepointobjects;
    unsigned k;

//  Extracting points according to its indices in parent object
    for( k = 0; k < ids.size(); k++ )
    {
        objectspoints.push_back( points[ ids[ k ] ] );
        templatepointobjects.push_back( points[ ids[ k ] ] );
    }

    typename vector< T* >::const_iterator j;
    CGeometricObject *new_obj = 0;

//  Checking if added object has the same points as any object owned by a point.
    for ( j = points.begin(); j != points.end(); j++ )
    {
        new_obj = (*j)->FindEqualOwner( os.Getxdim(), objectspoints );
        if ( new_obj )
        {
            break;
        }
    }

// if not create a new CGeometricObjectShape object
    if ( new_obj == 0 )
    {
        unsigned odim = templatepointobjects[ 0 ]->GetDim();
        CSpaceShapeApproxPtr< T > *ssa = new CSpaceShapeApproxPtr< T >( os, templatepointobjects, odim, const_cast<CSpace*>(main_shape->GetSpace()) );
        res = new_obj = new CGeometricObjectShape( gd, ssa );
        gd.AddSpace(ssa,"ObjectShape");
        switch( os.Getxdim() )
        {
            case 0 : gd.AddGeometricObject(new_obj,"point"); break;
            case 1 : gd.AddGeometricObject(new_obj,"edge"); break;
            case 2 : gd.AddGeometricObject(new_obj,"face"); break;
            case 3 : gd.AddGeometricObject(new_obj,"solid"); break;
        }
    }

    if ( new_obj->GetObjectDim() < GetObjectDim() ) { SetOwnershipOf( new_obj ); }
    else if ( new_obj->GetObjectDim() == GetObjectDim() ) { SetAdjacency( new_obj ); }
         else new_obj->SetOwnershipOf( this );

    return new_obj;
}


#endif // GEOMETRICOBJECTSHAPE_H
