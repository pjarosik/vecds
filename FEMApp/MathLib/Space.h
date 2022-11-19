#pragma once

/**
		Base, abstact class of spaces
		author: P. Tauzowski 2008
*/

#include "Clonable.h"
#include "matrix.h"
#include <map>

using namespace std;

class CGeometricDomain;

class CSpace
{
public:

    CSpace( unsigned a, unsigned b, CSpace *s = 0, bool ro = false );
    CSpace( const CSpace &s );

	virtual ~CSpace(void);

    virtual CSpace* Clone( ) const = 0;
    CSpace* DeepClone( );

    void SetSpace( CSpace *s ) { space = s; }
    CSpace* GetSpace() { return space; }
    const CSpace* GetSpace() const { return space; }
    int GetDegree() const { return dg; }
    int GetGlobalDegree() const { return max( GetDegree(), space ? space->GetGlobalDegree() : -1 ); }

    unsigned GetXdim() const { return Xdim; }
	unsigned Getxdim() const { return xdim; }

    virtual void transformFrom( const mvector &x,  mvector &X, 	CSpace *sp = 0 ) const;
    virtual void transform( const mvector &x,  mvector &X ) const = 0;
    virtual void Transform( const mvector &x,  mvector &X ) const;
    virtual void InverseTransform( const mvector &X,  mvector &x, double eps = 0.00001 ) const;

    virtual void jacobian( const mvector &x,  matrix &j ) const =0;
    virtual void Jacobian( const mvector &x,  matrix &J ) const;

protected:

    CSpace *space;
    unsigned xdim, Xdim;

    int dg;
};
