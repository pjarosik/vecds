// CGraphDevice.H definition of graphic device class


#include "GraphDevice.h"
#include "PaintFrame.h"
#include <string>
#include <cmath>
#include <cstdlib>
using namespace std;

CGraphDevice::CGraphDevice( const CPaintFrame *f ) : m_viewspace( 0 ), frame( f ),font_height(0)
{  

}

CGraphDevice::~CGraphDevice() { }


void  CGraphDevice::Transform( const mvector &X, double &px, double &py )
{
	mvector xp( 2 );
	Transform( X, xp );
	px = xp[ 0 ];
	py = xp[ 1 ];
}

void  CGraphDevice::Transform( const mvector &X, mvector &xp )
{
    if ( m_viewspace ) m_viewspace->Transform( X, xp );
    else xp = X;
    xp[ 0 ] = frame ? frame->xs( xp[ 0 ] ) : xp[ 0 ];
    xp[ 1 ] = frame ? frame->ys( xp[ 1 ] ) : xp[ 1 ];
}

void CGraphDevice::Triangle3D( const mvector &p1, const mvector &p2, const mvector &p3 )
{
    DPoint2D  dp1, dp2, dp3;
    Transform( p1, dp1.x, dp1.y );
    Transform( p2, dp2.x, dp2.y );
    Transform( p3, dp3.x, dp3.y );
    Triangle2D( dp1.x, dp1.y, dp2.x, dp2.y, dp3.x, dp3.y );
}

void CGraphDevice::Triangle2D( const mvector &x1, const mvector &x2, const mvector &x3 )
{
    DPoint2D  dp1, dp2, dp3;
    Transform( x1, dp1.x, dp1.y );
    Transform( x2, dp2.x, dp2.y );
    Transform( x3, dp3.x, dp3.y );
    Triangle2D( dp1.x, dp1.y, dp2.x, dp2.y, dp3.x, dp3.y );
}

void CGraphDevice::Triangle( const mvector&x1, const mvector &x2 ,const mvector&x3 )
{
    mvector xp1, xp2, xp3;
    Transform( x1, xp1 );
    Transform( x2, xp2 );
    Transform( x3, xp3 );
    Triangle2D( xp1[0], xp1[1], xp2[0], xp2[1], xp3[0], xp3[1] );
}

void CGraphDevice::Line3D( const mvector &p1, const mvector &p2 )
{
	double xp1, yp1, xp2, yp2;
	Transform( p1, xp1, yp1 );
	Transform( p2, xp2, yp2 );
    Line2D( xp1, yp1, xp2, yp2 );
}

void CGraphDevice::Line( const mvector&x1, const mvector &x2 )
{
    mvector xp1, xp2;
    Transform( x1, xp1 );
    Transform( x2, xp2 );
    Line2D( xp1[0], xp1[1], xp2[0], xp2[1] );
}

void CGraphDevice::Line3D( double x1, double y1, double z1, double x2, double y2, double z2 )
{
	mvector p1(3), p2(3);
	p1[0]=x1; p1[1]=y1; p1[2]=z1;
	p2[0]=x2; p2[1]=y2; p2[2]=z2;
    Line3D( p1, p2 );
}

void CGraphDevice::Point( const mvector &p, double size )
{
	Rectangle( p, size, size );
}

void CGraphDevice::Rectangle( const mvector &p, double a, double b )
{
	double xp, yp;
	Transform( p, xp, yp );
	Rectangle( xp - a / 2.0, yp - b / 2.0, xp + a / 2.0, yp + b / 2.0 );
}

void CGraphDevice::WriteText( const mvector &p, const string &tx )
{
	mvector xp( 2 );
	Transform( p, xp );
    Write2D( xp[ 0 ], xp[ 1 ], tx.c_str( ) );
}

void CGraphDevice::WriteInBox2D( double x1, double y1, double x2, double y2, const string &str, TextJustify tj )
{
    double fh = y2 - y1, mtw = x2 - x1, xs = (x1+x2)/2.0, ys = (y1+y2)/2.0;
    SetFontHeight( fh, true );
    double tw = GetTextWidth( str.c_str() );
    if ( tw > mtw )
    {
        fh *= mtw / tw;
        SetFontHeight( fh, true );
    }
    tw = GetTextWidth( str.c_str() );
    switch(tj)
    {
        case TJ_LT : Write2D( x1,         y2-fh,str.c_str()); break;
        case TJ_CT : Write2D( xs-tw/2.0,  y2-fh ,str.c_str()); break;
        case TJ_RT : Write2D( x2-tw,      y2-fh ,str.c_str()); break;

        case TJ_LC : Write2D( x1,         ys-fh/2.0 ,str.c_str()); break;
        case TJ_CC : Write2D( xs-tw/2.0,  ys-fh/2.0 ,str.c_str()); break;
        case TJ_RC : Write2D( x2-tw,      ys-fh/2.0 ,str.c_str()); break;

        case TJ_LB : Write2D( x1,         y1 ,str.c_str()); break;
        case TJ_CB : Write2D( xs-tw/2.0,  y1 ,str.c_str()); break;
        case TJ_RB : Write2D( x2-tw,      y1 ,str.c_str()); break;
    }
}


