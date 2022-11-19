#pragma once
#include "FEFunctorMatrixColDOF.h"
#include "DOF_Ux.h"
#include "DOF_Uy.h"
#include "DOF_Uz.h"


class CFEFunctorMatrixStrainDerivSolid :
	public CFEFunctorMatrixColDOF
{
public:
	CFEFunctorMatrixStrainDerivSolid( CFEFunctorMatrix * );
	CFEFunctorMatrixStrainDerivSolid( const CFEFunctorMatrixStrainDerivSolid &mf );
	virtual ~CFEFunctorMatrixStrainDerivSolid( void );

	CLONE_FN( CFEFunctorMatrixStrainDerivSolid )

protected:
	
	CFEFunctorMatrix *dSF;

	void Compute( CFiniteElementInstance *fe1 );
};
