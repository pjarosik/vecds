#pragma once
#include "../MathLib/matrix.h"
#include "FEException.h"
#include <string>
#include <map>
#include <float.h>
using namespace std;


/**
 * Base class represents material constants set. Object of this class stores material constants.
 */
class CFEMatConstants
{
public:
	//! CFEMatConstants constructor
    /*!
	 
    */
	CFEMatConstants( map< string, double > &cn );

	//! CFEMatConstants copy constructor
    /*!
		\param m const reference to the copied material
    */
	CFEMatConstants( const CFEMatConstants &m ):constants( m.constants ) { }

	//! CFEMatConstants destructor
    /*!
	 
    */
	virtual ~CFEMatConstants( void );

	//! material constant accesor
    /*!
		\param cn material constant name
		\return material constant value or NaN if a material constant named \param cn is not exists in the material.
    */
	double GetValue( const string &cn ) const; 

protected:

	map< string, double > constants ;
};

inline double CFEMatConstants::GetValue( const string &cn ) const
{ 
	map< string, double >::const_iterator cni = constants.find( cn );
	if ( cni != constants.end() ) return  cni->second;
	throw CFEException("Undefined material constant :"+cn);
}