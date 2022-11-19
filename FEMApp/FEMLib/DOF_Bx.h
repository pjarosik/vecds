#pragma once
#include "DOF.h"

class CDOF_Bx :
	public CDOF
{
public:
	
	//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_Bx(unsigned st);

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Bx(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Bx* Clone() const { return new CDOF_Bx(*this); }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( here "ux" ).

    */
	virtual const char* DofName()  const { return "bx"; }

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters(  here "Px" ).

    */
	virtual const char* LoadName() const { return "Bx"; }

protected:
	
	friend class CDOFs;
};
