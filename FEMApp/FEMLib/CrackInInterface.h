#pragma once
#include "CrackLine.h"
#include "Material.h"

class CCrackInInterface :
	public CCrackLine
{
public:
	friend class CFEFunctorVectorUauxK1;
	friend class CFEFunctorVectorUauxK2;
	friend class CFEFunctorMatrixGradUauxK1;
	friend class CFEFunctorMatrixGradUauxK2;
	CCrackInInterface( const mvector &a1, const mvector &a2, CMaterial *m1, CMaterial *m2, bool ips );

	virtual ~CCrackInInterface(void);
	virtual void EnrichNodes();
	virtual void ComputeSIFs( double dist );

protected:
	CMaterial *mat1, *mat2;
	double alpha, beta, k1, k2, mu1, mu2, eps;
	bool is_plane_stress;

};
