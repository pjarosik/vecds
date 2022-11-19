#pragma once
#include "FEFunctorVector.h"
#include "FEFunctorMatrix.h"

class CFEFunctorVectorQweight 
	: public CFEFunctorVector
{
public:
	CFEFunctorVectorQweight( CFEFunctorMatrix *dn, const mvector &x, double d );
	virtual ~CFEFunctorVectorQweight(void);
	CFEFunctorVectorQweight( const CFEFunctorVectorQweight &vf );
	CLONE_FN(CFEFunctorVectorQweight)
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

	bool AcceptFE(CFiniteElementInstance *fe1 );

protected:

    bool IsInside( CFENode *nd ) const { mvector X; nd->GetX(X); return fabs( xs[0] - X[0] ) < dist && fabs( xs[1] - X[1] ) < dist; }
//	bool IsInside( CFENode *nd ) const { return nd->Distance( xs ) < dist; }
//	bool isInside( const mvector &x ) const { return xs. < dist; }
	virtual void Compute(CFiniteElementInstance *fe1);
	CFEFunctorMatrix *dN;
	double dist;
	mvector xs;
	
};
