/* 
 * File:   CLargeMatrixSkyLineCholesky.h
 * Author: piotrek
 *
 * Created on December 4, 2011, 10:48 PM
 */

#ifndef CLARGEMATRIXSKYLINECHOLESKY2_H
#define	CLARGEMATRIXSKYLINECHOLESKY2_H
#include "LargeMatrixSkyLine2.h"

class CLargeMatrixSkyLineCholesky2 : public CLargeMatrixSkyLine2
{

public:

	CLargeMatrixSkyLineCholesky2();
	CLargeMatrixSkyLineCholesky2(const CLargeMatrixSkyLineCholesky2 &K ):CLargeMatrixSkyLine2(K) { }
	virtual ~CLargeMatrixSkyLineCholesky2();
	
	bool PositiveDiagonal();
	virtual bool Decompose(CProgress *pr = 0);
	virtual CLargeMatrixSkyLineCholesky2* Clone() const { return new CLargeMatrixSkyLineCholesky2(*this); }
	

};


#endif	/* CLARGEMATRIXSKYLINECHOLESKY2_H */

