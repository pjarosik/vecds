#ifndef ELEMINSTANCESELECTOR_H
#define ELEMINSTANCESELECTOR_H

#include "../MathLib/DomainContainer.h"
#include "FiniteElementInstance.h"
#include "FEException.h"
#include <vector>
using namespace std;

class CElemInstanceSelector
{
public:

    class CSelectMethod
    {
    public:
        vector<  CFiniteElementInstance* > sel;
        virtual void SetSelection( const DomainContainer< CFiniteElementInstance > &nd, vector<bool> &selection ) = 0;
        virtual CSelectMethod* Clone() const=0;
    };

    class CSelectMethodByNumber :public CSelectMethod
    {
    public:
        CSelectMethodByNumber( const vector<unsigned> &l ):list(l) {  }
        virtual void SetSelection( const DomainContainer< CFiniteElementInstance > &nd, vector<bool> &selection );
        virtual CSelectMethodByNumber* Clone() const { return new CSelectMethodByNumber( *this ); }
        vector<  unsigned > list;
    };

    class CSelectMethodByDomain :public CSelectMethod
    {
    public:
        CSelectMethodByDomain( const string &n ):dn(n) {  }
        virtual void SetSelection( const DomainContainer< CFiniteElementInstance > &nd, vector<bool> &selection );
        virtual CSelectMethodByDomain* Clone() const { return new CSelectMethodByDomain( *this ); }
        string dn;
    };

    CElemInstanceSelector( const DomainContainer< CFiniteElementInstance > &e, bool bsel=false ):elements(e),select_all( bsel ),lastnn(0) { }
    CElemInstanceSelector( const CElemInstanceSelector &es );
    ~CElemInstanceSelector();


    CElemInstanceSelector& operator=( const CElemInstanceSelector &es );
    void Select();
    void AddElement( CFiniteElementInstance *elem );
    size_t size() const { return selected.size(); }
    bool operator[] ( unsigned i ) { return selected[i]; }
    void AddSelector( const CSelectMethod &sm ) { selections.push_back( sm.Clone() ); }


protected:

    unsigned lastnn;
    vector< bool > selection;
    bool select_all;
    const DomainContainer< CFiniteElementInstance > &elements;
    vector< bool > selected;
    vector< CSelectMethod* > selections;
};

#endif // ELEMINSTANCESELECTOR_H
