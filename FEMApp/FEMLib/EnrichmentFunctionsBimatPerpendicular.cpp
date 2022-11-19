
#include "EnrichmentFunctionsBimatPerpendicular.h"

CEnrichmentFunctionsBimatPerpendicular::CEnrichmentFunctionsBimatPerpendicular(const mvector &x, double l, double a ):lambda(l), xs(x), pspace(x,a)
{

}

CEnrichmentFunctionsBimatPerpendicular::~CEnrichmentFunctionsBimatPerpendicular(void)
{
}


void CEnrichmentFunctionsBimatPerpendicular::GetValues( const mvector &X , mvector &N ) const
{
	mvector rt(2);
	transform(X,rt);
	double r=rt[0],t=rt[1];

	N[0]  = sqrt(r) * cos(lambda*log(r)) * exp(-(lambda*t)) * sin(t/2.0);
	N[1]  = sqrt(r) * cos(lambda*log(r)) * exp(-(lambda*t)) * cos(t/2.0);
	N[2]  = sqrt(r) * cos(lambda*log(r)) * exp((lambda*t))  * sin(t/2.0);
	N[3]  = sqrt(r) * cos(lambda*log(r)) * exp((lambda*t))  * cos(t/2.0);


}

void CEnrichmentFunctionsBimatPerpendicular::GetGradients( const mvector &X,  matrix &dN ) const
{
	mvector rt(2);
	transform(X,rt);
	double r=rt[0],t=rt[1];

		dN(0,0) = -(2*lambda*sin(lambda*log(r))-cos(lambda*log(r)))*exp(-(lambda*t))*sin(t/2)/(2*sqrt(r));
		dN(1,0) = -(2*lambda*sin(lambda*log(r))-cos(lambda*log(r)))*exp(-(lambda*t))*cos(t/2)/(2*sqrt(r));
		dN(2,0) = -(2*lambda*sin(lambda*log(r))-cos(lambda*log(r)))*exp((lambda*t))*sin(t/2)/(2*sqrt(r));
		dN(3,0) = -(2*lambda*sin(lambda*log(r))-cos(lambda*log(r)))*exp((lambda*t))*cos(t/2)/(2*sqrt(r));

		dN(0,1)  = -sqrt(r)*cos(lambda*log(r))*exp(-(lambda*t))*(2*lambda*sin(t/2)-cos(t/2))/2;
		dN(1,1)  = -sqrt(r)*cos(lambda*log(r))*exp(-(lambda*t))*(sin(t/2)+2*lambda*cos(t/2))/2;
		dN(2,1)  =  sqrt(r)*cos(lambda*log(r))*exp((lambda*t))*(2*lambda*sin(t/2)+cos(t/2))/2;
		dN(3,1)  = -sqrt(r)*cos(lambda*log(r))*exp((lambda*t))*(sin(t/2)-2*lambda*cos(t/2))/2;

	mvector g(2), G(2);
	matrix J;

	pspace.jacobian( rt, J );
	J.Inverse();


	for (unsigned k=0; k<12; k++)
	{
		g[0] = dN(k,0); g[1] = dN(k,1);
		G = J * g;
		dN(k,0) = g[0]; dN(k,1) = g[1];
	}
}
