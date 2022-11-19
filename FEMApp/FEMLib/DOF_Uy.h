#pragma once
#include "DOF.h"


//!  A CDOF class 
/*!
  Class represents y-displacement degree of freedom.
*/
class CDOF_Uy :
	public CDOF
{
public:
	

	//!  A CDOF class 
	/*!
		destructor.
	*/
	virtual ~CDOF_Uy(void);

	//! DOF clone member
    /*!
      \return pointer to copy ow the object
    */
	virtual CDOF_Uy* Clone() const { return new CDOF_Uy(*this); }

	//! DOF symbol for solution value.
    /*!
      \return returns string contains the DOF solution symbol ( here "uy" ).

    */
	virtual const char* DofName()  const { return "uy"; }

	//! DOF parameter.
    /*!
      \return returns string contains the DOF symbol of nodal parameters(  here "Py" ).

    */
	virtual const char* LoadName() const { return "Py"; }

protected:

	//!  A CDOF class 
	/*!
		Constructor.
	*/
	CDOF_Uy(void);

	friend class CDOFs;
};
