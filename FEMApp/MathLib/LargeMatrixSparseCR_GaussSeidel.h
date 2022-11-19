/* 
 * File:   CLargeMatrixSparseCR_GaussSeidel.h
 * Author: piotrek
 *
 * Created on November 19, 2011, 4:13 PM
 */

#ifndef CLARGEMATRIXSPARSECR_GAUSSSEIDEL_H
#define	CLARGEMATRIXSPARSECR_GAUSSSEIDEL_H

#include "LargeMatrixSparseCR.h"

class CLargeMatrixSparseCR_GaussSeidel:public CLargeMatrixSparseCR {
public:
    CLargeMatrixSparseCR_GaussSeidel();
    CLargeMatrixSparseCR_GaussSeidel(const CLargeMatrixSparseCR_GaussSeidel& orig);
    virtual ~CLargeMatrixSparseCR_GaussSeidel();
    virtual CLargeMatrixSparseCR_GaussSeidel* Clone() const { return new CLargeMatrixSparseCR_GaussSeidel(*this); }
    //!  Matrix solve
	/*!
		Calculates solution basedon right-hand vector and decomposed matrix.
	*/
        virtual bool Solve( const mvector &b, mvector &x );
private:

};

#endif	/* CLARGEMATRIXSPARSECR_GAUSSSEIDEL_H */

