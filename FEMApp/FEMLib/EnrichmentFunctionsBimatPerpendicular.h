#pragma once
#include "EnrichmentFunctions.h"
#include "../MathLib/SpacePolar.h"

class CEnrichmentFunctionsBimatPerpendicular :
	public CEnrichmentFunctions
{
public:
	CEnrichmentFunctionsBimatPerpendicular(const mvector &x, double l, double alpha );
	virtual ~CEnrichmentFunctionsBimatPerpendicular(void);

	virtual void GetValues( const mvector &X , mvector &N ) const = 0;
	virtual void GetGradients( const mvector &X,  matrix &dN ) const = 0;
	virtual unsigned GetN() { return 4; }

	void transform( const mvector &X, mvector &rt ) const;

protected:
	double lambda;
	mvector xs;
	CSpacePolar pspace;
};
