
#include "IntegrationPointsMultiTriangular.h"
#include "FiniteElementInstance2D.h"
#include <iomanip>

CIntegrationPointsMultiTriangular::CIntegrationPointsMultiTriangular( int dm, int dg):CIntegrationPoints(1,dm,dg)
{
	mvector ksi( 2);

	double a1 = 0.0597158717, b1 = 0.4701420641,a2 = 0.7974269853, b2=0.1012865073;

#ifdef _DEBUG

	assert( degree < MAX_GAUSS_POINTS );
	assert( dim < 4                   );

#endif
	if ( degree == 1 )
		{
			ksi[ 0 ] = 1.0 / 3.0;
			ksi[ 1 ] = 1.0 / 3.0;
			ksi[ 2 ] = 1.0 / 3.0;

			t_points.push_back( ksi );
			t_weights.push_back( 0.5 );

			ip     = 1;
			degree = 1;
			dim    = 2;
		}
	if ( degree == 7 )
		{
			ksi[ 0 ] = 1.0 / 3.0;  ksi[ 1 ] = 1.0 / 3.0;  t_weights.push_back( 0.225 );	t_points.push_back( ksi );
			ksi[ 0 ] = a1;  ksi[ 1 ] = b1;  t_weights.push_back(  0.1323941527 );	t_points.push_back( ksi );
			ksi[ 0 ] = b1;  ksi[ 1 ] = a1;  t_weights.push_back(  0.1323941527 );	t_points.push_back( ksi );
			ksi[ 0 ] = b1;  ksi[ 1 ] = b1;  t_weights.push_back(  0.1323941527 );	t_points.push_back( ksi );
			ksi[ 0 ] = a2;  ksi[ 1 ] = b2;  t_weights.push_back( 0.1259391805 );	t_points.push_back( ksi );
			ksi[ 0 ] = b2;  ksi[ 1 ] = a2;  t_weights.push_back( 0.1259391805 );	t_points.push_back( ksi );
			ksi[ 0 ] = b2;  ksi[ 1 ] = b2;  t_weights.push_back( 0.1259391805 );	t_points.push_back( ksi );

			ip     = 7;
			degree = 7;
			dim    = 2;
		}

	if ( degree == 13 )
		{

			ksi[ 0 ] = 0.8697397942;  ksi[ 1 ] = 0.0651301029; t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0651301029;  ksi[ 1 ] = 0.8697397942; t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0651301029;  ksi[ 1 ] = 0.0651301029; t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.6384441886;  ksi[ 1 ] = 0.312865496 ; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0486903154;  ksi[ 1 ] = 0.6384441886; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.312865496 ;  ksi[ 1 ] = 0.0486903154; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.312865496 ;  ksi[ 1 ] = 0.6384441886; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0486903154;  ksi[ 1 ] = 0.312865496 ; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.6384441886;  ksi[ 1 ] = 0.0486903154; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.4793080678;  ksi[ 1 ] = 0.2603459661; t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.2603459661;  ksi[ 1 ] = 0.4793080678; t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.2603459661;  ksi[ 1 ] = 0.2603459661; t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.3333333333;  ksi[ 1 ] = 0.3333333333; t_weights.push_back(-0.1495700444677 );	t_points.push_back( ksi );


			/*
			ksi[ 0 ] = 0.0651301029022;  ksi[ 1 ] = 0.0651301029022; 	  t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.8697397941956;  ksi[ 1 ] = 0.0651301029022; 	  t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0651301029022;  ksi[ 1 ] = 0.8697397941956; 	  t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.3128654960049;  ksi[ 1 ] = 0.0486903154253; 	  t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.6384441885698;  ksi[ 1 ] = 0.3128654960049; 	  t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0486903154253;  ksi[ 1 ] = 0.6384441885698; 	  t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.6384441885698;  ksi[ 1 ] = 0.0486903154253; 	  t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.3128654960049;  ksi[ 1 ] = 0.6384441885698; 	  t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0486903154253;  ksi[ 1 ] = 0.3128654960049; 	  t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.2603459660790;  ksi[ 1 ] = 0.2603459660790; 	  t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.4793080678419;  ksi[ 1 ] = 0.2603459660790; 	  t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.2603459660790;  ksi[ 1 ] = 0.4793080678419; 	  t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.333333333333 ;  ksi[ 1 ] = 0.4793080678419; 	  t_weights.push_back(-0.1495700444677 );	t_points.push_back( ksi );
*/

			/*
			ksi[ 0 ] = 0.8697397942;  ksi[ 1 ] = 0.0651301029; ksi[ 2 ] = 0.0651301029; t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0651301029;  ksi[ 1 ] = 0.8697397942; ksi[ 2 ] = 0.0651301029; t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0651301029;  ksi[ 1 ] = 0.0651301029; ksi[ 2 ] = 0.8697397942; t_weights.push_back(0.0533472356088 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.6384441886;  ksi[ 1 ] = 0.312865496 ; ksi[ 2 ] = 0.0486903154; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0486903154;  ksi[ 1 ] = 0.6384441886; ksi[ 2 ] = 0.312865496;  t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.312865496 ;  ksi[ 1 ] = 0.0486903154; ksi[ 2 ] = 0.6384441886; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.312865496 ;  ksi[ 1 ] = 0.6384441886; ksi[ 2 ] = 0.0486903154; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.0486903154;  ksi[ 1 ] = 0.312865496 ; ksi[ 2 ] = 0.6384441886; t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.6384441886;  ksi[ 1 ] = 0.0486903154; ksi[ 2 ] = 0.312865496;  t_weights.push_back(0.0771137608903 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.4793080678;  ksi[ 1 ] = 0.2603459661; ksi[ 2 ] = 0.2603459661; t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.2603459661;  ksi[ 1 ] = 0.4793080678; ksi[ 2 ] = 0.2603459661; t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.2603459661;  ksi[ 1 ] = 0.2603459661; ksi[ 2 ] = 0.4793080678; t_weights.push_back(0.1756152574332 );	t_points.push_back( ksi );
			ksi[ 0 ] = 0.3333333333;  ksi[ 1 ] = 0.3333333333; ksi[ 2 ] = 0.3333333333; t_weights.push_back(-0.1495700444677 );	t_points.push_back( ksi );
*/
			ip     = 13;
			degree = 13;
			dim    = 2;
		}

}
/*
  0.0651301029022; 		  0.0651301029022;   = 0.0651301029022; 		  = 0.0651301029022;  0.0533472356088, mode);
  0.8697397941956; 		  0.8697397941956;   = 0.0651301029022; 		  = 0.0651301029022; 0.0533472356088, mode);
  0.0651301029022; 		  0.0651301029022;   = 0.8697397941956; 		  = 0.8697397941956; 0.0533472356088, mode);
  0.3128654960049; 		  0.3128654960049;   = 0.0486903154253; 		  = 0.0486903154253; 0.0771137608903, mode);
  0.6384441885698; 		  0.6384441885698;   = 0.3128654960049; 		  = 0.3128654960049; 0.0771137608903, mode);
  0.0486903154253; 		  0.0486903154253;   = 0.6384441885698; 		  = 0.6384441885698; 0.0771137608903, mode);
  0.6384441885698; 		  0.6384441885698;   = 0.0486903154253; 		  = 0.0486903154253; 0.0771137608903, mode);
  0.3128654960049; 		  0.3128654960049;   = 0.6384441885698; 		  = 0.6384441885698; 0.0771137608903, mode);
  0.0486903154253; 		  0.0486903154253;   = 0.3128654960049; 		  = 0.3128654960049; 0.0771137608903, mode);
  0.2603459660790; 		  0.2603459660790;   = 0.2603459660790; 		  = 0.2603459660790; 0.1756152576332, mode);
  0.4793080678419; 		  0.4793080678419;   = 0.2603459660790; 		  = 0.2603459660790; 0.1756152576332, mode);
  0.2603459660790; 		  0.2603459660790;   = 0.4793080678419; 		  = 0.4793080678419 0.1756152576332, mode);
  0.333333333333 ; 		  0.333333333333;    = 0.4793080678419; 		  = 0.4793080678419; -0.1495700444677, mode);
*/

CIntegrationPointsMultiTriangular::~CIntegrationPointsMultiTriangular( void )
{

}

void CIntegrationPointsMultiTriangular::SetFE( CFiniteElementInstance *f )
{
	CFiniteElementInstance2D *f2D = (CFiniteElementInstance2D*)f;
	const vector< CTriangularSubelement > &subelems = f2D->GetSubElems();

	unsigned k,l;
	mvector tX(2);
	matrix tJ(2,2);

	points.clear();
	weights.clear();

	ofstream fout("gauss_points.txt");

	for (k=0; k<t_points.size(); k++)
		fout << setw(15) <<  t_points[k][0] << setw(15) << t_points[k][1] << setw(15) << endl;

	fout << endl;

	for (k=0; k<subelems.size(); k++)
	{
		for (l=0; l<t_weights.size(); l++)
		{
					subelems[ k ].GetJacobyMatrix( t_points[l], tJ );
					subelems[ k ].GetKsi( t_points[l], tX );
							fout << setw(15) <<  tX[0] << setw(15) << tX[1] << setw(15) << endl;
					points.push_back( tX );
					weights.push_back( 0.5 * t_weights[l] * fabs( tJ.Det() ) );
		}

	}
	fout.close();
	ip = weights.size();
}
