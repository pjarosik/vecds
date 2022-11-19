#pragma once

#include "../MathLib/matrix.h"
#include "../MathLib/ScalarFunction.h"

class CEnrichmentFunctions
{
public:
	CEnrichmentFunctions( );
	virtual ~CEnrichmentFunctions(void);
	virtual void GetValues( const mvector &X , mvector &N ) const = 0;
	virtual void GetGradients( const mvector &X,  matrix &dN ) const = 0;
	virtual unsigned GetN() const = 0;
protected:
};

