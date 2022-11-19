#pragma once
#include "Space.h"

class CSpaceProjection :
	public CSpace
{
public:
	CSpaceProjection( unsigned p );
    CSpaceProjection( const CSpaceProjection &sp ):CSpace(sp.xdim,sp.Xdim),ox(sp.ox),oy(sp.oy) { }
    // CSpaceProjection( CCopyer &cpr, const CSpaceProjection &sp ):CSpace(sp.xdim,sp.Xdim),ox(sp.ox),oy(sp.oy) { }
	virtual ~CSpaceProjection( void );

    CLONE_OBJECT(CSpaceProjection)
//    COPY_OBJECT(CSpaceProjection)

	virtual void transform( const mvector &x,  mvector &X ) const;
	virtual void jacobian( const mvector &x,  matrix &j ) const;

protected:
	unsigned ox, oy;
};
