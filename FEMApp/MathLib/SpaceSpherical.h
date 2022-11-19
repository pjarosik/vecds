#pragma once
#include "Space.h"

class CSpaceSpherical :
	public CSpace
{
public:
	CSpaceSpherical( );
    CSpaceSpherical( const CSpaceSpherical &sp ):CSpace( sp.xdim, sp.Xdim ) {  }
    //CSpaceSpherical( CCopyer &cpr, const CSpaceSpherical &sp ):CSpace( sp.xdim, sp.Xdim ) {  }
	virtual ~CSpaceSpherical(void);

    CLONE_OBJECT(CSpaceSpherical)
//    COPY_OBJECT(CSpaceSpherical)

	virtual void transform( const mvector &x,  mvector &X )const;
	virtual void jacobian( const mvector &x,  matrix &j )const;
};
