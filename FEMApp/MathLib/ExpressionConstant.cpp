#include "ExpressionConstant.h"
#include "MathException.h"
#include <sstream>
#include <cstdlib>

using namespace std;

CExpressionConstant::CExpressionConstant( const string &ex )
{
    std::istringstream sstr(ex);
    if (!(sstr >> value))
    {
        throw CMathException( "String does not contain constant value :" + ex );
    }

}

CExpressionConstant::~CExpressionConstant( void )
{
}

string CExpressionConstant::ToString() const 
{ 
	stringstream ss;
	ss << value;
	return  ss.str();
}
