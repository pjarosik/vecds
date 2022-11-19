/* 
 * File:   CFE_EquationQuasistaticNonlinear.h
 * Author: piotrek
 *
 * Created on September 9, 2010, 9:12 PM
 */

#ifndef _CFE_EQUATIONQUASISTATICNONLINEAR_H
#define	_CFE_EQUATIONQUASISTATICNONLINEAR_H

#include <vector>
#include "FE_Equation.h"
using namespace std;

class CFE_EquationQuasistaticNonlinear :
	public CFE_Equation
{
public:
    CFE_EquationQuasistaticNonlinear( DomainContainer< CFENode > &nd, double lf=1.0, unsigned st=1  );
    CFE_EquationQuasistaticNonlinear(const CFE_EquationQuasistaticNonlinear& orig);
    virtual ~CFE_EquationQuasistaticNonlinear();

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

	
	virtual void Solve( );

protected:

    CFEMatrix *Kt;  /*!< Tangent stifness matrix  */
    CFEVector *P;   /*!< Right-hand vector(Load)  */
    CFEVector *R;   /*!< Residual vector  */
    CFEVector *q;   /*!< Vector of nodal displacements (solution)   */

    vector< mvector > qs;
    unsigned steps;
    double lf;


};

#endif	/* _CFE_EQUATIONQUASISTATICNONLINEAR_H */

