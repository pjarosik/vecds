#ifndef MULTIREAD_H
#define MULTIREAD_H
#include <istream>

template< class A, class B >
class TVariantRead2
{
    A a;
    B b;
public:
    TVariantRead2( ):read( -1 ) {  }
    void Read( istream &ist );
    int WhichRead() const { return read; }
    A& GetFirst() { return a; }
    B& GetSecond() { return b; }
    const A& GetFirst() const { return a; }
    const B& GetSecond() const { return b; }
protected:
    int read;
};

template< class A, class B >
void TVariantRead2< A, B >::Read( istream &ist )
{
    read = -1;
    ist >> a;
    if ( a.IsRead() ) { read = 0; return; }
    ist >> b;
    if ( b.IsRead() ) { read = 1; return; }
}

template< class A, class B >
istream& operator >> ( istream &ist, TVariantRead2< A, B > &vr )
{
    vr.Read( ist );
    return ist;
}


template< class A, class B, class C >
class TVariantRead3
{
    A &a;
    B &b;
    C &c;

public:

    TVariantRead3( A &p1, B &p2, C &p3 ):a( p1 ),b( p2 ),c( p3 ), read( -1 ) {  }
    void Read( istream &ist );
    int WhichRead() const { return read; }

protected:

    int read;
};

template< class A, class B, class C >
void TVariantRead3< A, B, C >::Read( istream &ist )
{
    read = -1;
    ist >> a;
    if ( a.IsRead() ) { read = 0; return; }
    ist >> b;
    if ( b.IsRead() ) { read = 1; return; }
    ist >> c;
    if ( c.IsRead() ) { read = 2; return; }
}

template< class A, class B, class C >
istream& operator >> ( istream &ist, TVariantRead3< A, B, C > &vr )
{
    vr.Read( ist );
    return ist;
}

#endif // MULTIREAD_H
