/* 
 * File:   CProfileReduction.h
 * Author: piotrek
 *
 * Created on November 19, 2011, 6:46 PM
 */

#ifndef CPROFILEREDUCTION_H
#define	CPROFILEREDUCTION_H

#include "FENode.h"
#include "FiniteElementInstance.h"

class CProfileReduction {
public:
    CProfileReduction( ) { }
    void Init( const vector<CFENode*> &nd, const vector< CFiniteElementInstance* > &es );
    void Init( const vector<CFENode*> &nd, const vector< vector< CFiniteElementInstance* > > &es );
    CProfileReduction( const CProfileReduction& orig );
    virtual ~CProfileReduction();
    
    virtual void Optimize() =0;
    const valarray<unsigned> GetNumerator() const { return numerator; }
    
protected:

    vector< CFENode* > G;
    vector< CFENode* > nodes;
    vector< CFiniteElementInstance* > elements;
    valarray< unsigned > numerator;

    virtual void CreateGraph();
};

#endif	/* CPROFILEREDUCTION_H */

