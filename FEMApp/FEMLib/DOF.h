#pragma once

#include <iostream>
using namespace std;


//!  A CDOF class
/*!
  Base class for degree of freedom.
*/
class CDOF
{
public:
	
	~CDOF( void );

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF* Clone() const =0;

	//! DOF value const accessor
    /*!
      \return returns CDOF::value
	  \sa value
    */
	double GetValue() const { return value; }

	//! Load reference accessor
    /*!
      \return returns reference to CDOF::value
	  \sa value
    */
	double& GetValue()  { return value; }

	//! Solution results of DOF
    /*!
      \return returns value of DOF results
    */
	double GetSolution() const { return solution; }

	//! Sets DOF result value (by CFE_Equation).
    /*!
      \param s sets value of DOF result
    */
	void SetSolution( double s )
		{
			solution = s;
		}

	//! Checks if the DOF is fixed.
    /*!
      \return returns true if the DOF is fixed or false if the DOF is free.

    */
	bool IsFixed() const { return fixed; }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( for displacements ux,uy etc...).

    */
	virtual const char* DofName()  const =0;

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters( for Loads Px,Py etc...).

    */
	virtual const char* LoadName() const =0;

	//! DOF type.
    /*!
      \return returns DOF type constant.
	  \sa DOF_TYPES

    */
	unsigned GetType() const { return type; }

	//! DOF type operator.
    /*!
      converts DOF object to unsigned returning type of DOF.

    */
	operator unsigned() const { return type; }

	//! DOF number.
    /*!
      \return the global number (index) of the DOF. It is assigned during solution.
    */
	unsigned GetNumber() const { return nr; }

	//! DOF number.
    /*!
      \param n DOF number. Function is used by CFE_Equation object to asign  the DOF index of the global DOFs vector.
    */
	void SetNumber( unsigned n )  { nr = n; }

	//! Fixing DOF.
    /*!
      \param f  is true if the DOF is fixed or false if is free
	  \param fv is value of DOF. Prescribed value of solution ( displacement ) if the DOF is fixed or Value of right hand vector member if DOF is free.
    */
	void SetFixed( bool f, double fv=0.0 )
	{
        fixed = f; solution = value = fv;
	}

	//! DOF value setting.
    /*!
	  \param v is value of DOF. Prescribed value of solution ( displacement ) if the DOF is fixed or Value of right hand vector member if DOF is free.
    */
	void SetValue( double v ) { value  = v; }

	//! DOF value accumulation.
    /*!
	  \param v is value of DOF to be accumulated. Prescribed value of solution ( displacement ) if the DOF is fixed or Value of right hand vector member if DOF is free.
    */
	void AddLoad( double v ) { if ( !fixed ) value += v; }

	//! Printing DOF info to output stream
    /*!
	  \param v is value of DOF to be accumulated. Prescribed value of solution ( displacement ) if the DOF is fixed or Value of right hand vector member if DOF is free.
    */
	void PrintInfo( ostream &out )const;

	//! operator ==
    /*!
	  \param returns true if compared DOFs are the same type.
    */
    bool operator==( const unsigned t ) const { return type == t; }

	//! operator ==
    /*!
	  \param returns true if compared DOFs are the same type.
    */
    bool operator==( const CDOF& df ) const { return type == df.GetType(); }


    //! element selection accesor.
    /*!
        Returns true if element is selected
    */
    bool IsSelected() const { return isselected; }


    //! element selection setup.
    /*!
        Sets element selection state.
        \param s selection state.
    */
    void SetSelection( bool s ) { isselected = s; }

protected:

	unsigned type;	/*!< DOF type constant. \sa  DOF_TYPES */
	double value,	/*!< Is value of DOF. Prescribed value of solution ( displacement ) if the DOF is fixed or Value of right hand vector member if DOF is free. */
		  solution;	/*!< Result value. In the case of linear elastostatic it is respective displacement vector member */
	bool fixed;		/*!< is true if the DOF is fixed or false if is free */
	unsigned nr;	/*!< DOF index of the global DOFs vector (declared in CFE_Equation)*/
    bool isselected; /*!< bool variable allows for mark choosen DOFs (declared in CFE_Equation)*/

	//! CDOF constructor
    /*!
      \param t type of degree of fredom
	  \sa DOF_TYPES
    */
	CDOF( unsigned t );
};
