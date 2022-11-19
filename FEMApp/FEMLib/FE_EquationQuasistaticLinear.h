#pragma once
#include "FE_Equation.h"
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include "../MathLib/matrix.h"

/**
 * Linear elastostatic equation class
 */
class CFE_EquationQuasistaticLinear :
	public CFE_Equation
{
public:
	//! CFE_EquationQuasistaticLinear constructor
    /*!
      \param nd vector of node pointers for the element
	 \param el vector of finite elements
	 \param M large matrix
	 \sa CFENode
    */
    CFE_EquationQuasistaticLinear( DomainContainer< CFENode > &nd, vector< CFiniteElementInstance* > &el );

	//! CFE_EquationQuasistaticLinear destructor
    /*!
      
    */
	virtual ~CFE_EquationQuasistaticLinear( void );

	//! DOFs global vector
	/*!
		The functions creates global DOFs vector. It is just sequence of nodal DOFs placed in one vector.
		\param opt CProfileReduction object pointer. Default = 0 - no bandwidth reduction will be applied.
	*/
	virtual void CreateDOFs( CProfileReduction *pr = 0 );

	//! CFE_Equation initialisation
    /*!
      This function initializing a equation. Memory allocation for stifness matrix and right hand vector
	 the case of multiple calculations this function skould be called once.
    */
	virtual void Initialize();

	//! Acumulates DOF value
	/*!
		This function Accumulates DOF load by adding a value to proper member of right-hand vector.
		\param nd - DOF global index
		\param value - accumulated value
	*/
//	virtual void AddLoadDof( unsigned nd, double value  ) { P[nd] += value; }

	//! Solving function
	/*!
		This function Solving an equation. 
	*/
	virtual void Solve( );

protected:

    const vector< CFiniteElementInstance* > &all_elements;
	CFEMatrix *K;	/*!< Stifness matrix  */
	CFEVector *P;		/*!< Right-hand vector(Load)  */
	CFEVector *q;		/*!< Vector of nodal displacements (solution)   */

	//! Builds Right-hand vector.
	/*!
		The function builds right-hand vector. it should be called after Build() function because the global DOFs is used.
	*/
	void ApplyBoundaryConditions();
};

