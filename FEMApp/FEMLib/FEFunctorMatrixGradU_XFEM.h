#pragma once

#include "FEFunctorMatrix.h"
#include "FEFunctorVectorLagrangeX.h"
#include "FEFunctorMatrixShapeFNLagrangeGrad.h"
#include "Crack.h"
#include "EnrichmentFunctions.h"

class CFEFunctorMatrixGradU_XFEM :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixGradU_XFEM( CCrack *c, CEnrichmentFunctions *e, CFEFunctorVectorLagrangeX *x, CFEFunctorMatrixShapeFNLagrangeGrad *dn );
	virtual ~CFEFunctorMatrixGradU_XFEM( void );
	CFEFunctorMatrixGradU_XFEM( const CFEFunctorMatrixGradU_XFEM &vf );
	CLONE_FN(CFEFunctorMatrixGradU_XFEM)

	virtual void ManageDOFs( CFiniteElementInstance *fe1 ) { }

protected:

	CCrack *c;
	CEnrichmentFunctions *e;

	virtual void Compute(CFiniteElementInstance *fe1);
	CFEFunctorVectorLagrangeX *X;
	CFEFunctorMatrixShapeFNLagrangeGrad *dN;
};




