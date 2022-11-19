/* 
 * File:   CStatamentDOFs.h
 * Author: piotrek
 *
 * Created on November 20, 2011, 4:58 PM
 */

#ifndef CSTATAMENTDOFS_H
#define	CSTATAMENTDOFS_H

#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementDOFs:public CStatement {

public:

    CStatementDOFs( CFEMProject &FP );
    virtual ~CStatementDOFs();
    virtual void Execute( istream &i );

protected:

    CFEMProject &FP;

};

#endif	/* CSTATAMENTDOFS_H */

