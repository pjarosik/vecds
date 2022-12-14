// Class automatically generated by Dev-C++ New Class wizard

#ifndef BARPLOTPLOT_H
#define BARPLOTPLOT_H

#include "PaintFrame.h" // inheriting class's header file

/**
 * No description
 */
 
class CBarPlot : public CPaintFrame
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
				double desc_font_height, bar_values_font_height; // description font and values font sizes

				DPen 	default_pen;
				DBrush  default_brush;
		};
		
        virtual void CalculateContentSize (double &x, double &y, double &fw, double &fh )  { x = 0; y = 0; fw = 100; fh = 100 * aspect; }
		void Paint( CGraphDevice &gDV ) const;
		void SetPlotDescription( const string &ds ) { plot_description = ds; }
		void AddBar( double val, const string& ds ) { bar_values.push_back( val ); bar_descriptions.push_back( ds ); }
		CSetup& GetSetup() { return setup; }
		// class constructor
		CBarPlot( double x, double y, double w, double h, double a );
		// class destructor
		~CBarPlot() { }
protected:
   		CBarPlot() { }
   		CSetup setup;

		string plot_description;
   		vector<double> bar_values;
   		vector<string> bar_descriptions;
   		
   		double aspect;

};

#endif // BARPLOTPLOT_H
