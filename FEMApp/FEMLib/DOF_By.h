#pragma once
#include "DOF.h"

class CDOF_By :
	public CDOF
{
public:
	
	//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_By(unsigned st);

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_By(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_By* Clone() const { return new CDOF_By(*this); }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( here "ux" ).

    */
	virtual const char* DofName()  const { return "ux"; }

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters(  here "Px" ).

    */
	virtual const char* LoadName() const { return "Px"; }

protected:

	friend class CDOFs;
};
