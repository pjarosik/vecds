// IntegrationManager.h: interface for the CIntegrationManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTEGRATIONMANAGER_H__54A2BE9A_8F8A_4727_BCEF_D2AA1C8997E2__INCLUDED_)
#define AFX_INTEGRATIONMANAGER_H__54A2BE9A_8F8A_4727_BCEF_D2AA1C8997E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../MathLib/matrix.h"
#include "../MathLib/constants.h"
#include <vector>
using namespace std;

class CIntegrationPoints;

class CIntegrationManager  
{

public: 
	
	const CIntegrationPoints* GetIntegrationPoints( int t, int dim, int degree ) const;
	static CIntegrationPoints* FindIntegrationPoints( int t, int dim, int degree );
	
	static double gd[ MAX_GAUSS_NODES ][ 2 ][ MAX_GAUSS_NODES ];

	void ChangeInterval( matrix &gw, double x1, double x2 );
	CIntegrationManager( ); 
	virtual ~CIntegrationManager( );

protected:

	CIntegrationPoints* rp[ 3 ][ 4 ], *tp[ 3 ][ 4 ];

	static vector< CIntegrationPoints* > GP;

};

#endif // !defined(AFX_INTEGRATIONMANAGER_H__54A2BE9A_8F8A_4727_BCEF_D2AA1C8997E2__INCLUDED_)
