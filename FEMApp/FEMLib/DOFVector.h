#pragma once
#include <valarray>
#include <vector>
#include <algorithm>
#include "DOF.h"
using namespace std;


class CDOFVector :
	public vector< CDOF* >
{
public:
	CDOFVector( void );
	CDOFVector( unsigned n, CDOF*[] );
	~CDOFVector( void );
	void Add( CDOF *dof ) { push_back( dof ); }
	void Merge( const CDOFVector &Dofs, bool unique = false );
	void MergeUniqueType( const CDOFVector &Dofs );
	bool FindDOFs( const CDOFVector &DofTypes, CDOFVector &f, CDOFVector &nf ) const;
	CDOF* FindDOF( CDOF* type, unsigned &bind ) const;
	void GetAllocationVector( valarray< unsigned > &al ) const;
	void GetAllocationVector( const CDOFVector &dv, valarray< unsigned > &al ) const;
	void EnumerateDofs();
};


inline CDOF* CDOFVector::FindDOF( CDOF* type, unsigned &bind ) const 
{ 
	unsigned k;
	for (k=0; k<size(); k++)
	{
		if ( at( bind )->GetType() == (*type) ) return at( bind ); 
		bind = bind == size() - 1 ? 0 : bind + 1;
	}
	return 0;
}


inline void CDOFVector::GetAllocationVector( valarray< unsigned > &al ) const
{
	al.resize( size() );
	for(unsigned k=0; k<size(); k++)
		al[ k ] = at( k )->GetNumber();
}

inline void CDOFVector::EnumerateDofs(  ) 
{
	for(unsigned k=0; k<size(); k++)
		at( k )->SetNumber( k );
}

