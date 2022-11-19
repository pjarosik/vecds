
#include "DOFVector.h"

CDOFVector::CDOFVector( void )
{
}

CDOFVector::CDOFVector( unsigned n, CDOF* df[] )
{
	unsigned k;
	for( k=0; k<n; k++)
		push_back( df[ k ] );
}

CDOFVector::~CDOFVector( void )
{
}

void CDOFVector::Merge( const CDOFVector &Dofs, bool unique )
{ 
	for(unsigned k=0; k<Dofs.size(); k++) 
	{
		if ( unique ) 
		{
			CDOFVector::const_iterator i = find( begin(), end(), Dofs[k] );
			if ( i == end() )  push_back( Dofs[ k ] ); 
		}
		else push_back( Dofs[ k ] ); 
	}
}

void CDOFVector::MergeUniqueType( const CDOFVector &Dofs )
{
	unsigned bind=0;
	CDOFVector::const_iterator i;
	for (i=Dofs.begin(); i!=Dofs.end(); i++)
		if ( !FindDOF( (*i), bind ) ) push_back( *i );
	

}

bool CDOFVector::FindDOFs( const CDOFVector &DofTypes, CDOFVector &dof_found, CDOFVector &dof_not_found ) const
{
	unsigned ind = 0;
	
	if ( size() == 0 ) 
	{
		dof_not_found.Merge( DofTypes );
		return true;
	}

	CDOFVector::const_iterator i;
	for (i=DofTypes.begin(); i!=DofTypes.end(); i++)
	{
		CDOF *DOF = FindDOF( (*i), ind );
		if ( DOF ) dof_found.push_back( DOF );
		else dof_not_found.push_back( *i );
	}

	return true;
}


void CDOFVector::GetAllocationVector( const CDOFVector &dv, valarray< unsigned > &al ) const
{
	al.resize( dv.size( ) );
	unsigned idv = 0, ind = 0, off = 0;
	while( idv < dv.size() )
	{
		if ( dv[ idv ] == at( ind ) ) 
		{
			al[ idv++ ] = ind;
			off = 0;
		}
		else off++;
		if ( off == size() ) return;
		ind++; 
		if ( ind == size() ) ind = 0;
	}
}
