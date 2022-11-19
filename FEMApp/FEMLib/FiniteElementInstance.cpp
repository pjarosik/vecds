
#include "FiniteElementInstance.h"
#include "FEException.h"

CFiniteElementInstance::CFiniteElementInstance( CGeometricDomain &gd, const CSpaceShapeApproxPtr< CFENode > &s ):CGeometricObjectShape<CFENode>(gd,&s),material(0)
{
	FindBoundingBox();
    unsigned k;

}

CFiniteElementInstance::~CFiniteElementInstance(void)
{
}

bool CFiniteElementInstance::IsInBoundingBox( const mvector &x )
{
	unsigned k;
	for (k=0; k<x1.GetDim(); k++)
	{
		if ( (x[k] < x1[k]) || (x[k] > x2[k]) ) return false;
	}
	return true;
}

bool CFiniteElementInstance::IsInside( const mvector &X )
{
	unsigned dim = X.GetDim(), k;
	mvector xi( dim );
    main_shape->InverseTransform(X,xi);
	for( k=0; k<dim; k++)
		if ( xi[ k ] < -1.0 || xi[ k ] > 1.0 ) return false;
	return true;
}

void CFiniteElementInstance::FindBoundingBox()
{
	unsigned k,l;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
	if ( !nodes.size() ) return;
    nodes[0]->GetX(x1);
    nodes[0]->GetX(x2);
	for (k=1; k<nodes.size(); k++)
    {
        mvector X;
        nodes[ k ]->GetX( X );
		for (l=0; l<x1.GetDim(); l++)
		{
            if ( X[ l ] < x1[ l ] ) x1[ l ] =  X[ l ];
            if ( X[ l ] > x2[ l ] ) x2[ l ] =  X[ l ];
		}
    }
}

/*
void CFiniteElementInstance::FindDofInNodes( CDOF* dof_type, vector< unsigned > &vndof ) const
{
	vndof.clear();
	if (!dofs.size()) return;
	unsigned k,ind=0, nind=0;

	if ( dofs[ 0 ].FindDOF( dof_type, nind ) ) vndof.push_back( 0 );

	for (k=1; k<dofs.size(); k++)
	{
		ind = nind;
		if ( dofs[ k ].FindDOF( dof_type, ind ) ) vndof.push_back( k );
	}
}*/

void CFiniteElementInstance::CreateDOFs( const CDOFVector &doftype )
{
	unsigned k,l;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
	if ( dofs.size()==0 ) dofs.resize( nodes.size() );
	for (k=0; k<dofs.size(); k++)
	{
		CDOFVector not_found, nodal_found_dofs;
		nodes[ k ]->GetDofs().FindDOFs( doftype, nodal_found_dofs, not_found );
		dofs[ k ].clear();
		for ( l = 0; l< nodal_found_dofs.size(); l++ ) dofs[ k ].Add( nodal_found_dofs[ l ] );

		for ( l = 0; l< not_found.size(); l++ )
		{
				CDOF *dof = not_found[ l ]->Clone();
				nodes[ k ]->AddDOF( dof );
				dofs[ k ].Add( dof );
		}

	}
}

void CFiniteElementInstance::GetDOFs( const CDOFVector &doftype, CDOFVector &found_dofs )
{
	unsigned k;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
	if ( dofs.size()==0 ) dofs.resize( nodes.size() );


	for ( k=0; k<dofs.size(); k++ )
	{
		CDOFVector not_found;
        dofs[ k ].FindDOFs( doftype, found_dofs, not_found );
	}
}

const CIntegrationPoints* CFiniteElementInstance::GetIntegrationPoints( const CIntegrationManager &IM, int reduction  )
{
    const CSpaceShape &sh = main_shape->GetShapeSpace();
    return IM.GetIntegrationPoints( sh.GetShapeType(), sh.Getxdim(), sh.GetDegree() + reduction );
}

/*
void CFiniteElementInstance::TransformGradient(  const mvector &xi, const mvector &dXi, mvector &dX ) const
{
	matrix J1;
	GetJacobyMatrix( xi, J1 );
	J1.Inverse();
	dX = J1 * dXi;
}
*/

void CFiniteElementInstance::GetGlobalSFGradient( const mvector &xi, matrix &dN ) const
{
	matrix J1;
    GetJacobyMatrix( xi, J1 );
	J1.Inverse();
	unsigned k, l, m, dim = xi.GetDim();
	mvector dxi(dim), dX(dim);
	for (k=0; k<dN.GetRows(); k++)
	{
		dN.ExtractRow( k, dxi );
		for (l=0; l<dim; l++)
		{
			dN(k,l) = 0;
			for ( m = 0; m < dim; m++ )
				dN(k,l)+=J1(l,m)*dxi[m];
		}
	}
}


void CFiniteElementInstance::GetJacobyMatrix( const mvector &xi, matrix &J ) const
{
	unsigned dim = xi.GetDim();
	J.SetDim( dim, dim );
	matrix dN;
    main_shape->GetShapeSpace().jacobian( xi, dN );
	unsigned k,l,m;
    mvector X;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
    for ( m = 0; m < nodes.size(); m++ )
    {
        nodes[ m ]->GetX( X );
        for ( k = 0; k < dim; k++ )
            for ( l = 0; l < dim; l++ )
                J(k,l) += dN( m, k ) * X[ l ];
    }

}


void CFiniteElementInstance::GetXi( const mvector &X, mvector &xi ) const
{
    main_shape->InverseTransform(X,xi);
}



void CFiniteElementInstance::UpdateNodalGraph(  ) const
{
    unsigned k;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
    for (k=0; k<nodes.size(); k++ )
        nodes[ k ]->UpdateGraph( nodes );
 
}

void CFiniteElementInstance::ResetNodalGraph(  ) const
{
    unsigned k;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
    for (k=0; k<nodes.size(); k++ )
        nodes[ k ]->ResetGraph( );

}

void CFiniteElementInstance::SelectDofsState( bool st )
{
    unsigned k,l;
    for ( k=0; k<dofs.size(); k++ )
        for (l=0; l< dofs[k].size(); l++)
            dofs[ k ][ l ]->SetSelection( st );
}


void CFiniteElementInstance::SelectNodesState( bool st )
{
    unsigned k;
    for ( k=0; k<main_shape->GetPoints().size(); k++ )
             main_shape->GetPoints()[ k ]->SetSelection( st );
}


bool CFiniteElementInstance::HasResultNamed( const string &name )
{
    unsigned k, l;
    for ( k=0; k<dofs.size(); k++ )
        for (l=0; l< dofs[ k ].size(); l++)
            if ( dofs[ k ][ l ]->DofName() == name ) return true;
    return false;
}


vector< string > CFiniteElementInstance::GetResultFieldsNames(  ) const
{
    unsigned k;
    map< string, unsigned > names;
    const vector< CFENode* > &nodes = main_shape->GetPoints();
    for (k=0; k<nodes.size(); k++ )
    {
        const map< string, double > &nr =   nodes[ k ]->GetResults();
        map< string, double >::const_iterator i;
        for ( i = nr.begin(); i != nr.end(); i++ )  names[ i->first ]++;

    }
    map< string, unsigned >::const_iterator i;
    vector< string > res;
    for ( i = names.begin(); i != names.end(); i++ )  if ( i->second == nodes.size() ) res.push_back( i->first );
    return res;
}

CScalarShapeSpaceApprox CFiniteElementInstance::CreateResultApproximator( const string &rname ) const
{
    const vector< CFENode* > &nodes = main_shape->GetPoints();
    vector< double > vals( nodes.size() );
    vector< CFENode* >::const_iterator i;
    for (i=nodes.begin(); i != nodes.end(); i++ )
        vals[ i - nodes.begin() ] = (*i)->GetResult( rname );
    return CScalarShapeSpaceApprox( main_shape->GetShapeSpace(), vals, rname );
}

void CFiniteElementInstance::GetShapeObjects( unsigned objdim, vector< CGeometricObjectShapeNodal* > &gs, bool boundary  ) const
{
    gs.clear();
    vector< CGeometricObject* >::const_iterator i;
    for ( i = GetOwned().begin(); i != GetOwned().end(); i++ )
        if ( (*i)->GetObjectDim() == objdim ) {
                if ( boundary )
                {
                    if ( (*i)->IsBoundary() ) gs.push_back( dynamic_cast< CGeometricObjectShapeNodal* >( *i )  );
                }
                else gs.push_back( dynamic_cast< CGeometricObjectShapeNodal* >( *i )  );
        }
}


CScalarShapeSpaceApprox CFiniteElementInstance::CreateResultApproximator( const CGeometricObjectShapeNodal *gos,  const string &rname ) const
{
    if ( !Owns(gos) ) throw CFEException("CFiniteElementInstance::CreateResultApproximator object is not a component of FEElement");

    if ( gos == 0 ) throw CFEException("CFiniteElementInstance::CreateResultApproximator object is not a CGeometricObjectShape<CFENode*>  object");
    const vector< CFENode* > &nodes = gos->GetShapeSpace()->GetPoints();
    vector< double > vals( nodes.size() );
    vector< CFENode* >::const_iterator i;
    for (i = nodes.begin(); i != nodes.end(); i++ )
        vals[ i - nodes.begin() ] = (*i)->GetResult( rname );
    CScalarShapeSpaceApprox ssa( gos->GetShapeSpace()->GetShapeSpace(), vals, rname );
    return ssa;
}
