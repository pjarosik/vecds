#pragma once
#include "FE_Equation.h"
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include "../MathLib/matrix.h"
#include "GroundElement.h"
#include "FEFunctorScalarCompliance.h"

class CFE_EquationTopologyOptimization :
	public CFE_Equation
{
public:
    CFE_EquationTopologyOptimization( DomainContainer< CFENode > &nd, vector< CGroundElement > &el, map_named_ptr< CFiniteElement > &fe, bool ic=false );
	virtual ~CFE_EquationTopologyOptimization(void);

	//! CFE_Equation initialisation
    /*!
      This function initializing a equation. Memory allocation for any necessary obiects ( matrices ) should be placed in this function. In
	 the case of multiple calculations this function skould be called once.
    */
	virtual void Initialize();

	//! Solving function
	/*!
		This function Solving an equation. It should be used only in the case of single solution method ( e.g. Linear elastostatic ). 
		If several solution metod exists ( e.g. Integration methods Newmark, Central Difference etc. ) External class should be implemented. In this case
		CFE-Equation class should make accessible a members that are necessary to solve an equation ( e.g. tangent matrix )

	*/
	virtual void Solve( );

	//! DOFs global vector
	/*!
		The functions creates global DOFs vector. It is just sequence of nodal DOFs placed in one vector.
		\param opt CProfileReduction object pointer. Default = 0 - no bandwidth reduction will be applied.
	*/
	virtual void CreateDOFs( CProfileReduction *pr = 0 );

protected:

    map_named_ptr< CFiniteElement > &felements;
    vector< CGroundElement > &gelems;
    CFEFunctorScalarCompliance *csf;

    matrix Ke;
    CFEFunctorVector *qe;
    CFEFunctorMatrix *kf;
    CFEMatrix *K;	/*!< Stifness matrix  */
    CFEVector *P;	/*!< Right-hand vector(Load)  */
    CFEVector *q;	/*!< Vector of nodal displacements (solution)   */
    bool IsConsequent;
    double ge_volume;
    ifstream finres;

    void SolveFEM();
    void Finalize();
    void MakeFame( unsigned nfr );
};

