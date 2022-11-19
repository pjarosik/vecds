#ifndef GRAPHDEVICEPRM_H
#define GRAPHDEVICEPRM_H

#include <map>
#include "GraphDevice.h"
#include "GraphData.h"

using namespace std;

class CGraphDeviceStorage : public CGraphDevice
{

    class pt2d {
        double p[2];
    public:
        pt2d( ) { p[0]=0.0; p[1]=0.0; }
        pt2d( double x1, double y1 ) { p[0]=x1; p[1]=y1; }
        const double x() const { return p[0]; }
        const double y() const { return p[1]; }
        bool operator==( const pt2d &p1 ) const { return (p1.x() == p[0]) && (p1.y() == p[1] );  }
    };

    class pt3d {
        double p[3];
    public:
        pt3d()  { p[0]=0.0; p[1]=0.0;  p[2]=0.0; }
        pt3d( double x1, double y1, double z1 ) { p[0]=x1; p[1]=y1;  p[2]=z1; }
        const double x() const { return p[0]; }
        const double y() const { return p[1]; }
        const double z() const { return p[2]; }
        bool operator==( const pt3d &p1 ) const { return ( p1.x() == p[0] ) && ( p1.y() == p[1] ) && ( p1.z() == p[2] );  }
    };

    class ln2d {
        pt2d a,b;
    public:
        ln2d() { }
        ln2d( double x1,double y1, double x2, double y2):a(x1,y1),b(x2,y2) { }
        const pt2d& p1() const { return a; }
        const pt2d& p2() const { return b; }

        const double x1() const { return a.x(); }
        const double y1() const { return a.y(); }

        const double x2() const { return b.x(); }
        const double y2() const { return b.y(); }

        bool operator==( const ln2d &ln ) const { return (ln.p1() == a)&&(ln.p2()==b); }
    };

    class ln3d {
        pt3d a,b;
    public:
        ln3d() { }
        ln3d( double x1,double y1, double z1, double x2, double y2, double z2):a(x1,y1,z1),b(x2,y2,z2) { }
        const pt3d& p1() const { return a; }
        const pt3d& p2() const { return b; }

        const double x1() const { return a.x(); }
        const double y1() const { return a.y(); }
        const double z1() const { return a.z(); }

        const double x2() const { return b.x(); }
        const double y2() const { return b.y(); }
        const double z2() const { return b.z(); }

        bool operator==( const ln3d &ln ) const { return (ln.p1() == a)&&(ln.p2()==b); }
    };

    class tr2d {
        pt2d a,b,c;
    public:
        tr2d() { }
        tr2d( double x1,double y1, double x2, double y2, double x3, double y3):a(x1,y1),b(x2,y2),c(x3,y3) { }
        const pt2d& p1() const { return a; }
        const pt2d& p2() const { return b; }
        const pt2d& p3() const { return c; }

        const double x1() const { return a.x(); }
        const double y1() const { return a.y(); }

        const double x2() const { return b.x(); }
        const double y2() const { return b.y(); }

        const double x3() const { return c.x(); }
        const double y3() const { return c.y(); }

        bool operator==( const tr2d &tr ) const { return (tr.p1() == a)&&(tr.p2()==b)&&(tr.p3()==c); }
    };

    class tr3d {
        pt3d a,b,c;
    public:
        tr3d() { }
        tr3d( double x1,double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3):a(x1,y1,z1),b(x2,y2,z2),c(x3,y3,z3) { }
        const pt3d& p1() const { return a; }
        const pt3d& p2() const { return b; }
        const pt3d& p3() const { return c; }

        const double x1() const { return a.x(); }
        const double y1() const { return a.y(); }
        const double z1() const { return a.z(); }

        const double x2() const { return b.x(); }
        const double y2() const { return b.y(); }
        const double z2() const { return b.z(); }

        const double x3() const { return c.x(); }
        const double y3() const { return c.y(); }
        const double z3() const { return c.z(); }

        bool operator==( const tr3d &tr ) const { return (tr.p1() == a)&&(tr.p2()==b)&&(tr.p3()==c); }
    };

    map< DPen, vector<pt2d> > points2D;
    map< DPen, vector<pt3d> > points3D;

    map< DPen, vector<ln2d> > lines2D;
    map< DPen, vector<ln3d> > lines3D;

    map< DBrush, vector<tr2d> > triangles2D;
    map< DBrush, vector<tr3d> > triangles3D;
    vector< mvector > strX;
    vector< string >  strS;


    bool to_memory;
    DPen pen;
    DBrush brush;
    vector<DPen> penarray;
    vector<DBrush> brusharray;
    string name;

public:

    CGraphDeviceStorage( const string &nm, const CPaintFrame *f=0 , bool tm = false ):CGraphDevice(f),name(nm), to_memory( tm ) { }
    virtual ~CGraphDeviceStorage( ) { }

    // 2D functions

        virtual void Rectangle( double, double, double, double );
        virtual void Triangle2D( double, double, double, double, double, double );
        virtual void Line2D( double, double, double, double );
        virtual void Polygon2D( vector<DPoint2D> &dp );
        virtual void Polyline2D( vector<DPoint2D> &dp );
        virtual void Ellipse( double,double,double,double );
        void WriteInBox2D(double,double,double,double,const string &str, TextJustify tj = TJ_LC );
        void WriteInBox2D(const DRect &r,const string &str, TextJustify tj = TJ_LC ) { WriteInBox2D(r.GetX(), r.GetY(), r.GetX()+r.GetL(), r.GetY()+r.GetH(), str, tj); }

    // 3D functions

        virtual void Triangle3D( const mvector &p1, const mvector &p2, const mvector &p3 );
        virtual void Line3D( const mvector &p1, const mvector &p2 );
        virtual void Line3D( double x1, double y1, double z1, double x2, double y2, double z2 );
        virtual void Point( const mvector &p, double size );
        virtual void WriteText( const mvector &p, const string &tx );

    // Atributes

        virtual void SetPen( const DPen& );
        virtual void SetPredefinedPen( unsigned int );
        virtual void CreatePenArray( const vector<DPen> & );
        virtual void SelectArrayPen( unsigned int );
        virtual void SetNullPen();

        virtual void SetBrush( const DBrush& );
        virtual void SetPredefinedBrush( unsigned int );
        virtual void CreateBrushArray( const vector<DBrush> & );
        virtual void SelectArrayBrush( unsigned int );
        virtual void SetHollowBrush( );

        virtual void SetAspect( double, double );
        virtual double GetHeight() { return h; }
        virtual double GetWidth()  { return l; }

        virtual void MakeContext();
        virtual void DeleteContext();
        void RotateView( double rx, double ry, double rz );

    // Deprecated

        virtual void AssignFont( const char *nm, double ws );
        virtual double GetFontHeight();
        virtual double GetTextWidth( const char *tx );
        virtual void SetFontHeight( double, bool absolute = false );
        virtual double TranslateTextHeight( double );
        virtual void Write2D( double,double,const char* );
        virtual void WriteVerically2D( double,double,const char* );

protected:

        void FlushToTxT();
        void FlushToXml();
        void Reset();
};

#endif // GRAPHDEVICEPRM_H

