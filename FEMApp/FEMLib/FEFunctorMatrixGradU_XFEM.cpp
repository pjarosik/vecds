
#include "FEFunctorMatrixGradU_XFEM.h"
#include "DOFs.h"

CFEFunctorMatrixGradU_XFEM::CFEFunctorMatrixGradU_XFEM( CCrack *c1, CEnrichmentFunctions *e1, CFEFunctorVectorLagrangeX *x, CFEFunctorMatrixShapeFNLagrangeGrad *dn ):CFEFunctorMatrix("CFEFunctorMatrixGradU_XFEM"),c(c1),e(e1),X(x),dN( dn )
{
	AddNode( dN );
	AddNode( X );
}

CFEFunctorMatrixGradU_XFEM::CFEFunctorMatrixGradU_XFEM( const CFEFunctorMatrixGradU_XFEM &vf ):CFEFunctorMatrix(vf),c(vf.c),e(vf.e)
{
	CLONE_MEMBER( vf, dN )
	CLONE_MEMBER( vf,  X )
}

CFEFunctorMatrixGradU_XFEM::~CFEFunctorMatrixGradU_XFEM( void )
{

}

void CFEFunctorMatrixGradU_XFEM::Compute( CFiniteElementInstance *fe1)
{
	fv.SetDim(2,2);
	const vector<CDOFVector> &edofs = fe1->GetElemDofs();
	matrix &dU = fv, &dN1 = *dN;
	unsigned k, l=0;


	for (k=0; k<edofs.size(); k++)
	{
		dU(0,0)+=dN1(k,0)*edofs[k][0]->GetSolution();
		dU(0,1)+=dN1(k,1)*edofs[k][0]->GetSolution();

		dU(1,0)+=dN1(k,0)*edofs[k][1]->GetSolution();
		dU(1,1)+=dN1(k,1)*edofs[k][1]->GetSolution();



		CDOF *dof = edofs[k].FindDOF( &CDOFs::Ax, l );
		if ( dof )
		{
			dU(0,0)+= c->H( (*X) ) * dN1(k,0) * dof->GetSolution();
			dU(0,1)+= c->H( (*X) ) * dN1(k,1) * dof->GetSolution();
		}

		dof = edofs[k].FindDOF( &CDOFs::Ay, l );
		if ( dof )
		{
			dU(1,0)+= c->H( (*X) ) * dN1(k,0) * dof->GetSolution();
			dU(1,1)+= c->H( (*X) ) * dN1(k,1) * dof->GetSolution();
		}

		dof = edofs[k].FindDOF( &CDOFs::Bx[0], l );
		if ( dof ) assert( false );

	}

}
