#include "FEFunctorScalar.h"


CFEFunctorScalar::CFEFunctorScalar( const string &nm ):CFEFunctor< CFiniteElementInstance >( nm )
{

}


CFEFunctorScalar::~CFEFunctorScalar( void )
{

}


void CFEFunctorScalar::DebugPrint( ) const
{
	ofstream file("matrix_file_dump.txt", ios::app );
	
	file.setf( ios::scientific, ios::floatfield );
	file.setf( ios::right );
	file.precision( 10 );

	file << endl << objectname << " :" << value << endl;
}