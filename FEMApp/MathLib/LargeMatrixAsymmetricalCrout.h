// LargeMatrixAsymmetricalCrout.h: interface for the CLargeMatrixAsymmetricalCrout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LargeMatrixAsymmetricalCrout_H__9BF53524_94C5_4DEB_97D5_93C99107E8A1__INCLUDED_)
#define AFX_LargeMatrixAsymmetricalCrout_H__9BF53524_94C5_4DEB_97D5_93C99107E8A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LargeMatrixAsymmetricalSkyLine.h"

class CLargeMatrixAsymmetricalCrout : public CLargeMatrixAsymmetricalSkyLine  
{
public:
	virtual bool Decompose(CProgress *pr = 0);
	virtual CLargeMatrixAsymmetricalCrout* Clone() const   { return new CLargeMatrixAsymmetricalCrout(*this);  }
	CLargeMatrixAsymmetricalCrout();
	CLargeMatrixAsymmetricalCrout(const CLargeMatrixAsymmetricalCrout &K):CLargeMatrixAsymmetricalSkyLine( K ) { }
	virtual ~CLargeMatrixAsymmetricalCrout();

    virtual void Save( const char *nm );
    virtual void Read( const char *nm );

};

#endif // !defined(AFX_LargeMatrixAsymmetricalCrout_H__9BF53524_94C5_4DEB_97D5_93C99107E8A1__INCLUDED_)
