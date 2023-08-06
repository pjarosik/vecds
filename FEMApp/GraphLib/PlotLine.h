// Class automatically generated by Dev-C++ New Class wizard

#ifndef PLOTLINE_H
#define PLOTLINE_H


#include "GraphDevice.h"
#include "../MathLib/MaxMin.h"
#include <string>
using namespace std;


/**
 * Class represents properities of one line of the plot XY 
 */

enum POINT_STYLES
{
    PT_NOTHING,
    PT_SOLID_THICK,
    PT_RECT,
    PT_CARO,
    PT_TRIG,
    PT_CIRC,
    PT_DOT,
    PT_CROSS,
    PT_X,
};


class CPlotLine
{
	public:
		class CSetup
		{
			public:
				CSetup();
				bool has_line;
				DPen line_pen, point_pen;
				unsigned point_type, point_freq;
		};
		// class constructor
		CPlotLine( );
		CPlotLine( const CPlotLine &pl );
		virtual	~CPlotLine() { }
		const CPlotLine& operator=( const CPlotLine &pl );
		void  AddPoint( double x, double y ) { X.push_back( x );  Y.push_back( y ); }
		void  AddY( double y ) { Y.push_back( y ); }
		void  SetDescription( const string ds ) { description << ds; }
		string GetDescription( ) const { return description.str(); }
		void  SetY( const vector<double> &y ) { Y = y;  }
		void  SetX( const vector<double> &x ) { X = x;  }
		void  SetXRange( double min, double max );
		void  GetPoint( unsigned k, double &x, double &y ) const { x = X[ k ]; y = Y[ k ]; }
		void  GetPoint( unsigned k, DPoint2D &p ) const { p.SetXY( X[ k ], Y[ k ] ); }
		void  Setup( const CSetup &st ) { setup = st; }
		CSetup& GetSetup() { return setup; }
		const CSetup& GetSetup() const { return setup; }
		void  UpdateMax( CMaxMin &mm ) const;
		unsigned N() const { return (unsigned)Y.size(); }
		CPlotLine& operator << ( const string &v ) { description << v; return *this; }
		CPlotLine& operator << ( const double v ) { description << v; return *this; }
		CPlotLine& operator << ( const char *v )  { description << v; return *this; }
protected:
		stringstream description;
		CSetup setup;
		vector< double > X, Y;
};


#endif // PLOTLINE_H