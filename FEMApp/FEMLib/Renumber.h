#ifndef RENUMBER_H
#define RENUMBER_H

#include <string>
#include <vector>
using namespace std;

class CRenumber
{

public:

    CRenumber( const string &filename, unsigned size );
    int operator[](unsigned i) const { return inds[ i ]; }
    int InverseIndex( unsigned k ) const { return revinds[ k ]; }
    int GetIndexWithOffset( unsigned i ) const { return inds[ i ]; }

    int MyToExternal( unsigned i ) const { return inds[ i ]; }
    int ExternalToMy( unsigned i ) const { return revinds[ i ]; }

protected:
    vector< int > inds, revinds;
    int eoffset;
};

#endif // RENUMBER_H
