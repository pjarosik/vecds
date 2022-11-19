#pragma once
#include "Space.h"

class CSpaceLine :
	public CSpace
{
public:
    CSpaceLine( const mvector &a, const mvector &b, CSpace *sp = 0 );
    CSpaceLine( const CSpaceLine &sp ):CSpace(sp),X1(sp.X1),X2(sp.X2) {   }
    //CSpaceLine( CCopyer &cpr, const CSpaceLine &sp ):CSpace(cpr,sp),X1(sp.X1),X2(sp.X2) {   }
	virtual ~CSpaceLine(void);

    CLONE_OBJECT( CSpaceLine )
//    COPY_OBJECT( CSpaceLine )

	virtual void transform( const mvector &x,  mvector &X )const;
	virtual void jacobian( const mvector &x,  matrix &j )const;
protected:
    mvector X1, X2;
};
