#ifndef CONTOUR_H
#define CONTOUR_H
#include<vector>
#include "../GraphLib/brush.h"
#include "../GraphLib/pen.h"
#include "../GraphLib/GraphDevice.h"
#include "../MathLib/Scaler.h"
using namespace std;


enum MAP_COLOR_MODES
{
    CM_SPECTRUM,
    CM_RED_YELLOW,
    CM_GREEN_RED,
    CM_RED_GREEN,
    CM_RED_BLUE,
    CM_BLUE_RED,
    CM_GRAY,
    CM_USER_DEFINED
};

enum PAINT_MODES
{
    PM_MAP,
    PM_LINES
};

class CContour
{
public:

    class CContourVertex {
    public:
        CContourVertex( const mvector &a, double w):x(a),v(w) {  }
        const mvector &x;
        double v;
        bool operator < ( const CContourVertex &c ) const { return v < c.v; }
        bool operator > ( const CContourVertex &c ) const { return v > c.v; }

        bool operator < ( double w ) const { return v < w; }
        bool operator > ( double w ) const { return v > w; }
    };

    CContour( const string &nm, double min, double max, unsigned d, MAP_COLOR_MODES mode = CM_RED_YELLOW, bool paint_map=true, bool paint_contour=false );
    double operator []( unsigned i ) const { return contours[ i ]; }
    void PlotContourTriangle( CGraphDevice &GDC, const CContourVertex &c1, const CContourVertex &c2, const CContourVertex &c3 );
    const string GetName() const { return name; }

    int GetDegree() const { return degree; }
    void SetPaintMode( PAINT_MODES pm ) { paint_mode = pm; }

protected:

    string name, description;
    vector< double > contours, conf, con1, con2;
    vector< DPen > pens;
    vector< DBrush > brushes;
    DColor separator;
    double vmin, vmax, dv;
    unsigned dn,dim;
    int degree, abrush;
    bool paint_map, paint_contour, is_right;
    PAINT_MODES paint_mode;

    int FindContourIndex( double v ) const;
    int FindNearestContour( double v ) const;
    void CreateContourColors( const vector< DColor > &ncols, const vector<double> &sizes, const DColor &sep );
    void FindContours( const CContourVertex &c1, const CContourVertex &c2, vector<unsigned> &v12 );
    void triangleContour( CGraphDevice &GDC, const CContourVertex &c1, const CContourVertex &c2, const CContourVertex &c3  );
    void PaintTriangle( CGraphDevice &GDC, const mvector &x1, const mvector &x2, const mvector &x3 );
    void PaintLine( CGraphDevice &GDC, const mvector &x1, const mvector &x2 );

};

inline int CContour::FindContourIndex( double v ) const
{
    unsigned k;
    for ( k = 0; k < contours.size() - 1 ; k++ )
        if ( (contours[ k ] < v) && (v  < contours[ k + 1 ] )  ) return k;
    return -1;
}

inline int CContour::FindNearestContour( double v ) const
{
    unsigned k, kmin;
    double dmin = 1E300;
    for ( k = 0; k < contours.size() ; k++ )
        if (  fabs( v - contours[ k ] ) < dmin ) { dmin = fabs( v - contours[ k ] ); kmin = k; }
    return kmin;
}


#endif // CONTOUR_H
