#pragma once

#include "DOF.h"
#include "DOFVector.h"
#include "FiniteElementInstance.h"

class CFEAllocator
{
public:
	
	CFEAllocator( const CDOFVector& );
	CFEAllocator( CDOF* tp[], unsigned n );

	CFEAllocator( ):last_fe( 0 ) { }
	~CFEAllocator( void );
	
	void Init( const CFEAllocator &av ) { Init( av.GetTypes() ); }
	void Init( const CDOFVector &dv ) { ntypes = dv; dofs.clear(); }
	void Merge( const CFEAllocator &al, bool unique = false ) { ntypes.Merge( al.GetTypes(), unique ); }
	void CreateDOFs( CFiniteElementInstance *fe ) { if ( fe != last_fe) { fe->CreateDOFs( ntypes ); last_fe = fe; } }
	void CollectDOFs( CFiniteElementInstance *fe );
	void GetAllocationVector( valarray< unsigned > &alv ) const { dofs.GetAllocationVector( alv ); }
	void GetAllocationVector( const CDOFVector &dv, valarray< unsigned > &alv ) const { dofs.GetAllocationVector( dv, alv ); }
	const CDOFVector& GetDOFs() const { return dofs; }
	const CDOFVector& GetTypes() const { return ntypes; }
	unsigned GetDim() const { return dofs.size(); }
	unsigned operator []( unsigned ind ) const { return dofs[ ind ]->GetNumber(); }
	const CFEAllocator& operator=( const CFEAllocator &as ) { ntypes = as.ntypes; dofs = as.dofs; return as; }
	
protected:

	CDOFVector ntypes, dofs;
	CFiniteElementInstance *last_fe;

};


inline void CFEAllocator::CollectDOFs( CFiniteElementInstance *fe )
{
//	if ( fe != last_fe ) {
		dofs.clear( );
		fe->GetDOFs( ntypes, dofs ); 
		last_fe = fe;
//	}
}
