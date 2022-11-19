// Class automatically generated by Dev-C++ New Class wizard

#ifndef XML_DOCTYPE_H
#define XML_DOCTYPE_H

#include "xml_element.h" // inheriting class's header file
#include "xml_constant.h"
#include "iter_if.h"
/**
 * The class represents COCTYPE statment
 */
class CXML_DOCTYPE : public CXML_Element
{
	public:
		// class constructor
		CXML_DOCTYPE(const string &n,bool p,const string &i):CXML_Element(n,XML_DOCTYPE),system(p),id(i) { }
		const string& GetId() const { return id; }
		bool IsSystem() const { return system; }
		// class destructor
		virtual ~CXML_DOCTYPE( ) { }
protected:
    CXML_DOCTYPE() {  }
	string id;
	bool system;
};

typedef  iter_if< CXML_DOCTYPE, list< CXML_Element* >::const_iterator, CElemPredicator > doctype_iterator;

#endif // XML_DOCTYPE_H
