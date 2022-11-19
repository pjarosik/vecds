/* 
 * File:   CLargeMatrixSparseCR_PCG.h
 * Author: piotrek
 *
 * Created on November 19, 2011, 2:25 PM
 */

#ifndef CLARGEMATRIXSPARSECR_PCG_H
#define	CLARGEMATRIXSPARSECR_PCG_H

#include "LargeMatrixSparseIncompleteLU.h"

class CLargeMatrixSparseCR_PCG:public CLargeMatrixSparseCR {
public:
    CLargeMatrixSparseCR_PCG(bool ipc=false);
    CLargeMatrixSparseCR_PCG(const CLargeMatrixSparseCR_PCG& orig);
    virtual ~CLargeMatrixSparseCR_PCG();
    
     virtual CLargeMatrixSparseCR_PCG* Clone() const { return new CLargeMatrixSparseCR_PCG(*this); }
    //!  Matrix solve
	/*!
		Calculates solution basedon right-hand vector and decomposed matrix.
	*/
     virtual bool Solve( const mvector &b, mvector &x );
private:
    bool m_ipc;
    CLargeMatrixSparseIncompleteLU m_Prc;

};

#endif	/* CLARGEMATRIXSPARSECR_PCG_H */

