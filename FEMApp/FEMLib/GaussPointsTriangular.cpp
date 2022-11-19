
#include "GaussPointsTriangular.h"

CGaussPointsTriangular::CGaussPointsTriangular(int dm, int dg):CIntegrationPoints(1,dm,dg)
{

/*
	type   = 1;
	dim    = dm;
	degree = dg;
*/
    mvector ksi( 3 );

	double a1 = 0.0597158717, b1 = 0.4701420641,a2 = 0.7974269853, b2=0.1012865073;

#ifdef _DEBUG

	assert( degree < MAX_GAUSS_POINTS );
	assert( dim < 4                   );

#endif
	if ( degree == 1 )
		{
            ksi.SetDim( 2 );
			ksi[ 0 ] = 1.0 / 3.0;
			ksi[ 1 ] = 1.0 / 3.0;
            // ksi[ 2 ] = 1.0 / 3.0;

			points.push_back( ksi );
			weights.push_back( 0.5 );

			ip     = 1;
			degree = 1;
			dim    = 2;
		}
	if ( degree == 7 )
		{
			ksi[ 0 ] = 1.0 / 3.0;  ksi[ 1 ] = 1.0 / 3.0; ksi[ 2 ] = 1.0 / 3.0; weights.push_back( 0.225 );	points.push_back( ksi );
			ksi[ 0 ] = a1;  ksi[ 1 ] = b1; ksi[ 2 ] = b1; weights.push_back(  0.1323941527 );	points.push_back( ksi );
			ksi[ 0 ] = b1;  ksi[ 1 ] = a1; ksi[ 2 ] = b1; weights.push_back(  0.1323941527 );	points.push_back( ksi );
			ksi[ 0 ] = b1;  ksi[ 1 ] = b1; ksi[ 2 ] = a1; weights.push_back(  0.1323941527 );	points.push_back( ksi );
			ksi[ 0 ] = a2;  ksi[ 1 ] = b2; ksi[ 2 ] = b2; weights.push_back( 0.1259391805 );	points.push_back( ksi );
			ksi[ 0 ] = b2;  ksi[ 1 ] = a2; ksi[ 2 ] = b2; weights.push_back( 0.1259391805 );	points.push_back( ksi );
			ksi[ 0 ] = b2;  ksi[ 1 ] = b2; ksi[ 2 ] = a2; weights.push_back( 0.1259391805 );	points.push_back( ksi );

			ip     = 7;
			degree = 7;
			dim    = 2;
		}

	if ( degree == 13 )
		{
			ksi[ 0 ] = 0.8697397942;  ksi[ 1 ] = 0.0651301029; ksi[ 2 ] = 0.0651301029; weights.push_back(0.0533472356088 );	points.push_back( ksi );
			ksi[ 0 ] = 0.0651301029;  ksi[ 1 ] = 0.8697397942; ksi[ 2 ] = 0.0651301029; weights.push_back(0.0533472356088 );	points.push_back( ksi );
			ksi[ 0 ] = 0.0651301029;  ksi[ 1 ] = 0.0651301029; ksi[ 2 ] = 0.8697397942; weights.push_back(0.0533472356088 );	points.push_back( ksi );
			ksi[ 0 ] = 0.6384441886;  ksi[ 1 ] = 0.312865496 ; ksi[ 2 ] = 0.0486903154; weights.push_back(0.0771137608903 );	points.push_back( ksi );
			ksi[ 0 ] = 0.0486903154;  ksi[ 1 ] = 0.6384441886; ksi[ 2 ] = 0.312865496;  weights.push_back(0.0771137608903 );	points.push_back( ksi );
			ksi[ 0 ] = 0.312865496 ;  ksi[ 1 ] = 0.0486903154; ksi[ 2 ] = 0.6384441886; weights.push_back(0.0771137608903 );	points.push_back( ksi );
			ksi[ 0 ] = 0.312865496 ;  ksi[ 1 ] = 0.6384441886; ksi[ 2 ] = 0.0486903154; weights.push_back(0.0771137608903 );	points.push_back( ksi );
			ksi[ 0 ] = 0.0486903154;  ksi[ 1 ] = 0.312865496 ; ksi[ 2 ] = 0.6384441886; weights.push_back(0.0771137608903 );	points.push_back( ksi );
			ksi[ 0 ] = 0.6384441886;  ksi[ 1 ] = 0.0486903154; ksi[ 2 ] = 0.312865496;  weights.push_back(0.0771137608903 );	points.push_back( ksi );
			ksi[ 0 ] = 0.4793080678;  ksi[ 1 ] = 0.2603459661; ksi[ 2 ] = 0.2603459661; weights.push_back(0.1756152574332 );	points.push_back( ksi );
			ksi[ 0 ] = 0.2603459661;  ksi[ 1 ] = 0.4793080678; ksi[ 2 ] = 0.2603459661; weights.push_back(0.1756152574332 );	points.push_back( ksi );
			ksi[ 0 ] = 0.2603459661;  ksi[ 1 ] = 0.2603459661; ksi[ 2 ] = 0.4793080678; weights.push_back(0.1756152574332 );	points.push_back( ksi );
			ksi[ 0 ] = 0.3333333333;  ksi[ 1 ] = 0.3333333333; ksi[ 2 ] = 0.3333333333; weights.push_back(0.1495700444677 );	points.push_back( ksi );

			ip     = 13;
			degree = 13;
			dim    = 2;
		}
//	if ( dim < 3 ) for( k = 0;  k < ip; k++ ) points[ k ][ 2 ] = 0.0;
}

CGaussPointsTriangular::~CGaussPointsTriangular(void)
{
}
