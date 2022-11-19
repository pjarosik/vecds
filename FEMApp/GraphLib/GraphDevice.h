// GDC.H - Graphics device context - header

#pragma once

#include "../MathLib/Space.h"
#include "graph_constants.h"
#include "Point2D.h"
#include "rect.h"
#include "color.h"
#include "pen.h"
#include "brush.h"

#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

class CPaintFrame;

enum TextJustify
{
    TJ_LT,
    TJ_CT,
    TJ_RT,
    TJ_LC,
    TJ_CC,
    TJ_RC,
    TJ_LB,
    TJ_CB,
    TJ_RB
};

class CGraphDevice
{
public:

    CGraphDevice( const CPaintFrame *f=0 );
	virtual ~CGraphDevice();

// 2D functions	

    virtual void Rectangle( const mvector &p, double a, double b );
    virtual void Rectangle( double, double, double, double )=0;
    virtual void Triangle2D( double, double, double, double, double, double )=0;
    virtual void Triangle( const mvector&x1, const mvector &x2 ,const mvector&x3 );
    virtual void Line2D( double, double, double, double )=0;
    virtual void Line( const mvector&x1, const mvector &x2 );
    virtual void Polygon2D( vector<DPoint2D> &dp )=0;
    virtual void Polyline2D( vector<DPoint2D> &dp )=0;
    virtual void Ellipse( double,double,double,double )=0;
    void WriteInBox2D(double,double,double,double,const string &str, TextJustify tj = TJ_LC );
    void WriteInBox2D(const DRect &r,const string &str, TextJustify tj = TJ_LC ) { WriteInBox2D(r.GetX(), r.GetY(), r.GetX()+r.GetL(), r.GetY()+r.GetH(), str, tj); }
    virtual void Triangle2D( const mvector &x1, const mvector &x2, const mvector &x3 );

// 3D functions

    virtual void Triangle3D( const mvector &p1, const mvector &p2, const mvector &p3 );
    virtual void Line3D( const mvector &p1, const mvector &p2 );
    virtual void Line3D( double x1, double y1, double z1, double x2, double y2, double z2 );
	virtual void Point( const mvector &p, double size );
    virtual void WriteText( const mvector &p, const string &tx );


// Atributes


	virtual void SetPen( const DPen& )=0;
	virtual void SetPredefinedPen( unsigned int )=0;
	virtual void CreatePenArray( const vector<DPen> & )=0;
	virtual void SelectArrayPen( unsigned int )=0;
	virtual void SetNullPen()=0;

	virtual void SetBrush( const DBrush& )=0;
	virtual void SetPredefinedBrush( unsigned int )=0;
	virtual void CreateBrushArray( const vector<DBrush> & )=0;
	virtual void SelectArrayBrush( unsigned int )=0;
	virtual void SetHollowBrush( )=0;
     
	virtual void SetAspect( double, double ) =0;
	virtual double GetHeight() { return h; }
	virtual double GetWidth()  { return l; }

	virtual void MakeContext()=0;
	virtual void DeleteContext()=0;
    void RotateView( double rx, double ry, double rz );
    void RotateView( double fi );
    void SetViewSpace( const CSpace *sp ) { m_viewspace = sp; }

// Deprecated

    virtual void AssignFont( const char *nm, double ws )=0;
    virtual double GetFontHeight()=0;
    virtual double GetTextWidth( const char *tx ) =0;
    virtual void SetFontHeight( double, bool absolute = false )=0;
    virtual double TranslateTextHeight( double )=0;
    virtual void Write2D( double,double,const char* )=0;
    virtual void WriteVerically2D( double,double,const char* )=0;

// constants

protected:
	
    string doubletostring53( double x ) { stringstream s; s.setf( ios::fixed ); s.precision(3); s << setw(5) << x; return s.str(); }
    string inttostring( unsigned c ) { stringstream s; s << c; return s.str(); }

protected:

	double l, h, font_height, font_height_factor;
	void Transform( const mvector &x, double &px, double &py );
	void Transform( const mvector &X, mvector &xp );
    const CSpace *m_viewspace;
	const CPaintFrame *frame;

};


