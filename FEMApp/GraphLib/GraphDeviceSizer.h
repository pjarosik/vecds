#ifndef GRAPHDEVICESIZER_H
#define GRAPHDEVICESIZER_H

#include "GraphDevice.h"

class CGraphDeviceSizer : public CGraphDevice
{
public:
    CGraphDeviceSizer( const CSpace *sp=0, const CPaintFrame *f=0  );

    void Reset();

// 2D functions

    virtual void Rectangle( double, double, double, double );
    virtual void Triangle2D( double, double, double, double, double, double );
    virtual void Line2D( double, double, double, double );
    virtual void Polygon2D( vector<DPoint2D> &dp );
    virtual void Polyline2D( vector<DPoint2D> &dp );
    virtual void Ellipse( double,double,double,double );
    void WriteInBox2D(double,double,double,double,const string &str, TextJustify tj = TJ_LC );

// 3D functions

    virtual void Triangle3D( const mvector &p1, const mvector &p2, const mvector &p3 );
    virtual void Line3D( const mvector &p1, const mvector &p2 );
    virtual void Line3D( double x1, double y1, double z1, double x2, double y2, double z2 );
    virtual void Point( const mvector &p, double size );
    virtual void WriteText( const mvector &p, const string &tx );

// Atributes accessors

    virtual void SetPen( const DPen& ) { }
    virtual void SetPredefinedPen( unsigned int ) { }
    virtual void CreatePenArray( const vector<DPen> & ) { }
    virtual void SelectArrayPen( unsigned int ) { }
    virtual void SetNullPen() { }

    virtual void SetBrush( const DBrush& ) { }
    virtual void SetPredefinedBrush( unsigned int ) { }
    virtual void CreateBrushArray( const vector<DBrush> & ) { }
    virtual void SelectArrayBrush( unsigned int ) { }
    virtual void SetHollowBrush( ) { }

    virtual void SetAspect( double, double )  { }
    virtual double GetHeight() { return h; }
    virtual double GetWidth()  { return l; }

    virtual void MakeContext() { Reset(); }
    virtual void DeleteContext();
    void RotateView( double rx, double ry, double rz );

// Deprecated

    virtual void AssignFont( const char *nm, double ws ) { }
    virtual double GetFontHeight() { }
    virtual double GetTextWidth( const char *tx ) { }
    virtual void SetFontHeight( double, bool absolute = false ) { }
    virtual double TranslateTextHeight( double ) { }
    virtual void Write2D( double,double,const char* ) { }
    virtual void WriteVerically2D( double,double,const char* ) { }

    const mvector& GetMin2D( ) const { return min2D; }
    const mvector& GetMin3D( ) const { return min3D; }
    const mvector& GetMax2D( ) const { return max2D; }
    const mvector& GetMax3D( ) const { return max3D; }
    double GetSizes2D( mvector &size2D ) const;
    double GetSizes3D( mvector &size3D ) const;
    double GetDiagonal3D() const { return diag2D; }
    double GetDiagonal2D() const { return diag3D; }

protected:

    mvector min2D, max2D, min3D, max3D;
    double  diag2D, diag3D;

    void Update2D( const mvector &x );
    void Update3D( const mvector &x );
    void Update2D( double x1, double y1 );
    void Update3D( double x1, double y1, double z1 );
};

#endif // GRAPHDEVICESIZER_H
