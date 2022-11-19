

#define _USE_MATH_DEFINES
#include <cmath>


#include "EnrichmentFunctionsBimaterial.h"
#include <float.h>


CEnrichmentFunctionsBimaterial::CEnrichmentFunctionsBimaterial( const mvector &x, double e, double a ):ep(e),alpha(a),xs(x),pspace(x,a)
{

}

CEnrichmentFunctionsBimaterial::~CEnrichmentFunctionsBimaterial(void)
{
}

void CEnrichmentFunctionsBimaterial::GetValues( const mvector &X , mvector &N ) const
{
	mvector rt(2);
	transform(X,rt);  
	double r=rt[0],t=rt[1];

	N.SetDim( 12 );

	N[0]  = sqrt(r) * cos(ep*log10(r)) * exp(-(ep*t)) * sin(t/2.0);
	N[1]  = sqrt(r) * cos(ep*log10(r)) * exp(-(ep*t)) * cos(t/2.0);
	N[2]  = sqrt(r) * cos(ep*log10(r)) * exp((ep*t))  * sin(t/2.0);
	N[3]  = sqrt(r) * cos(ep*log10(r)) * exp((ep*t))  * cos(t/2.0);
	N[4]  = sqrt(r) * cos(ep*log10(r)) * exp((ep*t))  * sin(t/2.0)*sin(t);
	N[5]  = sqrt(r) * cos(ep*log10(r)) * exp((ep*t))  * cos(t/2.0)*sin(t);
	N[6]  = sqrt(r) * sin(ep*log10(r)) * exp(-(ep*t)) * sin(t/2.0);
	N[7]  = sqrt(r) * sin(ep*log10(r)) * exp(-(ep*t)) * cos(t/2.0);
	N[8]  = sqrt(r) * sin(ep*log10(r)) * exp((ep*t))  * sin(t/2.0);
	N[9]  = sqrt(r) * sin(ep*log10(r)) * exp((ep*t))  * cos(t/2.0);
	N[10] = sqrt(r) * sin(ep*log10(r)) * exp((ep*t))  * sin(t/2.0)*sin(t);
	N[11] = sqrt(r) * sin(ep*log10(r)) * exp((ep*t))  * cos(t/2.0)*sin(t);

	#ifdef _DEBUG
	for( unsigned k=0; k<12; k++)
        if ( !finite( N[k] ) )
		{
			double x=X[0],y=X[1];
			double a=1;
			assert(false);
		}
	#endif

}

void CEnrichmentFunctionsBimaterial::GetGradients( const mvector &X,  matrix &dN ) const
{
	mvector rt(2);
	transform(X,rt);
	dN.SetDim(12,2);
	double r=rt[ 0 ], t=rt[ 1 ],lg10 = log((double)10.0);


		dN(0,0) =  -(2*ep*sin(ep*log10(r))-lg10*cos(ep*log10(r)))*exp(-(ep*t))*sin(t/2)/(2*lg10*sqrt(r));
		dN(1,0) =  -(2*ep*sin(ep*log10(r))-lg10*cos(ep*log10(r)))*exp(-(ep*t))*cos(t/2)/(2*lg10*sqrt(r));
		dN(2,0) =  -(2*ep*sin(ep*log10(r))-lg10*cos(ep*log10(r)))*exp(ep*t)*sin(t/2)/(2*lg10*sqrt(r));
		dN(3,0) =  -(2*ep*sin(ep*log10(r))-lg10*cos(ep*log10(r)))*exp(ep*t)*cos(t/2)/(2*lg10*sqrt(r));
		dN(4,0) =  -(2*ep*sin(ep*log10(r))-lg10*cos(ep*log10(r)))*exp(ep*t)*sin(t/2)*sin(t)/(2*lg10*sqrt(r));
		dN(5,0) =  -(2*ep*sin(ep*log10(r))-lg10*cos(ep*log10(r)))*exp(ep*t)*cos(t/2)*sin(t)/(2*lg10*sqrt(r));
		dN(6,0) =  (lg10*sin(ep*log10(r))+2*ep*cos(ep*log10(r)))*exp(-(ep*t))*sin(t/2)/(2*lg10*sqrt(r));
		dN(7,0) =  (lg10*sin(ep*log10(r))+2*ep*cos(ep*log10(r)))*exp(-(ep*t))*cos(t/2)/(2*lg10*sqrt(r));
		dN(8,0) =  (lg10*sin(ep*log10(r))+2*ep*cos(ep*log10(r)))*exp(ep*t)*sin(t/2)/(2*lg10*sqrt(r));
		dN(9,0) =  (lg10*sin(ep*log10(r))+2*ep*cos(ep*log10(r)))*exp(ep*t)*cos(t/2)/(2*lg10*sqrt(r));
		dN(10,0) = (lg10*sin(ep*log10(r))+2*ep*cos(ep*log10(r)))*exp(ep*t)*sin(t/2)*sin(t)/(2*lg10*sqrt(r));
		dN(11,0) = (lg10*sin(ep*log10(r))+2*ep*cos(ep*log10(r)))*exp(ep*t)*cos(t/2)*sin(t)/(2*lg10*sqrt(r)); 

		dN(0,1)  = -sqrt(r)*cos(ep*log10(r))*exp(-(ep*t))* (2*ep*sin(t/2)-cos(t/2))/2;
		dN(1,1)  = -sqrt(r)*cos(ep*log10(r))*exp(-(ep*t))* (sin(t/2)+2*ep*cos(t/2))/2;
		dN(2,1)  =  sqrt(r)*cos(ep*log10(r))*exp(  ep*t) * (2*ep*sin(t/2)+cos(t/2))/2;
		dN(3,1)  = -sqrt(r)*cos(ep*log10(r))*exp(  ep*t) * (sin(t/2)-2*ep*cos(t/2))/2;
		dN(4,1)  =  sqrt(r)*cos(ep*log10(r))*exp(  ep*t) * (2*ep*sin(t/2)*sin(t)+cos(t/2)*sin(t)+2*sin(t/2)*cos(t))/2;
		dN(5,1)  = -sqrt(r)*cos(ep*log10(r))*exp(  ep*t) * (sin(t/2)*sin(t)-2*ep*cos(t/2)*sin(t)-2*cos(t/2)*cos(t))/2;
		dN(6,1)  = -sqrt(r)*sin(ep*log10(r))*exp(-(ep*t))* (2*ep*sin(t/2)-cos(t/2))/2;
		dN(7,1)  = -sqrt(r)*sin(ep*log10(r))*exp(-(ep*t))* (sin(t/2)+2*ep*cos(t/2))/2;
		dN(8,1)  =  sqrt(r)*sin(ep*log10(r))*exp(  ep*t) * (2*ep*sin(t/2)+cos(t/2))/2;
		dN(9,1)  = -sqrt(r)*sin(ep*log10(r))*exp(  ep*t) * (sin(t/2)-2*ep*cos(t/2))/2;
		dN(10,1) =  sqrt(r)*sin(ep*log10(r))*exp(  ep*t) * (2*ep*sin(t/2)*sin(t)+cos(t/2)*sin(t)+2*sin(t/2)*cos(t))/2;
		dN(11,1) = -sqrt(r)*sin(ep*log10(r))*exp(  ep*t) * (sin(t/2)*sin(t)-2*ep*cos(t/2)*sin(t)-2*cos(t/2)*cos(t))/2; 

	mvector g(2), G(2);
	matrix J;

	pspace.jacobian( rt, J );
	J.Inverse();


	for (unsigned k=0; k<12; k++)
	{
		g[0] = dN(k,0); 
		g[1] = dN(k,1);

		G = J * g;

		dN(k,0) = G[0]; 
		dN(k,1) = G[1];
	}

}

void CEnrichmentFunctionsBimaterial::transform( const mvector &X, mvector &rt )const
{
	double dx = X[0] - xs[0],  
		   dy = X[1] - xs[1], len = sqrt(dx*dx+dy*dy);

	double angle = asin( dy / len ); 
	if ( dx < 0.0 ) angle = M_PI - angle;
	else if ( dy < 0.0 ) angle =  angle + 2.0 * M_PI ;

	rt[0] = len;
	rt[1] = angle - alpha;


}
