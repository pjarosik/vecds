#pragma once

#include "FiniteElementInstance.h"
#include "FEFunctor.h"
#include "../MathLib/matrix.h"
#include "FEAllocator.h"
#include <string>

using namespace std;

class CFEFunctorVector : public CFEFunctor< CFiniteElementInstance >
{
public:

	friend mvector& operator << ( mvector &L, const CFEFunctorVector &vf );

	CFEFunctorVector( const string &nm );
	CFEFunctorVector( const string &nm, CDOF* tp[], unsigned n );
	virtual ~CFEFunctorVector( void );
	virtual CFEFunctorVector* Clone()=0;
	virtual void ManageDOFs( CFiniteElementInstance *p )  { al.CollectDOFs( p ); }
	virtual void CreateDOFs( CFiniteElementInstance *p );

	operator const mvector&() const { return fv; }
	const mvector& operator=( const mvector &v ) { InvalidateUsedByObjects(); fv = v; return v; }

	virtual void DebugPrint( ) const;
	const vector<string>& GetNames() const { return mnames; }
	void  GetAllocationVector( valarray<unsigned> &av ) const { al.GetAllocationVector( av ); }
	const CFEAllocator& GetAllocator() const { return al; }
	const CDOFVector& GetDOFsVector() const { return al.GetDOFs(); }

	unsigned GetDim() const { return fv.GetDim(); }
	double operator[](unsigned i) const { return fv[ i ]; }
	double& operator[](unsigned i) { return fv[ i ]; }

protected:

	mvector fv;
	CFEAllocator al;
	vector< string > mnames;

};


mvector& operator << ( mvector &L, const CFEFunctorVector &vf );
