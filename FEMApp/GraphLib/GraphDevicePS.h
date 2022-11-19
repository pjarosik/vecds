// GDC.H - Graphics device context - header

#ifndef __EPSDC_H
#define __EPSDC_H

#include "GraphDevice.h"
//#include "../UtilityLib/fstr.h"
//#include "../UtilityLib/PointerStrings.h"

#include <fstream>
#include <string>
using namespace std;

class GraphDevicePS: public CGraphDevice
{
	string filename, fontname, text_color;
	double aspect, thickness;
	double fonts_width[ FN_NUMBER ][ 96 ];
	vector<string> penarray, brusharray;
	string   pen,  brush;
	ofstream file;
	unsigned prev_pen, prev_brush, prev_text, font;

public:

    virtual void SetFontHeight( double ws, bool absolute = false );
//	virtual double GetNominalFontHeight();
	virtual double GetFontHeight();
	virtual double GetTextWidth(const char*);
    virtual void WriteVerically2D(double,double,const char*);
	virtual void SetAspect(double px,double py);
	virtual void SetBrush( const DBrush& );
	virtual void CreateBrushArray( const vector<DBrush> & );
	virtual void CreatePenArray( const vector<DPen> & );
	virtual void SelectArrayBrush( unsigned int );
	virtual void SelectArrayPen( unsigned int );
	virtual void SetPredefinedBrush( unsigned int );
	virtual void SetHollowBrush( );
	virtual void SetPen( const DPen& );
	virtual void SetNullPen();
	virtual void SetPredefinedPen( unsigned int );
    virtual void AssignFont(const char *nm,double ws);
	virtual void Rectangle( double, double, double, double );
    virtual void Line2D( double, double, double, double );
    virtual void Triangle2D( double, double, double, double, double, double );
    virtual void Polygon2D( vector<DPoint2D> &dp );
    virtual void Polyline2D( vector<DPoint2D> &dp );
	virtual void Ellipse(double,double,double,double);
    virtual void Write2D(double,double,const char*);
    virtual void WriteInBox2D(double x1, double y1, double x2, double y2, const string &str, TextJustify tj = TJ_LC );
	virtual double TranslateTextHeight( double );
	virtual double GetHeight();
	virtual double GetWidth();
	virtual void MakeContext();
	virtual void DeleteContext();
	
    GraphDevicePS( const char *filename, const CPaintFrame *f=0 );
	virtual ~GraphDevicePS( );
	
private:
	void SetFontMetrics( );
protected:

};

#endif
