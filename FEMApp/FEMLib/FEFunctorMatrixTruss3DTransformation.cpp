#include "FEFunctorMatrixTruss3DTransformation.h"


CFEFunctorMatrixTruss3DTransformation::CFEFunctorMatrixTruss3DTransformation(  ):CFEFunctorMatrix("CFEFunctorMatrixTruss3DTransformation")
{
      
    
}

CFEFunctorMatrixTruss3DTransformation::CFEFunctorMatrixTruss3DTransformation( const CFEFunctorMatrixTruss3DTransformation &mf ):CFEFunctorMatrix(mf)
{
    
}


void CFEFunctorMatrixTruss3DTransformation::ManageDOFs( CFiniteElementInstance *fe1 )
{
	
}

void CFEFunctorMatrixTruss3DTransformation::Compute( CFiniteElementInstance *fe1 )
{
	matrix &L = fv;
	L.SetDim( 6, 6 );
	matrix cj( 3, 3 );
    mvector X1,X2;
    fe1->GetNodes()[ 0 ]->GetX( X1 );
    fe1->GetNodes()[ 1 ]->GetX( X2 );
    mvector el = X2 - X1;
    
	double dx  = el[ 0 ], dy = el[ 1 ], dz = el[ 2 ];
    double l1  = sqrt( dx * dx + dy * dy ), l =  fe1->GetNodes()[ 0 ]->Distance( X1 );
    if ( l1 != 0.0 )
    {
		cj(0,0) = dx/l        ; cj(0,1) = dy/l         ;cj(0,2) = dz/l;
		cj(1,0) = -dy/l1      ; cj(1,1) = dx/l1        ;cj(1,2) = 0   ;
		cj(2,0) = -dx/l*dz/l1 ; cj(2,1) = -dz/l*dy/l1  ;cj(2,2) = l1/l;
	}
	else
	{
		cj(0,2) =  dz/l;
		cj(2,0) = -dz/l;
		cj(1,1) = 1.0;
	}
	L.Allocate( 0,0, cj );
	L.Allocate( 3,3, cj );	
}
