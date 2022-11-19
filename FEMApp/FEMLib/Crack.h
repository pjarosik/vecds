#pragma once
#include "Curve.h"
#include "../FEMLib/EnrichmentFunctionsBimaterial.h"



class CFiniteElementInstance;
class CFENode;

class CCrack : public CCurve
{
public:
	CCrack(void);
	virtual ~CCrack(void);
	
	virtual double angle( double t ) const=0;
	virtual double distance( const mvector &x ) const=0;
	virtual bool IsNear( const mvector &x, double eps=1e-6 ) const=0;

	double H( const mvector &x ) const;

	void FindCrackLocation( const vector< CFiniteElementInstance* > &elems,  const vector< CFENode* > &nodes );

	const vector< CFiniteElementInstance* >& GetEnrichedElemsH() const { return He; }
	const vector< CFiniteElementInstance* >& GetEnrichedElemsT() const { return Te; }
	const vector< CFiniteElementInstance* >& GetNotEnrichedElems() const { return Re; }
	const vector< CFiniteElementInstance* >& GetEnrichedElemsT1() const { return Te1; }
	const vector< CFiniteElementInstance* >& GetEnrichedElemsT2() const { return Te2; }

	const vector< CFiniteElementInstance* >& GetEnrichedElemsJh() const { return hFE; }
	const vector< CFiniteElementInstance* >& GetEnrichedElemsJr() const { return rFE; }

	const vector< CFENode* >& GetEnrichedNodesH() const { return Hn; }
	const vector< CFENode* >& GetEnrichedNodesT() const { return Tn; }

	void Subtriangularize();
	virtual void EnrichNodes()=0;
	virtual void ComputeSIFs( double dist )=0;

	double GetK1() const { return K1; }
	double GetK2() const { return K2; }
	double GetK0() const { return K0; }

	CEnrichmentFunctionsBimaterial* GetTfn1() const { return tip1; }
	CEnrichmentFunctionsBimaterial* GetTfn2() const { return tip2; }
protected:
	vector< CFiniteElementInstance* > He, Te, Te1, Te2, Re;
	vector< CFiniteElementInstance* > hFE, rFE;
	vector< CFENode* > Hn, Tn;
	CEnrichmentFunctionsBimaterial *tip1, *tip2;
	double K1, K2, K0;
	unsigned ndiv;
};
