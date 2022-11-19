#include "GraphDeviceSizer.h"

CGraphDeviceSizer::CGraphDeviceSizer( const CSpace *sp, const CPaintFrame *f ):CGraphDevice( f ),min2D(2),max2D(2),min3D(3),max3D(3)
{
    Reset();
}

void CGraphDeviceSizer::Reset()
{
    min2D[0] = min2D[1] = min3D[0] = min3D[1] = min3D[2] =  1e+300;
    max2D[0] = max2D[1] = max3D[0] = max3D[1] = max3D[2] = -1e+300;
}

// 2D functions


void CGraphDeviceSizer::Rectangle( double x1, double y1, double x2, double y2 )
{
    Update2D( x1, y1 );
    Update2D( x2, y2 );
}

void CGraphDeviceSizer::Triangle2D( double x1, double y1, double x2, double y2, double x3, double y3 )
{
    Update2D( x1, y1 );
    Update2D( x2, y2 );
    Update2D( x3, y3 );
}

void CGraphDeviceSizer::Line2D( double x1, double y1, double x2, double y2  )
{
    Update2D( x1, y1 );
    Update2D( x2, y2 );
}

void CGraphDeviceSizer::Polygon2D( vector<DPoint2D> &dp )
{
    unsigned k;
    for (k=0; k<dp.size(); k++)
        Update2D(dp[k].x,dp[k].y);
}

void CGraphDeviceSizer::Polyline2D( vector<DPoint2D> &dp )
{
    unsigned k;
    for (k=0; k<dp.size(); k++)
        Update2D(dp[k].x,dp[k].y);
}

void CGraphDeviceSizer::Ellipse( double x1,double y1,double a,double b )
{
        Update2D( x1-a/2.0, y1-a/2.0 );
        Update2D( x1+a/2.0, y1+a/2.0 );
}

void CGraphDeviceSizer::WriteInBox2D(double x1,double y1,double x2,double y2,const string &str, TextJustify tj )
{
    Update2D( x1, y1 );
    Update2D( x2, y2 );
}


// 3D functions

void CGraphDeviceSizer::Triangle3D( const mvector &p1, const mvector &p2, const mvector &p3 )
{
    CGraphDevice::Triangle3D(p1,p2,p3);
    Update3D(p1);
    Update3D(p2);
    Update3D(p3);
}

void CGraphDeviceSizer::Line3D( const mvector &p1, const mvector &p2 )
{
    CGraphDevice::Line3D(p1,p2);
    Update3D(p1);
    Update3D(p2);
}

void CGraphDeviceSizer::Line3D( double x1, double y1, double z1, double x2, double y2, double z2 )
{
    CGraphDevice::Line3D( x1,y1,z1,x2,y2,z2 );
    Update3D(x1,y1,z1);
    Update3D(x2,y2,z2);
}

void CGraphDeviceSizer::Point( const mvector &p, double size )
{
    CGraphDevice::Point(p,size);
    Update3D(p);
}

void CGraphDeviceSizer::WriteText( const mvector &p, const string &tx )
{
    CGraphDevice::WriteText(p,tx);
    Update3D(p);
}

void CGraphDeviceSizer::Update2D( const mvector &x )
{
    unsigned k;
    for (k=0; k<2; k++ )
    {
        if ( x[k] < min2D[k] ) min2D[k]=x[k];
        if ( x[k] > max2D[k] ) max2D[k]=x[k];
    }

}

void CGraphDeviceSizer::Update2D( double x, double y )
{
      if ( x < min2D[0] ) min2D[0]=x;
      if ( x > max2D[0] ) max2D[0]=x;

      if ( y < min2D[1] ) min2D[1]=y;
      if ( y > max2D[1] ) max2D[1]=y;
}

void CGraphDeviceSizer::Update3D( const mvector &x )
{
    unsigned k;
    for (k=0; k<3; k++ )
    {
        if ( x[k] < min3D[k] ) min3D[k]=x[k];
        if ( x[k] > max3D[k] ) max3D[k]=x[k];
    }
}


void CGraphDeviceSizer::Update3D( double x, double y, double z )
{
      if ( x < min2D[0] ) min2D[0]=x;
      if ( x > max2D[0] ) max2D[0]=x;

      if ( y < min2D[1] ) min2D[1]=y;
      if ( y > max2D[1] ) max2D[1]=y;

      if ( z < min2D[2] ) min2D[2]=z;
      if ( z > max2D[2] ) max2D[2]=z;
}

double CGraphDeviceSizer::GetSizes2D( mvector &size2D ) const
{
    unsigned k;
    size2D.SetDim(2);
    for (k=0; k<2; k++)
        size2D[k]=max2D[k]-min2D[k];
    return sqrt( size2D[0]*size2D[0] + size2D[1] * size2D[1] );
}

double CGraphDeviceSizer::GetSizes3D( mvector &size3D ) const
{
    unsigned k;
    size3D.SetDim(3);
    for (k=0; k<3; k++)
        size3D[k]=max3D[k]-min3D[k];
    return sqrt( size3D[0]*size3D[0] + size3D[1] * size3D[1] + size3D[2] * size3D[2] );
}


void CGraphDeviceSizer::DeleteContext()
{
    mvector dummy;
    diag2D = GetSizes2D( dummy );
    diag3D = GetSizes3D( dummy );
}
