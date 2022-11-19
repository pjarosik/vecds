#pragma once
#include "DOF.h"

class CDOF_Ax :
	public CDOF
{

public:
	

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Ax(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Ax* Clone() const { return new CDOF_Ax(*this); }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( here "ux" ).

    */
	virtual const char* DofName()  const { return "ax"; }

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters(  here "Px" ).

    */
	virtual const char* LoadName() const { return "Ax"; }

protected:

	//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_Ax();

	friend class CDOFs;
};
