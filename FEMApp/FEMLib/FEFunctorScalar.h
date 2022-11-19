#pragma once
#include "FEFunctor.h"

class CFEFunctorScalar :
	public CFEFunctor< CFiniteElementInstance >
{
public:

	operator double() const { return value; }
	CFEFunctorScalar(const string &nm);
	virtual ~CFEFunctorScalar(void);
	virtual void DebugPrint( ) const;
    const double& operator=( const double &v ) { InvalidateUsedByObjects(); value = v; return v; }

protected:	

	double value;
};

