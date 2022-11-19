#pragma once
#include "DOF.h"

class CDOF_Fiy :
	public CDOF
{
public:

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Fiy(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Fiy* Clone() const { return new CDOF_Fiy(*this); }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( here "ux" ).

    */
	virtual const char* DofName()  const { return "fiy"; }

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters(  here "Px" ).

    */
	virtual const char* LoadName() const { return "My"; }

protected:

		//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_Fiy(void);

	friend class CDOFs;
};


