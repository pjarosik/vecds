#include "GeometricObjectMorph.h"
#include "Clonable.h"
#include "../MathLib/SpaceLine.h"
#include "../MathLib/GeometricObjectShape.h"

CGeometricObjectMorph::CGeometricObjectMorph( const CGeometricObject *go1, const CGeometricObject *go2  ):CGeometricObject(  )
{

    g1 = go1;
    g2 = go2;
    if ( g1->IsPoint( ) )
    {
        const CGeometricObjectPoint     *pt1 = dynamic_cast< const CGeometricObjectPoint* >( g1 ),
                                        *pt2 = dynamic_cast< const CGeometricObjectPoint* >( g2 );

        sp = new CSpaceLine( pt1->Getx(), pt2->Getx(), const_cast<CSpace*>( g1->GetSpace() )  );
        object_dim = 1;

    }
    else {
        sp = new CSpaceMorph1D( g1->GetSpace(), g2->GetSpace() );
        object_dim = g1->GetObjectDim() + 1;
    }
}


