// Class automatically generated by Dev-C++ New Class wizard

#ifndef CXMLPARAMETER_H
#define CXMLPARAMETER_H

#include "xml_exceptions.h"
#include "xml_element.h" // inheriting class's header file
#include "xml_constant.h"
#include "iter_if.h"
#ifndef LINUX
	#include <values.h>
#endif
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;


/**
 * No description
 */
class CXML_Parameter: public CXML_Element
{
	public:
		// class constructor
	CXML_Parameter( const string &n, const string &v ):CXML_Element( n, XML_PARAMETER ), value( v ) { }
	const string& GetValue() const { return value; }
	double GetValueAsDouble( bool ex = true ) const;
    int    GetValueAsInt() const { return atoi( value.c_str() ); }
		// class destructor
	virtual ~CXML_Parameter() { }
protected:
    CXML_Parameter( ) { }
    string value;
};

inline double CXML_Parameter::GetValueAsDouble( bool ex ) const
{
	char *err=0;
	double res;
	res = strtod( value.c_str(), &err );
	if ( *err )
	{
		if ( ex ) throw CXML_Exception(" value of parameter : " + name + " is not a double type :" + value );
		else res = DBL_MAX;
	}
	return res;
}

typedef list< CXML_Parameter* >  ListOfParameters;

#endif // CXMLPARAMETER_H