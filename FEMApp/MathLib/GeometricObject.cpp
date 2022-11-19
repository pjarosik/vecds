#include "GeometricObject.h"
#include "GeometricDomain.h"
#include "GeometricObjectPoint.h"

CGeometricObject::CGeometricObject( ):sp( 0 ), is_boundary( false ), object_dim( 0 ),id_number(0)
{

}



CGeometricObject::CGeometricObject( const CGeometricObject& go ):owns( go.owns ), ownedby( go.ownedby ),subelems( go.subelems ),props( go.props ), object_dim( go.object_dim ),
                is_boundary( false ), isselected( go.isselected ), sp( go.sp )
{


}

/*CGeometricObject::CGeometricObject( CCopyer& cpr, const CGeometricObject& go ):own( go.own ), isownedby( go.isownedby ),subelems( go.subelems ),props( go.props ), object_dim( go.object_dim ),
    is_boundary( false ), isselected( go.isselected ), sp( go.sp )
{
    unsigned k;
    for(  k = 0; k < own.size(); k++ )
        own[ k ] = cpr.Copy( own[ k ] );

    for(  k = 0; k < isownedby.size(); k++ )
        isownedby[ k ] = cpr.Copy( isownedby[ k ] );

    sp = cpr.Copy( sp );
}*/

bool CGeometricObject::IsApproximatellyEqual( const CGeometricObject &cp, double eps ) const
{
    if ( owns.size() != cp.GetOwned().size() ) return false;

    unsigned k,l;
    for (k=0; k<owns.size(); k++)
    {
        bool isequal = false;
        for (l=0; l<GetOwned().size(); l++)
            if ( !owns[ k ]->IsApproximatellyEqual( *cp.GetOwned()[l], eps ) ) isequal = true;
        if ( !isequal ) return false;
    }
    return true;
}


 void CGeometricObject::FindBoundaryObjects()
 {
     unsigned k;
     for(  k = 0; k < owns.size(); k++ )
         if ( owns[ k ]->GetObjectDim() == GetObjectDim() - 1 && owns[ k ]->HasSingleOwner() )
         {
             int dm = owns[ k ]->GetObjectDim();
             owns[ k ]->SetBoundary( true );
         }
 }

 bool CGeometricObject::ContainsPoints( const vector< CGeometricObjectPoint *> &op ) const
 {
    unsigned k,nd0=0;
    for(  k = 0; k < owns.size(); k++ )
    {
        if ( owns[ k ]->IsPoint() )
        {
            nd0++;
            if ( find( op.begin(), op.end(), owns[ k ] ) == op.end() ) return false;
        }
    }
    if ( nd0 != op.size() ) return false;
    return true;
 }

 CGeometricObject* CGeometricObject::FindEqualOwner( unsigned d, const vector< CGeometricObjectPoint *> &objectspoints ) const
 {
     unsigned k;
     for(  k = 0; k < ownedby.size(); k++ )
         if ( d == ownedby[k]->GetObjectDim() && ownedby[ k ]->ContainsPoints( objectspoints ) ) return ownedby[ k ];
     return 0;
 }

void CGeometricObject::Print( const string &marg, ofstream &fout )const
{
    const string m2="    ";
    vector< CGeometricObject* > points,edges,faces, volumes;

    if ( owns.size()==0 && ownedby.size()==0 ) return;

    unsigned k;
    for( k=0; k<owns.size(); k++)
    {
        CGeometricObject *object = owns[ k ];
        if ( object->IsPoint() )    points.push_back( owns[ k ] );
        if ( object->IsLine() )     edges.push_back( owns[ k ] );
        if ( object->IsSurface() )  faces.push_back( owns[ k ] );
        if ( object->IsVolume() )  volumes.push_back( owns[ k ] );

    }


    switch ( GetObjectDim() ) {
        case 0:   fout << marg << "Point " << GetNumber() << " {";  break;
        case 1:   fout << marg << "Edge " << GetNumber() << " {";  break;
        case 2:   fout << marg << "Face " << GetNumber() << " {";  break;
        case 3:   fout << marg << "Volume " << GetNumber() << " {";  break;
    }

    fout << endl;

    if ( points.size() ) { fout << marg + m2 << "points :"; for( k=0; k<points.size(); k++) fout << points[k]->GetNumber() << ","; fout << endl; }
    if (  edges.size() ) { fout << marg + m2 << "edges :";  for( k=0; k<edges.size(); k++) fout << edges[k]->GetNumber() << ","; fout << endl;   }
    if (  faces.size() ) { fout << marg + m2 << "faces :";  for( k=0; k<faces.size(); k++) fout << faces[k]->GetNumber() << ","; fout << endl;   }

    for( k=0; k<edges.size(); k++) edges[ k ]->Print( marg + m2, fout );
    for( k=0; k<faces.size(); k++) faces[ k ]->Print( marg + m2, fout );

    points.clear();
    edges.clear();
    faces.clear();
    volumes.clear();

    for( k=0; k<ownedby.size(); k++)
    {
        CGeometricObject *object = ownedby[ k ];
        if ( object->IsVolume() )    volumes.push_back( object );
        if ( object->IsLine() )     edges.push_back( object );
        if ( object->IsSurface() )  faces.push_back( object );

    }
    if (  edges.size() )   { fout << marg + m2 << "is owned by edges :";        for( k=0; k<edges.size(); k++) fout << edges[ k ]->GetNumber() << ",";      fout << endl;   }
    if (  faces.size() )   { fout << marg + m2 << "is owned by faces :";        for( k=0; k<faces.size(); k++) fout << faces[ k ]->GetNumber() << ",";      fout << endl;   }
    if (  volumes.size() ) { fout << marg + m2 << "is owned by volumes :";      for( k=0; k<volumes.size(); k++) fout << volumes[ k ]->GetNumber() << ",";  fout << endl;   }

    fout << marg << "       " <<  "     }"; fout << endl;
}


void CGeometricObject::GetObjects( vector< CGeometricObject* > &obj, unsigned d ) const
{
    vector< CGeometricObject* >::const_iterator i;
    for ( i = owns.begin(); i!=owns.end(); i++ )
        if ( (*i)->GetObjectDim() == d ) obj.push_back( *i );
}
