#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

template <class T>
class point2D {
public:
    T x,y;
    istream& read( istream &in );
};

template <class T>
istream& point2D<T>::read( istream &in )
{
    in >> x >> y; return in;
}

template<class T> istream& operator >> ( istream &in, point2D<T> &GD) { GD.read(in); return in; }


template <class T>
class point3D {
public:
    T x,y,z;
    istream& read( istream &in );
};

template <class T>
istream& point3D<T>::read( istream &in )
{
    in >> x >> y >> z;
    return in;
}

template<class T> istream& operator >> ( istream &in, point3D<T> &GD) { GD.read(in); return in; }

template <class T>
class line2D {
public:
    point2D<T> p1,p2;
    istream& read( istream &in );
};

template <class T>
istream& line2D<T>::read( istream &in )
{
     in >> p1 >> p2;
     return in;
}

template<class T> istream& operator >> ( istream &in, line2D<T> &GD) { GD.read(in); return in; }

template <class T>
class line3D {
public:
    point3D<T> p1,p2;
    istream& read( istream &in );
};

template <class T>
istream& line3D<T>::read( istream &in )
{
    in >> p1 >> p2;
    return in;
}

template<class T> istream& operator >> ( istream &in, line3D<T> &GD) { GD.read(in); return in; }

template <class T>
class trig3D {
public:
    point3D<T> p1,p2,p3,n;
    istream& read( istream &in );
};

template<class T>
istream& trig3D<T>::read( istream &in )
{
    in >> p1 >> p2 >> p3;
    return in;
}

template<class T> istream& operator >> ( istream &in, trig3D<T> &GD) { GD.read(in); return in; }

template <class T>
class trig2D {
public:
    point2D<T> p1,p2,p3;
    istream& read( istream &in );
};

template<class T>
istream& trig2D<T>::read( istream &in )
{
    in >> p1 >> p2 >> p3;
    return in;
}

template<class T> istream& operator >> ( istream &in, trig2D<T> &GD) { GD.read(in); return in; }

template <class T,class R>
class PenObj {
public:
    T *data;
    unsigned size;
    int st;
    R th,r,g,b,a;
    PenObj():data(0),size(0) { }
    ~PenObj() { delete data; }
    void Init(unsigned s) { delete [] data; data = new T[ s ]; size = s; }
    istream& read( istream &in );
};

template<class T,class R>
istream& PenObj<T,R>::read( istream &in )
{
    in >> st >> th >> r >> g >> b >> a >> size;
    Init(size);
    unsigned k=0;
    while( k < size )in >> data[k++];
    return in;
}

template<class T,class R> istream& operator >> ( istream &in, PenObj<T,R> &GD) { GD.read(in); return in; }

template <class T,class R>
class BrushObj {
public:
    T *data;
    unsigned size;
    int st;
    R r,g,b,a;
    BrushObj():data(0),size(0) { }
    ~BrushObj() { delete data; }
    void Init(unsigned s) { delete [] data; data = new T[ s ]; size = s; }
    istream& read( istream &in );
};

template<class T,class R>
istream& BrushObj<T,R>::read( istream &in )
{
    in >> st >> r >> g >> b >> a >> size;
    Init(size);
    unsigned k=0;
    while( k < size )in >> data[k++];
    return in;
}

template<class T,class R> istream& operator >> ( istream &in, BrushObj<T,R> &GD) { GD.read(in); return in;}

template < class T >
class GraphData
{

public:
    vector< BrushObj< trig2D< T >, T > > trs2D;
    vector< BrushObj< trig3D< T >, T > > trs3D;

    vector< PenObj< point2D< T >, T > > pts2D;
    vector< PenObj< point3D< T >, T > > pts3D;

    vector< PenObj< line2D< T >, T > > lns2D;
    vector< PenObj< line3D< T >, T > > lns3D;
    const istream& operator >> ( const istream &in );
    istream& read(  istream &in );
    GraphData():ipen( -1 ), ibrush( -1 ) {  }
protected:
    int ipen, ibrush;

};


template < class T >
istream& GraphData<T>::read( istream &in )
{
    unsigned k, size;
    in >> size;
    if ( size )
    {
        pts2D.resize( size );
        k=0;
        while( k < size ) in >> pts2D[ k++ ];
    }
    in >> size; if ( size ) { pts3D.resize( size ); k=0; while( k < size ) in >> pts3D[ k++ ]; }

    in >> size; if ( size ) { lns2D.resize( size ); k=0; while( k < size ) in >> lns2D[ k++ ]; }
    in >> size; if ( size ) { lns3D.resize( size ); k=0; while( k < size ) in >> lns3D[ k++ ]; }

    in >> size; if ( size ) { trs2D.resize( size ); k=0; while( k < size ) in >> trs2D[ k++ ]; }
    in >> size; if ( size ) { trs3D.resize( size ); k=0; while( k < size ) in >> trs3D[ k++ ]; }

    return in;
}
template<class T> istream& operator >> ( istream &in, GraphData<T> &GD) { GD.read(in); return in; }

#endif // GRAPHDATA_H
