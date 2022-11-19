#pragma once
#include "DOF.h"

class CDOF_Fix :
	public CDOF
{
public:
	

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Fix(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Fix* Clone() const { return new CDOF_Fix(*this); }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( here "ux" ).

    */
	virtual const char* DofName()  const { return "fix"; }

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters(  here "Px" ).

    */
	virtual const char* LoadName() const { return "Mx"; }

protected:

	//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_Fix(void);

	friend class CDOFs;
};


