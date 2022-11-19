#include "Model.h"
#include "../MathLib/MathException.h"


CModel::CModel( double e ):object( "root", this ),eps(e)
{

}

CGeometricObjectPoint* CModel::FindPoint( const mvector &Xp, bool ex )
{
    unsigned k;
    for ( k = 0; k < GD.GetAllGeometricObjects( ).size(); k++ )
        if ( GD.GetAllGeometricObjects()[ k ]->IsPoint( ) )
        {
            CGeometricObjectPoint *pt = dynamic_cast< CGeometricObjectPoint* >( GD.GetAllGeometricObjects( )[ k ] );
            if ( pt->IsApproximatellyEqual( Xp, eps ) ) return pt;
        }
    if ( ex )  throw CMathException( "Point :" + Xp.toString() + "not found in model '" + object.GetName() + "'" );
    return 0;
}

CGeometricObject* CModel::FindGeometricObject( const string &nm, bool ex )
{
    CGeometricObject *obj = object.FindGeometricObject( nm, false );
    if ( obj==0 && ex ) throw CMathException( "Object :" + nm + "not found in model '" + object.GetName() + "'" );
    return obj;
}

CModelObject* CModel::FindModelObject( const string &nm, bool ex )
{
    CModelObject *obj = object.FindModelObject( nm, false );
    if ( obj==0 && ex ) throw CMathException( "Model object :" + nm + "not found in model '" + object.GetName() + "'" );
    return obj;
}


void CModel::DuplicateGeometricObject( CGeometricObject *go, const string &dn )
{

}
