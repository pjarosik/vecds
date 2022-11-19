
#include "ScalarFunction.h"

CScalarFunction::CScalarFunction( unsigned d, const CSpace *s ):dim( d ),space( s )
{
}

CScalarFunction::~CScalarFunction( void )
{
}

void CScalarFunction::Grad( const mvector &x, mvector &G ) const
{
	matrix J;
	grad( x, G );
	if ( !space ) return;
	space->Jacobian( x, J );
	J.Inverse();
	G = J * G;
}