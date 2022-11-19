#pragma once
#include "PaintFrame.h"

class CMultiBoxPlot : public CPaintFrame
{
	public:
        class CSetup
		{
			public:
				CSetup();
				int type;

				double bx, by; // margins at left and down of plot
				double ex, ey; // margins at rignt and top of plot
				double ny;     // distances between text  description and axial values
				double bny;     // distance between border of plot area and description of higest bar;
				double bc;     // bar width / bars distance  coefficient. 1 - no distance betwen bars, 0 - bars haven't width an are painted by single line only
				double desc_font_height, box_values_font_height; // description font and values font sizes

				DPen 	default_pen;
				DBrush  default_brush;
		};
		
		class CBoxData
		{
			public:
				unsigned ncals;
				double min, q25, q50, mv, q75, max;
				const vector<double> *samples;
				string box_descriptions;
				CBoxData( double a, double b, double c, double d, double e, double f, unsigned n, const vector<double> &sm, const string &ds):min( a ), q25( b ), q50( c ), mv( d ), q75( e ), max( f ), ncals(n), samples( &sm ), box_descriptions( ds ) {   }
		};

        virtual void CalculateContentSize (double &x, double &y, double &fw, double &fh )  { x = 0; y = 0; fw = 100; fh = 100 * aspect; }
		void Paint( CGraphDevice &gDV ) const;
		void SetPlotDescription( const string &ds ) { plot_description = ds; }
		void SetAxialDescription( const string &xds, const string &yds ) { xdesc = xds; ydesc = yds; }
		void AddBox( double min, double q25, double q50, double mv, double q75, double max, unsigned nc, const vector<double> &samples, const string& ds ) { box_values[ box_values.size() - 1 ].push_back( CBoxData( min, q25, q50, mv, q75, max, nc, samples, ds ) ); }
		void AddGroupBox( ) { box_values.push_back( vector<CBoxData>() ); }
		CSetup& GetSetup() { return setup; }
		// class constructor
		CMultiBoxPlot( double x, double y, double w, double h, double a );
		// class destructor
		~CMultiBoxPlot() { }
protected:
   		CMultiBoxPlot()  { }
   		CSetup setup;

		string plot_description, xdesc, ydesc;
   		vector< vector<CBoxData> >  box_values;
   		
   		double aspect;
};

