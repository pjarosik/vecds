#ifndef GEOMETRICOBJECT_H
#define GEOMETRICOBJECT_H

#include "Space.h"
#include <algorithm>
#include "AlgorithmsMath.h"
#include "GeometricDomain.h"
#include "Clonable.h"

class CGeometricObjectPoint;

class CGeometricObject //: public Copyable
{
public:

    CGeometricObject(  );
    CGeometricObject( const CGeometricObject& go );
//    CGeometricObject( CCopyer &cpr, const CGeometricObject &go );
    virtual ~CGeometricObject() { }

    CLONE_OBJECT( CGeometricObject )
//    COPY_OBJECT( CGeometricObject )

    virtual void TransformObject( const CSpace *spt ) {  }

    unsigned GetNumber() const { return id_number; }
    unsigned GetObjectDim() const { return object_dim; }
    unsigned GetSpaceDim() const { return sp ? sp->GetXdim() : GetObjectDim(); }

    const vector< CGeometricObject* >& GetOwned() const { return owns; }
    bool IsAdjacent( const CGeometricObject *go ) const { return find( adjacent.begin(), adjacent.end(), go ) != adjacent.end(); }
    bool Owns( const CGeometricObject *go ) const { return find( owns.begin(), owns.end(), go ) != owns.end(); }
    bool IsOwnedBy( const CGeometricObject *go ) const { return find( ownedby.begin(), ownedby.end(), go ) != ownedby.end(); }
    bool HasSingleOwner() const { return ownedby.size() == 1; }

    CGeometricObject* FindOwnObject( CGeometricObject &go, bool recurs = false ) const;
    CGeometricObject* FindOwnObject( unsigned d, const vector< CGeometricObject* > &objects, bool recurs = false ) const;
    CGeometricObject* FindEqualOwner( unsigned d, const vector< CGeometricObjectPoint* > &objects ) const;

    bool ContainsPoints( const vector< CGeometricObjectPoint *> &objectspoints ) const;
    const CSpace* GetSpace() const { return sp; }
    bool operator==( const CGeometricObject &go ) const;
    virtual bool IsApproximatellyEqual( const CGeometricObject &cp, double eps = 1E-06 ) const;

    const vector< CGeometricObject* >& GetIsOwnedSet() const { return ownedby; }

    void AddOwner( CGeometricObject *go ) { ownedby.push_back( go ); }
    void SetNumber( unsigned n ) { id_number = n; }
    void SetAdjacency( CGeometricObject *go ) { adjacent.push_back(go); go->AddAdjacent( this ); }
    void SetOwnershipOf( CGeometricObject *go  )  { owns.push_back(go); go->AddOwner( this ); }
    void FindBoundaryObjects();

    void SetAttribute( const string &key, const string &value ) { props[ key ] = value; }
    const string GetAttribute( const string &key ) const;
    bool IsBoundary() const { return is_boundary; }
    void SetBoundary( bool t, bool wo = true );

    void GetPoints( vector< CGeometricObject* > &points  )  const { GetObjects(points,0); }
    void GetEdges( vector< CGeometricObject* > &edges  )    const { GetObjects(edges,1);  }
    void GetFaces( vector< CGeometricObject* > &faces  )    const { GetObjects(faces,2);  }


    //! element selection accesor.
    /*!
        Returns true if element is selected
    */
    bool IsSelected() const { return isselected; }


    //! element selection setup.
    /*!
        Sets element selection state.
        \param s selection state.
    */
    void SetSelection( bool s ) { isselected = s; }
    bool IsPoint() const        { return object_dim == 0; }
    bool IsLine() const         { return object_dim == 1; }
    bool IsSurface() const      { return object_dim == 2; }
    bool IsVolume() const       { return object_dim == 3; }
    void Print( const string &marg, ofstream &fout ) const;

    CGeometricObject *selected_owner;

protected:

    void AddAdjacent ( CGeometricObject *go ) { adjacent.push_back( go ); }
    CGeometricObject* DeepClone( map< CGeometricObject*, CGeometricObject* > &dm  );
    void GetObjects( vector< CGeometricObject* > &obj, unsigned dim ) const;

    CSpace *sp;
    vector< CGeometricObject* >  owns, ownedby, adjacent, subelems;

    map< string, string > props;
    bool is_boundary, isselected;
    unsigned object_dim, id_number;

};


inline void CGeometricObject::SetBoundary( bool t, bool wo )
{
    is_boundary = t;
    if ( !wo ) return;
    vector< CGeometricObject* >::iterator i;
    for( i = owns.begin(); i != owns.end(); i++)
        (*i)->SetBoundary( t, wo );

}

inline const string CGeometricObject::GetAttribute( const string &key ) const
{
    map< string, string >::const_iterator i  = props.find( key );
    if ( i == props.end() ) return "";
    return i->second;
}


inline bool CGeometricObject::operator==( const CGeometricObject &go ) const
{
    if ( GetObjectDim() != go.GetObjectDim() ) return false;
    return CompareContainers( owns, go.owns );
}

inline CGeometricObject* CGeometricObject::FindOwnObject( unsigned d, const vector< CGeometricObject* > &objects, bool recurs  ) const
{
    vector< CGeometricObject* >::const_iterator i;
    for (i=owns.begin(); i!=owns.end(); i++)
    {
        if ( (*i)->GetObjectDim() == d && Contains( (*i)->owns, objects ) ) return *i;
        if ( recurs )
        {
            CGeometricObject *res = ( *i )->FindOwnObject( d, objects );
            if ( res ) return res;
        }
    }
    return 0;
}


inline CGeometricObject* CGeometricObject::FindOwnObject( CGeometricObject &go, bool recurs ) const
{
    vector< CGeometricObject* >::const_iterator i;

    for ( i = ownedby.begin(); i != ownedby.end(); i++ )
    {
        if ( **i == go )  return *i;
        if ( recurs )
        {
            CGeometricObject *res = ( *i )->FindOwnObject( go );
            if ( res ) return res;
        }
    }
    return 0;
}

#endif // GEOMETRICOBJECT_H
