// GDC.H - SVG Graphics device context - header

#ifndef __ESVGDC_H
#define __ESVGDC_H

#include "GraphDevice.h"
#include <fstream>
#include <string>
#include "../XMLLib/xml_creator.h"

using namespace std;
class GraphDeviceSVG: public CGraphDevice
{
	string filename, fontname, text_color;
	double aspect, thickness;
	double fonts_width[ FN_NUMBER ][ 96 ];
    vector< string > penarray, brusharray;
	string   pen,  brush;
	ofstream file;
    CXML_OutputDevice *xmlout;
	unsigned prev_pen, prev_brush, prev_text, font;
    CXML_Creator *creator;
    bool embend_in_html;
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
	
    GraphDeviceSVG( const char *filename, const CPaintFrame *f=0, bool eh = false );
    virtual ~GraphDeviceSVG( );
	
private:
	void SetFontMetrics( );
protected:

};

#endif
