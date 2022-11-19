#ifndef NODESELECTOR_H
#define NODESELECTOR_H

#include "../MathLib/DomainContainer.h"
#include "FENode.h"
#include "FEException.h"

class CNodeSelector
{
public:

    class CSelectMethod
    {
    public:
        virtual void SetSelection( DomainContainer< CFENode > &nd, vector< bool > &sel ) = 0;
        virtual CSelectMethod* Clone() const=0;
    };

    class CSelectMethodByNumber :public CSelectMethod
    {
    public:
        CSelectMethodByNumber( const vector<unsigned> &l ):list(l) {  }
        virtual void SetSelection(  DomainContainer< CFENode > &nd, vector< bool > &sel );
        virtual CSelectMethodByNumber* Clone() const { return new CSelectMethodByNumber( *this ); }
        vector<  unsigned > list;
    };

    class CSelectMethodByDomain :public CSelectMethod
    {
    public:
        CSelectMethodByDomain( const string &n ):dn(n) {  }
        virtual void SetSelection(  DomainContainer< CFENode > &nd, vector< bool > &sel );
        virtual CSelectMethodByDomain* Clone() const { return new CSelectMethodByDomain( *this ); }
        string dn;
    };

    class CSelectMethodNearests:public CSelectMethod
    {
    public:
        CSelectMethodNearests( const vector< mvector > &p, CSpace *sp=0 ):points(p),space(sp) {  }
        virtual void SetSelection(  DomainContainer< CFENode > &nd, vector< bool > &sel );
        virtual CSelectMethodNearests* Clone() const { return new CSelectMethodNearests( *this ); }
        vector<  mvector > points;
        CSpace *space;
    };

    class CSelectMethodRectangular:public CSelectMethod
    {
    public:
        CSelectMethodRectangular( const  mvector &a, const  mvector &b,  CSpace *sp=0  ):p1(a), p2(b),space(sp) {  }
        virtual void SetSelection(  DomainContainer< CFENode > &nd, vector< bool > &sel );
        virtual CSelectMethodRectangular* Clone() const { return new CSelectMethodRectangular( *this ); }
        mvector p1, p2;
        CSpace *space;
    };

    class CSelectMethodPolar:public CSelectMethod
    {
    public:
        CSelectMethodPolar( const  mvector &a, double b ):p(a), d(b) {  }
        virtual void SetSelection(  DomainContainer< CFENode > &nd, vector< bool > &sel );
        virtual CSelectMethodPolar* Clone() const { return new CSelectMethodPolar( *this ); }
        mvector p;
        double d;
    };

    class CSelectMethodInPlanes :public CSelectMethod
    {
    public:
        CSelectMethodInPlanes( const vector< double > &p, const vector< unsigned > &c, double e = 1.0e-06 ):planes(p), coords(c), eps(e) {  }
        virtual void SetSelection(  DomainContainer< CFENode > &nd, vector< bool > &sel );
        virtual CSelectMethodInPlanes* Clone() const { return new CSelectMethodInPlanes( *this ); }
        vector< double > planes;
        vector< unsigned > coords;
        double eps;

    };

    class CSelectMethodInLines :public CSelectMethod
    {
    public:
        CSelectMethodInLines( const vector< double > &p1, const vector< double > &p2, const vector< unsigned > &a, const vector< unsigned > &b, double e = 1.0e-06 ):x1(p1), x2(p2), c1(a), c2(b), eps(e) {  }
        virtual void SetSelection(  DomainContainer< CFENode > &nd, vector< bool > &sel );
        virtual CSelectMethodInLines* Clone() const { return new CSelectMethodInLines( *this ); }
        vector< double > x1, x2;
        vector< unsigned > c1, c2;
        double eps;

    };


    CNodeSelector( DomainContainer< CFENode > &n, bool bsel = false );
    CNodeSelector( const CNodeSelector &ns );
    ~CNodeSelector();

    const CNodeSelector& operator=( const CNodeSelector &ns );
    void SetSelection( bool st = true );
    void AddSelector( const CSelectMethod &sm ) { lastnn=0; selections.push_back( sm.Clone() ); }

    CFENode* SetBegin() { ci = 0; if ( !nodes[ 0 ]->IsSelected() ) return Next(); else return nodes[ 0 ]; }
    CFENode* Next();

    void GetNodalSelection( ) { selection.resize( nodes.size() ); unsigned k; for(k=0; k<nodes.size(); k++) selection[k]=nodes[k]->IsSelected(); }
    bool IsSelected( unsigned k ) const { return selection[ k ]; }
    bool operator[]( unsigned k ) const { return selection[ k ]; }

protected:

    unsigned ci, lastnn;
    DomainContainer< CFENode > &nodes;
    vector< CSelectMethod* > selections;
    vector< bool > selection;
    bool select_all;
};


#endif // NODESELECTOR_H
