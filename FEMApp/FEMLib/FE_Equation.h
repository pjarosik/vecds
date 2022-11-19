#pragma once

#include "../MathLib/LargeMatrix.h"
#include "../MathLib/Timer.h"
#include "FENode.h"
#include "DOF.h"
#include "DOFVector.h"
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include "FEMatrix.h"
#include "FEVector.h"
#include "FiniteElement.h"
#include "ProfileReduction.h"
#include "PostProcess.h"
#include "../MathLib/DomainContainer.h"
#include <vector>
using namespace std;


/**
 * Base class for finite elements equations
 */
class CFE_Equation
{

public:
	//! CFE_Equation constructor
    /*!
      \param nd vector of node pointers for the element
	 \sa CFENode
      \param pp PostProcessing class object
     \sa CPostProcessing
    */
    CFE_Equation( DomainContainer< CFENode > &nd, CPostProcessing *pp=0 );

	//! CFE_Equation destructor
    /*!
      
    */
	virtual ~CFE_Equation( void );


	//! Finds CFEMatrix
	/*!
		The function finds CFEMatrix object. If not found throws CFEException;
		\param mname Name of the matrix 
	*/
	CFEMatrix* FindMatrix( const string &mname );

	//! Finds CFEVector
	/*!
		The function finds CFEVector object. If not found throws CFEException;
		\param mname Name of the vector 
	*/
	CFEVector* FindVector( const string &vname );

	//! CFE_Equation initialisation
    /*!
      This function initializing a equation. Memory allocation for any necessary obiects ( matrices ) should be placed in this function. In
	 the case of multiple calculations this function skould be called once.
    */
	virtual void Initialize()=0;

	//! Solving function
	/*!
		This function Solving an equation. It should be used only in the case of single solution method ( e.g. Linear elastostatic ). 
		If several solution metod exists ( e.g. Integration methods Newmark, Central Difference etc. ) External class should be implemented. In this case
		CFE-Equation class should make accessible a members that are necessary to solve an equation ( e.g. tangent matrix )

	*/
    virtual void Solve( ) = 0;

	//! DOFs global vector
	/*!
		The functions creates global DOFs vector. It is just sequence of nodal DOFs placed in one vector.
		\param opt CProfileReduction object pointer. Default = 0 - no bandwidth reduction will be applied.
	*/
	virtual void CreateDOFs( CProfileReduction *pr = 0 )=0;

	//! Functor assignment
	/*!
		The functions creates assignment between CFEFunctorMatrix, CLargeMatrix and vector of finite elements.
	*/
	void CreateFEFunctorMatrixAssignment( const string &nm, CFEFunctorMatrix *mf,  const vector< CFiniteElementInstance* > &vfe );

	//! Matrix Functor assignment
	/*!
		The functions creates assignment between CFEFunctorMatrix, CLargeMatrix and vector of finite elements.
	*/
	void CreateFEFunctorMatrixAssignment( const map< string , CFEFunctorMatrix* > &mf,  const vector< CFiniteElementInstance* > &vfe );

	//! Vector Functor assignment
	/*!
		The functions creates assignment between CFEFunctorMatrix, CLargeMatrix and vector of finite elements.
	*/
	void CreateFEFunctorVectorAssignment( const map<string, CFEFunctorVector *> &mfs,  const vector< CFiniteElementInstance* > &vfe );

	//! Functor assignment
	/*!
		The functions creates assignment between CFEFunctorVector, CLargeMatrix and vector of finite elements.
	*/
	void CreateFEFunctorVectorAssignment( mvector *M, CFEFunctorVector *vf,  vector< CFiniteElementInstance* > &vfe );

	//! Finite element assignment
	/*!
		The functions creates assignment between CFEFunctorVector, CLargeMatrix and vector of finite elements.
	*/
	void AssignFiniteElement ( CFiniteElement *fe );

    //! Choose fixed BCs
    /*!
        The functions selects fixed boundary conditions;
    */
    void SelectFixedBCs( const CDOFVector &dv );

    //! Check stage
    /*!
        The functions checks if the stage is allowed for the analysis
        \param dv vector of node pointers for the element
    */
    bool IsAvailebleStage( const string &st, bool ex = true );


    //! Sets Post processing object
    /*!
        The functions sets a post processing object
        \param pp Post processing object pointer
    */
    void SetPostProcessingObject( CPostProcessing *pp ) { delete postproces; postproces=pp; postproces->RegisterStages( stages ); }
    //! Adds post processing item
    /*!
        The functions checks if the stage is allowed for the analysis
        \param is CPostProcessingItem object pointer
    */
    void AddPostProcessingItem( CPostProcessingItem *it ) {  if ( postproces ) postproces->AddItem( it ); }

    //! Recomputes elements results
    /*!
        The function recomputes element results and averaged nodal results
    */
    void RecomputeResults( bool sselected_only = true );

    //! Checks if a results with given name is involved in this equation. It maybe a defree of freedom or element result.
    /*!
        \param name name of a result
    */
    bool HasResultNamed( const string &nm );

protected:

    CTimer timer;
    DomainContainer< CFENode > &all_nodes;
    vector< CFiniteElement* > felements;
    vector< string > stages;


    CDOFVector dofs_fixed;

	//	CDOFVector DOFs;

	map< string, CFEMatrix* > M;
	map< string, CFEVector* > V;

    unsigned nthreads;
    CPostProcessing *postproces;

    void BeginStage( const string &stg )  { if ( postproces ) postproces->BeginStage( this, stg ); }
    void EndStage(  );
    //void PostProcess() { if ( postproces ) postproces->PostProcessing( this ); }

    //! Solving function
    /*!
        This function Solving an equation. It should be used only in the case of single solution method ( e.g. Linear elastostatic ).
        If several solution metod exists ( e.g. Integration methods Newmark, Central Difference etc. ) External class should be implemented. In this case
        CFE-Equation class should make accessible a members that are necessary to solve an equation ( e.g. tangent matrix )

    */
//	virtual void SolutionImplementation( )=0;

	//! Matrix Creation
	/*!
		The function creates new CFEMatrix.
		\param out ofstream obiect, target of the print ( it can be file or screen )
	*/
	CFEMatrix* CreateMatrix( const string &name );

	//! Matrix Creation
	/*!
		The function creates new CFEMatrix.
		\param out ofstream obiect, target of the print ( it can be file or screen )
	*/
	CFEVector* CreateVector( const string &name );

	//! Application og boundary conditions to large matrix
	/*!
		The function applies boundary conditions to the matrix
	*/
	void ApplyBoundaryConditionsToMatrix( CLargeMatrix &K, const vector< CDOF* > &dofs );

//    unsigned EnumerateDOFs( const valarray<unsigned> &numerator );
	

};
