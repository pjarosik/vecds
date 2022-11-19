#pragma once
#include "DOF.h"
class CDOF_Fiz :
	public CDOF
{
public:
	
	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Fiz(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Fiz* Clone() const { return new CDOF_Fiz(*this); }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( here "ux" ).

    */
	virtual const char* DofName()  const { return "fiz"; }

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters(  here "Px" ).

    */
	virtual const char* LoadName() const { return "Mz"; }

protected:

	//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_Fiz(void);

	friend class CDOFs;
};


