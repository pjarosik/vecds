#pragma once
#include "DOF.h"


//!  A CDOF class 
/*!
  Class represents x-displacement degree of freedom.
*/
class CDOF_Ux :
	public CDOF
{
public:
	

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Ux(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Ux* Clone() const { return new CDOF_Ux(*this); }

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
	CDOF_Ux(void);

	friend class CDOFs;
};
