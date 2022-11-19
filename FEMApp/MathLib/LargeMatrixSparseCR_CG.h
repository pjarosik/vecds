/* 
 * File:   CLargeMatrixSparseCR_CG.h
 * Author: piotrek
 *
 * Created on November 18, 2011, 2:22 PM
 */

#ifndef CLARGEMATRIXSPARSECR_CG_H
#define	CLARGEMATRIXSPARSECR_CG_H

#include "LargeMatrixSparseCR.h"

class CLargeMatrixSparseCR_CG:public CLargeMatrixSparseCR  {
public:
    CLargeMatrixSparseCR_CG();
    CLargeMatrixSparseCR_CG(const CLargeMatrixSparseCR_CG& orig);
    virtual ~CLargeMatrixSparseCR_CG();
    
    virtual CLargeMatrixSparseCR_CG* Clone() const { return new CLargeMatrixSparseCR_CG(*this); }
    //!  Matrix solve
	/*!
		Calculates solution basedon right-hand vector and decomposed matrix.
	*/
        virtual bool Solve( const mvector &b, mvector &x );
    
private:

};

#endif	/* CLARGEMATRIXSPARSECR_CG_H */

