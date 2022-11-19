
#include "FEMatConstants.h"
#include "FEException.h"

CFEMatConstants::CFEMatConstants(map< string, double > &cn):constants(cn)
{
}

CFEMatConstants::~CFEMatConstants(void)
{
}

/*
void CFEMatConstants::SetMaterialConstants( const map< string, double > &mc )
{
	map< string, double >::const_iterator i;
	map< string, double >::iterator fc;
	for ( i = mc.begin(); i != mc.end(); i++ )
	{
		fc = constants.find( i->first );
		if (  fc == constants.end() ) throw CFEException( "Wrong material constant name :" + i->first + "for material :" + type );
		fc->second = i->second;
	}
}
*/