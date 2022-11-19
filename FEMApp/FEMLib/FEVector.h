#pragma once
#include "../MathLib/matrix.h"
#include "DOFVector.h"
#include "FEFunctorVector.h"
#include "FENode.h"


class CFEVector :
	public mvector
{

public:

	CFEVector(void);
	CFEVector(const CDOFVector &dv):DOFsVector(dv) { SetDim( dv.size() ); }
	CFEVector(const CFEVector &dv):mvector(dv),DOFsVector(dv.DOFsVector),DOFsTypes( dv.DOFsTypes ),VF(dv.VF) { }
	virtual ~CFEVector( void );
	void SetDOFVector( const CDOFVector &dv ) { DOFsVector = dv; SetDim( dv.size() ); }
	const CDOFVector& GetDOFVector() const { return DOFsVector; }
	const CDOFVector& GetDOFTypesVector() const { return DOFsTypes; }
    const vector< CFENode* >& GetUsedNodes() const { return used_nodes; }
	void AssignFiniteElements( CFEFunctorVector* mf, const vector< CFiniteElementInstance* > &fes );
    void BuildDOFsVector( vector< CFENode* > & );
	void Aggregate(bool dofs=true);
	void ApplyFixedBoundaryConditions( double mult );
	void ExtractDOFsSolution();
    void Save( const string &fn, bool append=true );
    void Read( ifstream &fin  );
    void SelectDOFsAndNodes( bool state );

protected:

    vector< CFENode* > used_nodes;
	CDOFVector DOFsVector, DOFsTypes;
	map< CFEFunctorVector* , vector< CFiniteElementInstance* > > VF;

};
