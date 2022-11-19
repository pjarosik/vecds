// SetOfGaussPoints.cpp: implementation of the CSetOfGaussPoints class.
//
//////////////////////////////////////////////////////////////////////

#include "../MathLib/NCounter.h"
#include "IntegrationManager.h"
#include "GaussPointsRectangular.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CGaussPointsRectangular::~CGaussPointsRectangular()
{

}

 

CGaussPointsRectangular::CGaussPointsRectangular(int dm, int dg):CIntegrationPoints(0,dm,dg)
{
	double val = 0.0;
    int k;
    unsigned l=0;
    CNCounter c( dim , degree + 1 );
	mvector ksi( dim );

#ifdef _DEBUG
    
	assert( degree < MAX_GAUSS_POINTS );
	assert( dim < 4                   );

#endif


    for ( c.Reset();  c.GetIndex() < c.GetSize(); c++ )
    {
		val = 1.0;
		for ( k = 0; k < dim; k++ ) 
		{
            val *= CIntegrationManager::gd[ degree ][ 1 ][  c.GetIndex( k ) ];
            ksi[ k ] = CIntegrationManager::gd[ degree ][ 0 ][  c.GetIndex( k ) ];
		}
		points.push_back( ksi );
		weights.push_back( val );
	}
    ip = c.GetSize();
}
