#pragma once
#include "Space.h"

class CSpaceExtend2D3D :
	public CSpace
{
public:
	CSpaceExtend2D3D( unsigned a, double v );
    CSpaceExtend2D3D( const CSpaceExtend2D3D &s  ):CSpace( s ),	axis(s.axis), ox( s.ox), oy( s.oy ),value( s.value ) {  }
    //CSpaceExtend2D3D( CCopyer &cpr, const CSpaceExtend2D3D &s  ):CSpace( s ),	axis(s.axis), ox( s.ox), oy( s.oy ),value( s.value ) {  }
	virtual ~CSpaceExtend2D3D( void );

    CLONE_OBJECT(CSpaceExtend2D3D)
    //COPY_OBJECT(CSpaceExtend2D3D)

	virtual void transform( const mvector &x,  mvector &X ) const;
	virtual void jacobian( const mvector &x,  matrix &j ) const;
protected:
    unsigned axis, ox, oy;
	double	value;
};
