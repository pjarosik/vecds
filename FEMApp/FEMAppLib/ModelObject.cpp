#include "ModelObject.h"
#include "Model.h"
#include "../FEMLib/FEException.h"
#include "../MathLib/SpaceTranslation.h"
#include <sstream>

CModelObject::CModelObject( const string &nm, CModel *pr, const CSpace *sp ):name( nm ), dim( { 0, 0, 0, 0 } ), root( pr ), space( sp )
{
}


bool CModelObject::AddModelObject( const string &nm, CModelObject *m, bool ex )
{
    map< string, CModelObject* >::const_iterator i = modelobjects.find( nm );
    if ( i != modelobjects.end() ) {
        if ( ex ) throw CFEException( "Model object '" + nm + "' already exists in model object '" + name + "'" );
        else return false;
    }
    modelobjects[ nm ] = m;
    return true;
}


bool CModelObject::AddGeomObject( const string &nm, CGeometricObject *m, bool ex )
{
    unsigned k;
    string newname = nm;
    map< string, CGeometricObject* >::const_iterator i = geomobjects.begin();
    for (i=geomobjects.begin(); i != geomobjects.end(); i++)
        if ( i->second == m ) return false;
    for (k=0; k<m->GetOwned().size(); k++ )
        AddGeomObject( "", m->GetOwned()[k], ex );
    if ( newname == "") newname = CreateAutoName( m->GetObjectDim() );
    else {
        i = geomobjects.find( newname );
        if ( i != geomobjects.end() ) {
            if ( ex )
            {
                delete m;
                throw CFEException( "Geometric object object '" + newname + "' already exists in model object '" + name + "'" );
            }
            else return false;
        }
    }
    geomobjects[ newname ] = m;
    root->StoreGeometricObject( m, name );
    dim[ m->GetObjectDim() ]++;
    m->SetAttribute( "name", newname );
    return true;
}


CGeometricObjectPoint* CModelObject::FindPoint( const string &nm, bool ex )
{
    CGeometricObject* gobj = FindGeometricObject( nm );
    unsigned k;
    if ( gobj && !gobj->IsPoint() )
    {
       if ( ex ) throw CMathException( "Object :" + nm + "is not a point in model '" + GetName() + "'" );
       else return 0;
    }
    if ( !gobj && ex )  throw CMathException( "Point :" + nm + "not found in model '" + GetName() + "'" );
    return dynamic_cast< CGeometricObjectPoint* >( gobj );
}


CGeometricObject* CModelObject::FindGeometricObject( const string &nm, bool ex )
{
    map< string, CGeometricObject* >::iterator i = geomobjects.find( nm );
    if ( i != geomobjects.end() ) return i->second;
    map< string, CModelObject* >::iterator j;
    for (j = modelobjects.begin(); j != modelobjects.end(); j++ )
    {
        CGeometricObject *ret=j->second->FindGeometricObject( nm, false );
        if ( ret != 0 ) return ret;
    }
    if ( ex ) throw CFEException( "Geometric object object '" + nm + "' not found" );
    return 0;
}


CModelObject* CModelObject::FindModelObject( const string &nm, bool ex  )
{
    map< string, CModelObject* >::iterator i = modelobjects.find( nm );
    if ( i != modelobjects.end() ) return i->second;
    map< string, CModelObject* >::iterator j;
    for (j = modelobjects.begin(); j != modelobjects.end(); j++ )
    {
        CModelObject *ret = j->second->FindModelObject( nm, false );
        if ( ret != 0 ) return ret;
    }
    if ( ex ) throw CFEException( "Model object object '" + nm + "' not found" );
    return 0;
}


string CModelObject::CreateAutoName( unsigned od )
{
    char pr;
    switch ( od ) {
        case 0: pr='p'; break;
        case 1: pr='l'; break;
        case 2: pr='s'; break;
        case 3: pr='v'; break;
    }

    unsigned ind = 0;
    CGeometricObject *ge = 0;
    string name;
    do {
        stringstream ss;
        ss << pr << ind++;
        ge = FindGeometricObject( ss.str(), false );
        name = ss.str();
    } while ( ge );
    return name;
}


void CModelObject::ExtrudeObject( CGeometricObject &ego, const mvector &dx, CSpace *space )
{


}
