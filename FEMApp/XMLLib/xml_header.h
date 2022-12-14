// Class automatically generated by Dev-C++ New Class wizard

#ifndef CXML_HEADER_H
#define CXML_HEADER_H

#include "xml_element.h" // inheriting class's header file
#include "xml_constant.h"
#include "iter_if.h"
/**
 * The class represents a header of an xml document
 */
class CXML_Header : public CXML_Element
{
	public:
		// class constructor
		CXML_Header(const string &cn):CXML_Element("xml",XML_HEADER),content(cn) { }
		const string& GetContent() const { return content; }
		// class destructor
		virtual ~CXML_Header() { }
protected:
		string content;
		CXML_Header();
};

typedef  iter_if< CXML_Header, vector< CXML_Element* >::const_iterator, CElemPredicator > header_iterator;

#endif // CXML_HEADER_H
