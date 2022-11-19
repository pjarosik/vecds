#ifndef SCALARSHAPESPACEAPPROX_H
#define SCALARSHAPESPACEAPPROX_H

#include "Space.h"
#include "MathException.h"

class CScalarShapeSpaceApprox
{

    const CSpace &ss;
    const vector< double > values;
    const string name;

public:

    CScalarShapeSpaceApprox( const CSpace &s, const vector< double > &v, const string &n = "" );
    CScalarShapeSpaceApprox( const CScalarShapeSpaceApprox & ssa ):ss( ssa.ss ), values( ssa.values ), name( ssa.name ) {  }

    const string& GetName() const { return name; }
    double interpolate( const mvector &x ) const;

};

#endif // SCALARSHAPESPACEAPPROX_H
