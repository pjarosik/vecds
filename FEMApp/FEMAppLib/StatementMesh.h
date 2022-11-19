/* 
 * File:   CStatementMesh.h
 * Author: piotrek
 *
 * Created on November 22, 2011, 9:39 AM
 */

#ifndef CSTATEMENTMESH_H
#define	CSTATEMENTMESH_H

#include "../LangLib/Statement.h"
#include "FEMProject.h"

class CStatementMesh: public CStatement {
public:
    CStatementMesh( CFEMProject &f );
    virtual ~CStatementMesh();

    void Execute( istream &in );

    
protected:
    
    CFEMProject &FP;
};

#endif	/* CSTATEMENTMESH_H */

