
#include "DOF.h"

CDOF::CDOF( unsigned t ):type( t ), value(0.0), solution(0.0), fixed( false ), nr( 0 ), isselected( false )
{

}

CDOF::~CDOF( void )
{

}


void CDOF::PrintInfo( ostream &out ) const
{
    out << "nr :" << nr << "\t type :" << type << "\t";
	if ( fixed ) out << " \t fixed";
	else out << " \t free";
	out << "\t value :" << value << "\t solution :" << solution << endl;
}
