// IntegrationManager.cpp: implementation of the CIntegrationManager class.
//
//////////////////////////////////////////////////////////////////////

#include "IntegrationManager.h"
#include "IntegrationPoints.h"	// Added by ClassView
#include "GaussPointsRectangular.h"
#include "GaussPointsTriangular.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


double CIntegrationManager::gd[ MAX_GAUSS_NODES ][ 2 ][ MAX_GAUSS_NODES ];
vector< CIntegrationPoints* > CIntegrationManager::GP;

CIntegrationManager::CIntegrationManager()
{
	gd[0][ 0 ][ 0 ] = 0.0;                gd[0][ 1 ][ 0 ] = 2.0;

    gd[1][ 0 ][ 0 ] = -0.577350269189626; gd[1][ 1 ][ 0 ] = 1.0;
    gd[1][ 0 ][ 1 ] =  0.577350269189626; gd[1][ 1 ][ 1 ] = 1.0;
						 

    gd[2][0][0] = -0.774596669241483;	  gd[2][1][0] = 5.0/9.0;
    gd[2][0][1] =  0.0;                   gd[2][1][1] = 8.0/9.0;
    gd[2][0][2] =  0.774596669241483;     gd[2][1][2] = 5.0/9.0;


    gd[3][0][0] = -0.861136311594053; gd[3][1][0] = 0.347854845137454;
    gd[3][0][1] = -0.339981043584856; gd[3][1][1] = 0.652145154862546;
    gd[3][0][2] =  0.339981043584856; gd[3][1][2] = 0.652145154862546;
    gd[3][0][3] =  0.861136311594053; gd[3][1][3] = 0.347854845137454;

    gd[4][0][0] =-0.906179845938664;  gd[4][1][0] = 0.236926885056189;
    gd[4][0][1] =-0.538469310105683;  gd[4][1][1] = 0.478628670499366;
    gd[4][0][2] = 0.0;                gd[4][1][2] = 0.568888888888889;
    gd[4][0][3] = 0.538469310105683;  gd[4][1][3] = 0.478628670499366;
    gd[4][0][4] = 0.906179845938664;  gd[4][1][4] = 0.236926885056189;

    gd[5][0][0] =-0.932469514203152;  gd[5][1][0] = 0.171324492379170;
    gd[5][0][1] =-0.661209386466265;  gd[5][1][1] = 0.360761573048139;
    gd[5][0][2] =-0.238619186083197;  gd[5][1][2] = 0.467913934572691;
    gd[5][0][3] = 0.238619186083197;  gd[5][1][3] = 0.467913934572691;
    gd[5][0][4] = 0.661209386466265;  gd[5][1][4] = 0.360761573048139;
    gd[5][0][5] = 0.932469514203152;  gd[5][1][5] = 0.171324492379170;
 
    gd[6][0][0] =-0.949107912342759; gd[6][1][0] = 0.129484966168870;
    gd[6][0][1] =-0.741531185599394; gd[6][1][1] = 0.279705391489277;
    gd[6][0][2] =-0.405845151377397; gd[6][1][2] = 0.381830050505119;
    gd[6][0][3] = 0.0;               gd[6][1][3] = 0.417959183673469;
    gd[6][0][4] = 0.405845151377397; gd[6][1][4] = 0.381830050505119;
    gd[6][0][5] = 0.741531185599394; gd[6][1][5] = 0.279705391489277;
    gd[6][0][6] = 0.949107912342759; gd[6][1][6] = 0.129484966168870;


    gd[7][0][0] =-0.960289856497536; gd[7][1][0] = 0.101228536290376;
    gd[7][0][1] =-0.796666477413627; gd[7][1][1] = 0.222381034453374;
    gd[7][0][2] =-0.525332409916329; gd[7][1][2] = 0.313706645877887;
    gd[7][0][3] =-0.183434642495650; gd[7][1][3] = 0.362683783378362;
    gd[7][0][4] = 0.183434642495650; gd[7][1][4] = 0.362683783378362;
    gd[7][0][5] = 0.525332409916329; gd[7][1][5] = 0.313706645877887;
    gd[7][0][6] = 0.796666477413627; gd[7][1][6] = 0.222381034453374;
    gd[7][0][7] = 0.960289856497536; gd[7][1][7] = 0.101228536290376;


    gd[8][0][0] =-0.968160239507626; gd[8][1][0] = 0.081274388361574;
    gd[8][0][1] =-0.836031107326636; gd[8][1][1] = 0.180648160694857;
    gd[8][0][2] =-0.613371432700590; gd[8][1][2] = 0.260610699402935;
    gd[8][0][3] =-0.324253423403809; gd[8][1][3] = 0.312347077040003;
    gd[8][0][4] = 0.0;               gd[8][1][4] = 0.330239355001260;
    gd[8][0][5] = 0.324253423403809; gd[8][1][5] = 0.312347077040003;
    gd[8][0][6] = 0.613371432700590; gd[8][1][6] = 0.260610699402935;
    gd[8][0][7] = 0.836031107326636; gd[8][1][7] = 0.180648160694857;
    gd[8][0][8] = 0.968160239507626; gd[8][1][8] = 0.081274388361574;


    gd[9][0][0] = -0.973906528517172; gd[9][1][0] = 0.066671344308688;
    gd[9][0][1] = -0.865063366688985; gd[9][1][1] = 0.149451349150581;
    gd[9][0][2] = -0.679409568299024; gd[9][1][2] = 0.219086362515982;
    gd[9][0][3] = -0.433395394129247; gd[9][1][3] = 0.269266719309996;
    gd[9][0][4] = -0.148874338981631; gd[9][1][4] = 0.295524224714753;
    gd[9][0][5] =  0.148874338981631; gd[9][1][5] = 0.295524224714753;
    gd[9][0][6] =  0.433395394129247; gd[9][1][6] = 0.269266719309996;
    gd[9][0][7] =  0.679409568299024; gd[9][1][7] = 0.219086362515982;
    gd[9][0][8] =  0.865063366688985; gd[9][1][8] = 0.149451349150581;
    gd[9][0][9] =  0.973906528517172; gd[9][1][9] = 0.066671344308688;
				
	gd[10][0][0 ] =-0.978228658146057; gd[10][1][0 ] = 0.055668567116174;
    gd[10][0][1 ] =-0.887062599768095; gd[10][1][1 ] = 0.125580369464905;
    gd[10][0][2 ] =-0.730152005574049; gd[10][1][2 ] = 0.186290210927734;
    gd[10][0][3 ] =-0.519096129206812; gd[10][1][3 ] = 0.233193764591990;
    gd[10][0][4 ] =-0.269543155952345; gd[10][1][4 ] = 0.262804544510247;
    gd[10][0][5 ] =-0.000000000000000; gd[10][1][5 ] = 0.272925086777901;
    gd[10][0][6 ] = 0.269543155952345; gd[10][1][6 ] = 0.262804544510247;
    gd[10][0][7 ] = 0.519096129206812; gd[10][1][7 ] = 0.233193764591990;
    gd[10][0][8 ] = 0.730152005574049; gd[10][1][8 ] = 0.186290210927734;
    gd[10][0][9 ] = 0.887062599768095; gd[10][1][9 ] = 0.125580369464905;
	gd[10][0][10] = 0.978228658146057; gd[10][1][10] = 0.055668567116174;


	gd[11][0][0 ] = -0.981560634246719; gd[11][1][0 ] = 0.047175336386512;
    gd[11][0][1 ] =-0.904117256370475;  gd[11][1][1 ] = 0.106939325995318;
    gd[11][0][2 ] =-0.769902674194305;  gd[11][1][2 ] = 0.160078328543346;
    gd[11][0][3 ] =-0.587317954286617;  gd[11][1][3 ] = 0.203167426723066;
    gd[11][0][4 ] =-0.367831498998180;  gd[11][1][4 ] = 0.233492536538355;
    gd[11][0][5 ] =-0.125233408511469;  gd[11][1][5 ] = 0.249147045813403;
    gd[11][0][6 ] = 0.125233408511469;  gd[11][1][6 ] = 0.249147045813403;
    gd[11][0][7 ] = 0.367831498998180;  gd[11][1][7 ] = 0.233492536538355;
    gd[11][0][8 ] = 0.587317954286617;  gd[11][1][8 ] = 0.203167426723066;
    gd[11][0][9 ] = 0.769902674194305;  gd[11][1][9 ] = 0.160078328543346;
	gd[11][0][10] = 0.904117256370475;  gd[11][1][10] = 0.106939325995318;
    gd[11][0][11] = 0.981560634246719;  gd[11][1][11] = 0.047175336386512;

    unsigned idim, idegree;
    for (idim=0; idim<3; idim++)
        for (idegree=0; idegree<4; idegree++)
		{
            rp[ idim ][ idegree ] = new CGaussPointsRectangular( idim + 1, idegree + 1  );
            tp[ idim ][ idegree ] = new CGaussPointsTriangular( idim + 1, idegree + 1 );
		}

}

CIntegrationManager::~CIntegrationManager()
{
	unsigned k,l;
	for (k=0; k<2; k++)
		for (l=0; k<l; l++)
		{
			delete rp[ k ][ l ];
			delete tp[ k ][ l ];
		}
}

void CIntegrationManager::ChangeInterval( matrix &gw, double x1, double x2 )
{
	
/*      double wsp = ( x2 - x1 ) / 2.0;
        int n, m, k;
        gw.Rozmiar( m, n );
        for (k=0; k<n; k++)
        {
                gw( 0, k ) = wsp * gw( 0, k ) + x1 + wsp;
                gw( 1, k ) = gw( 1, k ) * wsp;
        } */
}

CIntegrationPoints* CIntegrationManager::FindIntegrationPoints( int t, int dim, int degree )
{
	unsigned k;
	for (k=0; k<GP.size(); k++)
		if ( GP[k]->Check( t, dim, degree ) ) return GP[ k ];
	return 0;
}

const CIntegrationPoints* CIntegrationManager::GetIntegrationPoints( int t, int dim, int degree ) const
{
//	CIntegrationPoints *gps = FindIntegrationPoints( t, dim, degree );
//	if ( !gps ) 
//	{ 
		switch( t )
		{
            case 0 : return rp[ dim - 1 ][ degree - 1 ];
            case 1 : return tp[ dim - 1 ][ degree - 1 ];
		}
//		GP.push_back( gps );
//	}
//	return gps;
	return 0;
}
