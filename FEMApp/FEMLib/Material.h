#pragma once
#include "../MathLib/matrix.h"
#include "FEException.h"
#include <string>
#include <map>
#include <float.h>
using namespace std;


/**
 * Base class represents material. Object of this class stores material constants and type of material.
 */
class CMaterial
{
public:
	//! CMaterial constructor
    /*!
	 
    */
	CMaterial( const string &tp, map< string, double > &cn );

	//! CMaterial copy constructor
    /*!
		\param m const reference to the copied material
    */
	CMaterial( const CMaterial &m ):constants( m.constants ) {  }

	//! CMaterial destructor
    /*!
	 
    */
	virtual ~CMaterial( void );

	//! material constant accesor
    /*!
		\param cn material constant name
		\return material constant value or NaN if a material constant named \param cn is not exists in the material.
    */
	double GetValue( const string &cn ) const; 
	const string& GetTypeName( ) const { return type; }
    const map< string, double >& GetConstatnts() const { return constants; }

protected:

	string type;
    map< string, double > constants;
};

inline double CMaterial::GetValue( const string &cn ) const
{ 
	map< string, double >::const_iterator cni = constants.find( cn );
	if ( cni != constants.end() ) return  cni->second;
	throw CFEException( "Undefined material constant :" + cn );
}
