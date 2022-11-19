#pragma once
#include "Space.h"

class CSpaceAxonometry :
	public CSpace
{
public:
	CSpaceAxonometry(void);
    CSpaceAxonometry(const CSpaceAxonometry &s):CSpace( s ) { }
    // CSpaceAxonometry( CCopyer &cpr, const CSpaceAxonometry &s):CSpace( cpr, s ) {  }
	virtual ~CSpaceAxonometry(void);

    CLONE_OBJECT( CSpaceAxonometry )
//     COPY_OBJECT( CSpaceAxonometry )

	virtual void transform( const mvector &x,  mvector &X )const;
	virtual void jacobian( const mvector &x,  matrix &j )const;
};
