// LargeMatrixSkyLineCholesky.h: interface for the CLargeMatrixSkyLineCholesky class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LargeMatrixSkyLineCholesky_H__D5F0820B_347E_4259_9ACF_50F35138C537__INCLUDED_)
#define AFX_LargeMatrixSkyLineCholesky_H__D5F0820B_347E_4259_9ACF_50F35138C537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LargeMatrixSkyLine.h"

class CLargeMatrixSkyLineCholesky : public CLargeMatrixSkyLine  
{

public:

	CLargeMatrixSkyLineCholesky();
	CLargeMatrixSkyLineCholesky(const CLargeMatrixSkyLineCholesky &K ):CLargeMatrixSkyLine(K) { }
	virtual ~CLargeMatrixSkyLineCholesky();
	
	bool PositiveDiagonal();
	virtual bool Decompose(CProgress *pr = 0);
	virtual CLargeMatrixSkyLineCholesky* Clone() const { return new CLargeMatrixSkyLineCholesky(*this); }
	

};

#endif // !defined(AFX_LargeMatrixSkyLineCholesky_H__D5F0820B_347E_4259_9ACF_50F35138C537__INCLUDED_)
