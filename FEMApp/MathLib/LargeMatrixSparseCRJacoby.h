/* 
 * File:   CLargeMatrixSparseCRJacoby.h
 * Author: piotrek
 *
 * Created on November 17, 2011, 3:12 PM
 */

#ifndef CLARGEMATRIXSPARSECRJACOBY_H
#define	CLARGEMATRIXSPARSECRJACOBY_H

#include "LargeMatrixSparseCR.h"

class CLargeMatrixSparseCRJacoby:public CLargeMatrixSparseCR {
public:
    CLargeMatrixSparseCRJacoby();
    CLargeMatrixSparseCRJacoby(const CLargeMatrixSparseCRJacoby& orig);
    virtual ~CLargeMatrixSparseCRJacoby();
    virtual CLargeMatrixSparseCRJacoby* Clone() const { return new CLargeMatrixSparseCRJacoby(*this); }
    //!  Matrix solve
	/*!
		Calculates solution basedon right-hand vector and decomposed matrix.
	*/
        virtual bool Solve( const mvector &b, mvector &x );
private:

};

#endif	/* CLARGEMATRIXSPARSECRJACOBY_H */

