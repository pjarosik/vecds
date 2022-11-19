/* 
 * File:   CProfileReductionRCM.h
 * Author: piotrek
 *
 * Created on November 20, 2011, 12:01 PM
 */

#ifndef CPROFILEREDUCTIONRCM_H
#define	CPROFILEREDUCTIONRCM_H

#include "ProfileReduction.h"
#include <list>

class CProfileReductionRCM : public CProfileReduction {
public:
    CProfileReductionRCM() { }
    CProfileReductionRCM(const CProfileReductionRCM& orig);
    virtual ~CProfileReductionRCM();
    virtual void Optimize();
private:
    void RemoveList ( const list<CFENode*> &source, list<CFENode*> &target );
    void MergeLists( const list<CFENode*> &source, list<CFENode*> &target );

};

#endif	/* CPROFILEREDUCTIONRCM_H */

