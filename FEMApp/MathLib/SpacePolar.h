#pragma once
#include "Space.h"

class CSpacePolar :
	public CSpace
{
public:
    CSpacePolar( const mvector &x, double f=0, CSpace *sp=0 );
    CSpacePolar( const CSpacePolar &sp ):CSpace( sp.xdim, sp.Xdim ),xs(sp.xs),fi0(sp.fi0) {  }
    // CSpacePolar( CCopyer &cpr, const CSpacePolar &sp ):CSpace( cpr, sp ),xs( sp.xs ), fi0( sp.fi0 ) {  }
	virtual ~CSpacePolar(void);

    CLONE_OBJECT(CSpacePolar)
//    COPY_OBJECT( CSpacePolar )

	virtual void transform( const mvector &x,  mvector &X )const;
	virtual void jacobian( const mvector &x,  matrix &j )const;

protected:
    mvector xs;
    double fi0;

};
