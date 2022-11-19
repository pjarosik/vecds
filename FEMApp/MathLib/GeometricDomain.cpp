#include "GeometricDomain.h"
#include "GeometricObject.h"
#include "GeometricObjectMorph.h"

CGeometricObject* CGeometricDomain::FindApproximatelyEqual( const CGeometricObject *obj ) const
{
    unsigned k;
    for (k=0; k<GO.GetAll().size(); k++ )
        if ( obj->IsApproximatellyEqual( *GO.GetAll()[ k ], eps ) ) return GO.GetAll()[ k ];

    return 0;
}


void CGeometricDomain::ComputeBoundaryStat()
{
    unsigned n0=0,b0=0,n1=0,b1=0,n2=0,b2=0,k=0;
    for( k=0; k<GO.GetAll().size(); k++)
    {
        switch ( GO.GetAll()[k]->GetObjectDim() )
        {
            case 0: n0++; if ( GO.GetAll()[k]->IsBoundary() ) b0++;   break;
            case 1: n1++; if ( GO.GetAll()[k]->IsBoundary() ) b1++;   break;
            case 2: n2++; if ( GO.GetAll()[k]->IsBoundary() ) b2++;   break;

        }
    }

    int a=0;
}

unsigned CGeometricDomain::DetermineSpaceDimension() const
{
    unsigned dim = 0, k;
    for( k=0; k<GO.GetAll().size(); k++)
        if ( GO.GetAll()[k]->GetSpaceDim() > dim ) dim = GO.GetAll()[k]->GetSpaceDim();
    return dim;
}

void CGeometricDomain::EnumerateObjects()
{
    unsigned n0=1,n1=1,n2=1,n3=1,k;
    for( k=0; k<GO.GetAll().size(); k++)
    {
        switch ( GO.GetAll()[k]->GetObjectDim() )
        {
            case 0: GO.GetAll()[k]->SetNumber( n0++ );   break;
            case 1: GO.GetAll()[k]->SetNumber( n1++ );   break;
            case 2: GO.GetAll()[k]->SetNumber( n2++ );   break;
            case 3: GO.GetAll()[k]->SetNumber( n3++ );   break;
        }
    }
    int a = 0;
}

void CGeometricDomain::PrintConnections()
{
    unsigned n0=0,b0=0,n1=0,b1=0,n2=0,b2=0,k=0;
    ofstream fout("connections.txt");
    const string mrg("    ");
    EnumerateObjects();
    for( k=0; k<GO.GetAll().size(); k++)
    {
        CGeometricObject *object = GO.GetAll()[k];
        if ( object->IsVolume() )
            object->Print(mrg,fout);
    }
    for( k=0; k<GO.GetAll().size(); k++)
    {
        CGeometricObject *object = GO.GetAll()[k];
        if ( object->IsPoint() )
            object->Print(mrg,fout);
    }
    int a = 0;
}


