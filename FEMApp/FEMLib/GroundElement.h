#pragma once

#include "Material.h"
#include "FiniteElementInstance.h"
#include "FEFunctorScalar.h"
#include "FEFunctorMatrix.h"

class CGroundElement
{
public:
    CGroundElement( const vector< CFiniteElementInstance* > &fe, unsigned gs ):felems( fe ), is_active( true ),tg(1.0),gsize(gs) { }
//	CGroundElement(CMaterial *m):mat( m ) { }
	virtual ~CGroundElement() { }
	void SetThickness( double t ); 
	void AddElement( CFiniteElementInstance *fe ) { felems.push_back( fe ); }
	bool IsActive() const { return is_active; }
	bool SetActive( bool a ) { bool ret = is_active != a; is_active = a; return ret; }
//    double Calc_Vr( ) const;
    double Calc_Rg( CFEFunctorScalar *cmp, double &C ) const;
    double CalcCompliance( CFEFunctorScalar *cmp ) const ;
	double GetT() const { return tg; }
    void AggregateStiffnessMatrix( const matrix &ke, CFEFunctorMatrix *mfk, CLargeMatrix &K );
    unsigned GetInstancesNumber() const { return felems.size(); }
    unsigned GetSize() const { return gsize; }
    const vector< CFiniteElementInstance* >& GetElements() const { return felems; }
	
protected:

    CFEFunctorScalar *cmp;
//	CMaterial *mat;
	vector< CFiniteElementInstance* > felems;
	double tg;
	bool is_active;
    unsigned gsize;
};
