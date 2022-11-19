#pragma once
#include "DOF.h"


//!  A CDOF class 
/*!
  Class represents z-displacement degree of freedom.
*/
class CDOF_Uz :
	public CDOF
{
public:
	

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Uz(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Uz* Clone() const { return new CDOF_Uz(*this); }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( here "uz" ).

    */
	virtual const char* DofName()  const { return "uz"; }

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters(  here "Pz" ).

    */
	virtual const char* LoadName() const { return "Pz"; }

protected:

	//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_Uz(void);

	friend class CDOFs;
};
