#pragma once

#include "FEFunctorVector.h"
#include "FEFunctorMatrixColDOF.h"
#include "Crack.h"
#include "DOF_Ax.h"
#include "DOF_Ay.h"

class CFEFunctorMatrixStrainDerivPlaneXFEM_H :
	public CFEFunctorMatrixColDOF
{
public:

	CFEFunctorMatrixStrainDerivPlaneXFEM_H( CCrack *c, CFEFunctorMatrix *mf, CFEFunctorVector *vf );
	CFEFunctorMatrixStrainDerivPlaneXFEM_H( const CFEFunctorMatrixStrainDerivPlaneXFEM_H &mf );
	~CFEFunctorMatrixStrainDerivPlaneXFEM_H( void );

protected:

	CFEFunctorMatrix *dSF;
	CFEFunctorVector *X;
	CCrack *crack;

	virtual void Compute( CFiniteElementInstance *fe1 );
};
