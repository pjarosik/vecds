#pragma once
#include "Space.h"

class CSpacePerspective :
	public CSpace
{
public:
	CSpacePerspective( double d,double h, double xp  );
	CSpacePerspective( const CSpacePerspective &sp ):CSpace(3,2),deepth(sp.deepth), height(sp.height), xpoz( sp.xpoz ) { }
    // CSpacePerspective( CCopyer &cpr, const CSpacePerspective &sp ):CSpace(cpr,sp),deepth(sp.deepth), height(sp.height), xpoz( sp.xpoz ) { }
	~CSpacePerspective(void);

    CLONE_OBJECT( CSpacePerspective )
//    COPY_OBJECT( CSpacePerspective )

	virtual void transform( const mvector &x,  mvector &X )const;
	virtual void jacobian( const mvector &x,  matrix &j )const;

protected:
	double deepth, height, xpoz;
};
