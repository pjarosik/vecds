#pragma once

#define _USE_MATH_DEFINES 
#include <cmath>


#include <cmath>
#include "Space.h"
#include "GeometricObjectPoint.h"

class CSpaceRotation3D :
	public CSpace
{
public:
    CSpaceRotation3D( double t1, double t2, double t3 );
    CSpaceRotation3D( const CSpaceRotation3D &r ):CSpace( r ),R(r.R),rx(r.rx),ry(r.ry),rz(r.rz) {  }
    // CSpaceRotation3D( CCopyer &cpr, const CSpaceRotation3D &r ):CSpace( r ),R(r.R),rx(r.rx),ry(r.ry),rz(r.rz) {  }
    virtual ~CSpaceRotation3D( void );

    CLONE_OBJECT( CSpaceRotation3D )
//    COPY_OBJECT( CSpaceRotation3D )

	void Init( double t1, double t2, double t3 );
    void Rotate( double t1, double t2, double t3 ) { Init( rx + t1 * M_PI / 360.0, ry + t2 * M_PI / 360.0, rz + t3 * M_PI / 360.0 ); }

protected:

    virtual void transform( const mvector &x,  mvector &X ) const;
    virtual void jacobian( const mvector &x,  matrix &j ) const;

protected:
    matrix  R;
	double rx, ry, rz;

};
