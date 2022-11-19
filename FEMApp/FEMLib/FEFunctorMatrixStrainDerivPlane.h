#pragma once
#include "FEFunctorMatrixColDOF.h"
#include "DOF_Ux.h"
#include "DOF_Uy.h"

class CFEFunctorMatrixStrainDerivPlane :
	public CFEFunctorMatrixColDOF
{
public:

	CFEFunctorMatrixStrainDerivPlane( CFEFunctorMatrix *dn );
	virtual ~CFEFunctorMatrixStrainDerivPlane( void );
	CFEFunctorMatrixStrainDerivPlane( const CFEFunctorMatrixStrainDerivPlane &mf );
        CLONE_FN( CFEFunctorMatrixStrainDerivPlane )
protected:

	CFEFunctorMatrix *dSF;
	static CDOF_Ux dux;
	static CDOF_Uy duy;

	virtual void Compute( CFiniteElementInstance *fe1);
};
