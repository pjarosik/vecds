#include "FEFunctorMatrixToNodalTransformation.h"
#include "../MathLib/SpaceShape.h"

CFEFunctorMatrixToNodalTransformation::CFEFunctorMatrixToNodalTransformation( const CIntegrationManager &im, CFEFunctorMatrix *m  ):CFEFunctorMatrix("CFEFunctorMatrixToNodalTransformation"),IM(im),gf(m)
{
   
    AddNode( m );
	cols_names = m->GetColsNames();
    
}

CFEFunctorMatrixToNodalTransformation::CFEFunctorMatrixToNodalTransformation( const CFEFunctorMatrixToNodalTransformation &mf ):CFEFunctorMatrix(mf),gf( mf.gf ), IM( mf.IM )
{
    CLONE_MEMBER( mf, gf )
}


void CFEFunctorMatrixToNodalTransformation::ManageDOFs( CFiniteElementInstance *fe1 )
{
	gf->ManageDOFs( fe1 );

}

void CFEFunctorMatrixToNodalTransformation::Compute( CFiniteElementInstance *fe1 )
{
	
	const matrix &mgf = (*gf);
	
    const vector< CFENode* > &nodes = fe1->GetNodes();
    const CSpaceShape &sf = fe1->GetShapeFunctions();
    mvector N, res( mgf.GetCols() );
	unsigned k=0, l=0, m=0;
    const CIntegrationPoints *ip = IM.GetIntegrationPoints( sf.GetShapeType(), sf.Getxdim(), sf.GetDegree() );
    const vector< mvector > &xi = ip->GetPoints();
    const matrix &gres = *gf;
    matrix jm;


	for (k=0; k<xi.size(); k++)
	{
        sf.transform( xi[ k ], N );
		fe1->GetJacobyMatrix( xi[ k ], jm );
		double detJ = jm.Det();
		for (l=0; l<nodes.size(); l++)
		{
			for (m=0; m<gres.GetCols(); m++) 
				res[ m ] = gres( k, m ) * N[ l ] * detJ;
            nodes[ l ]->AccumulateResults( cols_names, res, detJ );
		}
	}
} 
