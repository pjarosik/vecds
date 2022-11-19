// Class automatically generated by Dev-C++ New Class wizard

#include "MaxMin.h" // class's header file

// class constructor
// class destructor

CMaxMin::~CMaxMin()
{
	// insert your code here
}

void CMaxMin::Reset()
{
    min =  1e300;
    max = -1e300;
}

void CMaxMin::Update( const valarray<double>  &values )
{
    unsigned k;
    for (k=0; k<values.size(); k++) Update( k, values[ k ] );

}

void CMaxMin::Update( const vector<double>  &values )
{
    unsigned k;
    for (k=0; k<values.size(); k++) Update( k, values[ k ] );

}

void CMaxMin::Update( const mvector &values )
{
    unsigned k;
    for (k=0; k<values.GetDim(); k++) Update( k, values[ k ] );

}
