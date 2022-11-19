// Class automatically generated by Dev-C++ New Class wizard

#include "HistogramPlot.h" // class's header file
#include "Histogram.h"

// class constructor

CHistogramPlot::CSetup::CSetup()
{
    bx = by = 2.0;
    ex = ey = 2.0;
    nx = 2.0;
    ny = 2.0;
    amx = 1.0;
	amy = 1.0;
    desc_font_height = 1.5;
	value_font_height = 1.0;
	vx_frm = vy_frm = "%4.3f";
   	bdiv = 10;
   	vdiv = 10;
	pen = DPen( DColor(0, 0, 0), 0, 1.5 );
	brush = DBrush( DColor(150, 150, 150), 0);
}

CHistogramPlot::CHistogramPlot( double x, double y, double w, double h, double a ):CPaintFrame( x, y, w, h ),aspect( a )
{
	description = "histogram";
}

void CHistogramPlot::Paint( CGraphDevice &gDV ) const
{
	if ( !setup.bdiv ) return;

	unsigned k;
    gDV.Rectangle( xs(0), ys(0), xs(100.0), ys( aspect  * 100.0 ) ); // - for testing only

    double tscale=scale(), h_size, plot_height = 100 * aspect , ox, oy, plot_area_width, plot_area_height, arrow = 5.0;

// Dimeension of plot elements calculations

	vector<unsigned int> counts;
	vector<double> x;
	histogram.GetCounts( x, counts, setup.bdiv );
	double hmin, hmax, dvalue;
	histogram.GetMinMax( hmin, hmax );
	dvalue = hmax - hmin;

	ox = setup.bx;
	oy = setup.by + setup.desc_font_height + setup.ny + setup.value_font_height + setup.amy;
	plot_area_width = 100 -  setup.bx - setup.ex;
	plot_area_height= plot_height - oy - setup.ey;
	
	gDV.SetPredefinedPen( PEN_THICK_DARKGRAY );
	gDV.Rectangle( xs( ox ), ys( oy ), xs( 100 - setup.ex ), ys( plot_height - setup.ey ) );
	gDV.SetPredefinedPen( PEN_DARKGRAY );

// auxiliar lines drawing

    for (k=1; k<setup.vdiv; k++)
        gDV.Line2D( xs( ox + k * plot_area_width / (double) setup.vdiv  ), ys( oy ), xs( ox + k * plot_area_width / (double) setup.vdiv  ), ys( plot_height - setup.ey ) );

// description writting

	vector< double > v;
    gDV.AssignFont( "Arial", scale( setup.value_font_height ) );
	for (k=0; k<=setup.vdiv; k++) v.push_back(  hmin + k * dvalue / (double)setup.vdiv );
	write_set_horizontally( setup.bx, setup.by + setup.desc_font_height + setup.ny, 100 - setup.ex, setup.vx_frm, v , gDV, 3 );

    gDV.AssignFont( "Arial", scale( setup.desc_font_height ) );
    h_size = gDV.GetTextWidth( description.c_str( ) ) / tscale;
    gDV.Write2D( xs( ox + ( 100 - setup.ex - ox - h_size ) / 2.0   ), ys( setup.by ), description.c_str( ) );

// bars painting

	unsigned mc = *max_element( counts.begin( ), counts.end( ) );

	double cy = 0.0;
	if ( mc ) cy = plot_area_height / (double)mc;
	
	double bar_width = plot_area_width / setup.bdiv;
	
	gDV.SetPen( setup.pen );
	gDV.SetBrush( setup.brush );
	for ( k=0; k<setup.bdiv; k++)
		gDV.Rectangle( xs( ox + k * bar_width ), ys( oy ), xs( ox + ( k + 1 ) * bar_width ), ys( oy + (double)counts[ k ] * cy  ) );
}
