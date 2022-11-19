#pragma once
#include "../FEMLib/EnrichmentFunctions.h"
#include "../MathLib/SpacePolar.h"

class CEnrichmentFunctionsBimaterial :
	public CEnrichmentFunctions
{
public:
	
	CEnrichmentFunctionsBimaterial( const mvector &x, double e, double alpha );
	virtual ~CEnrichmentFunctionsBimaterial(void);
	virtual void GetValues( const mvector &X,  mvector &N ) const;
	virtual void GetGradients( const mvector &X, matrix &dN ) const;
	virtual unsigned GetN() const { return 12; }
	const CSpacePolar& GetPolarSpace() const { return pspace; }
	void transform( const mvector &X, mvector &rt ) const;

protected:

	

	double ep, alpha;
	mvector xs;
	CSpacePolar pspace;
};
