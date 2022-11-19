#pragma once
#include "DOF.h"

class CDOF_Ay :
	public CDOF
{
public:
	

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Ay(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Ay* Clone() const { return new CDOF_Ay(*this); }

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
	
	//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_Ay(void);

	friend class CDOFs;
};
