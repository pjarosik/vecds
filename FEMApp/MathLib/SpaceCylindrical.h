#pragma once
#include "Space.h"

class CSpaceCylindrical :
	public CSpace
{
public:
    CSpaceCylindrical( CSpace *sp = 0 );
    CSpaceCylindrical(const CSpaceCylindrical &s ):CSpace( s ) { }
    //CSpaceCylindrical(CCopyer &cpr, const CSpaceCylindrical &s ):CSpace( s ) { }
	virtual ~CSpaceCylindrical(void);

    CLONE_OBJECT(CSpaceCylindrical)
    //COPY_OBJECT(CSpaceCylindrical)

	virtual void transform( const mvector &x,  mvector &X )const;
	virtual void jacobian( const mvector &x,  matrix &j )const;
};
