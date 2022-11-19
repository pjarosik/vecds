
#include "FE_Equation.h"


CFE_Equation::CFE_Equation(  DomainContainer< CFENode > &nd, CPostProcessing *pp ):all_nodes( nd ),postproces( pp ),nthreads( 1 )
{

}

CFE_Equation::~CFE_Equation( void )
{
	map<string,CFEMatrix*>::iterator k;
	for( k=M.begin(); k!=M.end(); k++)
		delete k->second;

	map<string,CFEVector*>::iterator i;
	for( i=V.begin(); i!=V.end(); i++)
		delete i->second;
    delete postproces;
}

void CFE_Equation::SelectFixedBCs( const CDOFVector &dv )
{
    unsigned k;
    dofs_fixed.clear();
    for (k=0; k<dv.size(); k++)
         if ( dv[ k ]->IsFixed() ) dofs_fixed.push_back( dv[ k ] );
}

CFEMatrix* CFE_Equation::CreateMatrix( const string &name )
{
	map< string, CFEMatrix* >::iterator i = M.find(name);
	if ( i != M.end() ) throw CFEException( "CFE_Equation::CreateMatrix -  matrix named :" + name + " already exists" );
	CFEMatrix *nM = new CFEMatrix;
	M[ name ] = nM;
	return nM;
}

CFEVector* CFE_Equation::CreateVector( const string &name )
{
	map< string, CFEVector* >::iterator i = V.find( name );
	if ( i != V.end() ) throw CFEException( "CFE_Equation::CreateVector -  vector named :" + name + " already exists" );
	CFEVector *nP =new CFEVector;
	V[ name ] = nP;
	return nP;
}

CFEMatrix* CFE_Equation::FindMatrix( const string &mname )
{
	map<string,CFEMatrix*>::iterator i = M.find( mname );
	if ( i == M.end() ) throw CFEException ( " CFE_Equation::FindMatrix - can't find CFEMatrix named: " + mname );
	return i->second;
}

CFEVector* CFE_Equation::FindVector( const string &vname )
{
	map<string,CFEVector*>::iterator i = V.find( vname );
	if ( i == V.end() ) throw CFEException ( " CFE_Equation::FindVector - can't find CFEVector named:" + vname );
	return i->second;
}

void CFE_Equation::ApplyBoundaryConditionsToMatrix( CLargeMatrix &LM, const vector< CDOF* > &dofs )
{
	unsigned k = 0;
    if ( dofs_fixed.size() )
    {
        for ( k = 0; k < dofs_fixed.size( ); k++ )
            LM.EraseDof( dofs_fixed[ k ]->GetNumber( ) );
    }
    else
    {
        if ( LM.GetDim() != (unsigned)dofs.size() ) throw CFEException( "CFE_Equation::ApplyBoundaryConditions() - differend LargeMatrix and DOF vector sizes" );
        for ( k = 0; k < dofs.size( ); k++ )
            if ( dofs[ k ]->IsFixed( ) ) LM.EraseDof( dofs[ k ]->GetNumber() );
    }
}

void CFE_Equation::CreateFEFunctorMatrixAssignment( const string &nm, CFEFunctorMatrix *mf,  const vector< CFiniteElementInstance* > &vfe )
{
	CFEMatrix *fm = FindMatrix( nm );
	if (!fm) throw CFEException("CFE_Equation can't match matrix functor named:" + nm );
	fm->AssignFiniteElements( mf, vfe );
}


void CFE_Equation::CreateFEFunctorMatrixAssignment( const map<string, CFEFunctorMatrix *> &mfs,  const vector< CFiniteElementInstance* > &vfe )
{
	map<string, CFEFunctorMatrix *>::const_iterator cit;
	for (cit=mfs.begin(); cit != mfs.end(); cit++)
	{
		CFEMatrix *fm = FindMatrix( cit->first );
		if (!fm) throw CFEException("CFE_Equation can't match matrix functor named:" + cit->first );
		fm->AssignFiniteElements( cit->second, vfe );
	}
}

void CFE_Equation::CreateFEFunctorVectorAssignment( const map<string, CFEFunctorVector *> &vfs,  const vector< CFiniteElementInstance* > &vfe )
{
	map<string, CFEFunctorVector *>::const_iterator cit;
	for (cit=vfs.begin(); cit != vfs.end(); cit++)
	{
		CFEVector *fv = FindVector( cit->first );
		if ( !fv ) throw CFEException("CFE_Equation can't match vector functor named:" + cit->first );
		fv->AssignFiniteElements( cit->second, vfe );
	}
}



void CFE_Equation::AssignFiniteElement ( CFiniteElement *fe )
{
	felements.push_back( fe );
    const map< string, CFEFunctorVector* >&V = fe->GetFEFunctorVectors();
    const map< string, CFEFunctorMatrix* >&M = fe->GetFEFunctorMatrixs();
	const vector< FEDomain* > &domains = fe->GetDomains();
	vector< FEDomain* >::const_iterator i;
	for (i=domains.begin(); i!=domains.end(); i++)
	{
		CreateFEFunctorMatrixAssignment( M, **i );
		CreateFEFunctorVectorAssignment( V, **i );
	}
}

void  CFE_Equation::EndStage(  )
{
    if ( postproces ) postproces->EndStage( this );
}

bool  CFE_Equation::IsAvailebleStage( const string &st, bool ex )
{
    bool res = find( stages.begin(), stages.end(), st ) != stages.end();
    if ( !res && ex ) throw CFEException( "Unsupported stage named \'" + st + "\'" );
}


void CFE_Equation::RecomputeResults( bool selected_only  )
{
   unsigned k;

   for (k=0; k<all_nodes.size(); k++)
       if ( all_nodes[k]->IsSelected() ) all_nodes[ k ]->ResetResults();

   for (k=0; k<felements.size(); k++)
       felements[ k ]->ComputeResults( selected_only );
}

bool CFE_Equation::HasResultNamed( const string &nm )
{
    unsigned k;
    for (k=0; k<felements.size(); k++)
        if ( felements[ k ]->HasResultNamed( nm ) ) return true;
    return false;
}
