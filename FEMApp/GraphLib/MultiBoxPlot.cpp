
#include "MultiBoxPlot.h"
#include <vector>
#include <algorithm>
#include "pen.h"
#include "../MathLib/Scaler.h"
#include "ValueIndicator.h"


CMultiBoxPlot::CSetup::CSetup()
{
    bx = by = 2.0;
    ex = ey = 2.0;
    ny = 2.0;
    bny = 1.0;
	bc = 0.5;
    desc_font_height = 1.5;
	box_values_font_height = 1.0;
	default_pen = DPen( DColor(0, 0, 0), 0, 1.5 );
	default_brush = DBrush( DColor(255, 255, 252), 0);
}

CMultiBoxPlot::CMultiBoxPlot( double x, double y, double w, double h, double a ):CPaintFrame( x, y, w, h ),aspect( a )
{
	plot_description = "bar plot";
}

void CMultiBoxPlot::Paint( CGraphDevice &gDV ) const
{
	if ( !box_values.size() ) return;
    unsigned k, l, a;
//    gDV.Rectangle( xs(0), ys(0), xs(100.0), ys( aspect  * 100.0 ) ); // - for testing only
    double tscale=scale(), h_size, plot_height = 100 * aspect , ox, oy, 
	       plot_area_width, plot_area_height, arrow = 5.0, hborder = 3.0, dxsize=0.0,dysize=0.0, hx, hy ;
    bool drect=false;
	DColor gray(200,200,200);
	DBrush grayb( gray,0 );
	DPen grayp( gray,0,1 );

// Dimeension of plot elements calculations

    gDV.AssignFont( "Arial", scale( setup.desc_font_height  )  );
    

    double hmin=1.e300, hmax=-1.e300, dvalue = 0.0, tmin=1.e300, tmax=-1.e300;

	for (a=0; a<box_values.size(); a++)
		for (k=0; k<box_values[ a ].size(); k++)
		{
			double dq = box_values[a][ k ].q75 - box_values[a][ k ].q25;
			if ( box_values[a][ k ].q75 + hborder * dq > hmax ) hmax = box_values[ a ][ k ].q75 + hborder * dq;
			if ( box_values[a][ k ].q25 - hborder * dq < hmin ) hmin = box_values[ a ][ k ].q25 - hborder * dq;

			if ( box_values[a][ k ].max > tmax ) tmax = box_values[a][ k ].max;
			if ( box_values[a][ k ].min < tmin ) tmin = box_values[a][ k ].min;
		} 

		if ( tmin > hmin ) hmin = tmin;
		if ( tmax < hmax ) hmax = tmax;

//   	double smin = frac

	 h_size = gDV.GetTextWidth( plot_description.c_str( ) ) / tscale;
	 
	 if ( xdesc.length() ) 
	 { 
		 hy = gDV.GetFontHeight() / tscale;
		 dxsize = gDV.GetTextWidth( xdesc.c_str( ) ) / tscale;
	 }
	 
	 if ( ydesc.length() ) 
	 {
		 hx = gDV.GetFontHeight() / tscale;
		 dysize = gDV.GetTextWidth( ydesc.c_str( ) ) / tscale;
	 }
	 
     gDV.Write2D( xs( setup.bx  + ( 100 - setup.ex - h_size ) / 2.0   ), ys( setup.by + 2.0 * hy ), plot_description.c_str( ) );

	 double dfsize = gDV.GetFontHeight( ) / tscale;

	 oy = setup.by + 1.0 * setup.desc_font_height  + 2.0 * hy + setup.ny;
	 plot_area_height = plot_height - oy - setup.ey - 2.0 * hy - 2.0 * ( gDV.GetFontHeight() / tscale + setup.bny );


     if ( xdesc.length() )  gDV.Write2D( xs( ( 100 - setup.ex - dxsize ) / 2.0   ), ys( setup.by ), xdesc.c_str( ) );

     CValueIndicator vi( hmin, hmax, ys( oy + 2.0 * dfsize ), ys( oy + plot_area_height - 1.2 * dfsize ), 3 );

	 hmin = vi.GetMin();
	 hmax = vi.GetMax();

	 dvalue = hmax - hmin;

	 plot_area_width = ( 100 -  setup.bx - 2.0 * hx - setup.ex - vi.GetWidth(gDV) / tscale ) / (double)box_values.size() ;
	 
	 const CScaler	ps( hmin, hmax, oy, oy + plot_area_height ),
				ps1( hmin, hmax, oy + 2.0 * dfsize  , oy + plot_area_height - 1.2 * dfsize  );
	 
     if ( ydesc.length() ) gDV.WriteVerically2D( xs(setup.bx+hx), ys( ( 100 - setup.ey - dysize ) / 2.0 ) , ydesc.c_str() );

    for (a=0; a<box_values.size(); a++)
    {
		string stmin,stmax;
		stringstream sst;
		sst << hmin;
		stmin = sst.str();
		stringstream sst1;
		sst1 << hmax;
		stmax = sst1.str();

		if ( dvalue == 0.0 ) return;

		ox = setup.bx + 2.0 * hx + vi.GetWidth(gDV) / tscale + a * plot_area_width ;

	// box painting

	//	double cy = plot_area_height / dvalue;
		double box_place_width = plot_area_width / (double)box_values[a].size(), box_width = box_place_width * setup.bc;
	//	double zero_pos_y = oy + setup.box_values_font_height + setup.bny - hmin * cy;

		gDV.SetPen( setup.default_pen );
		gDV.SetBrush( setup.default_brush );
		
		if ( drect )
		{
			gDV.SetPen( grayp );
			gDV.SetBrush( grayb );
			gDV.Rectangle( xs( ox ), ys( ps1.scale( hmin ) ), xs( ox + plot_area_width ), ys( ps1.scale( hmax ) ) );
			gDV.SetPredefinedBrush( BRUSH_HOLLOW );
		}

		drect = !drect;

		if ( !a ) vi.Paint( gDV, xs( setup.bx + 2.0 * hy ) );

		gDV.SetPredefinedPen( PEN_BLACK ); 
        gDV.Line2D( xs( ox ),ys( ps1.scale( 0.0 ) ),xs( ox + plot_area_width  ),ys( ps1.scale( 0.0 ) ) );

		double xbc,xb1,xb2,yb, dsize, mv,q1,q2,q3,ym, dq, dm = box_width / 10.0, sy;
		for ( k=0; k<box_values[a].size(); k++)
		{
			gDV.SetPredefinedPen( PEN_BLUE );
			xbc = ox + ( (double)k + 0.5 ) * box_place_width;
			xb1 = xbc - box_width / 2.0;
			xb2 = xb1 + box_width;
			dq = box_values[a][ k ].q75  - box_values[a][ k ].q25;
			yb = ps1.scale( box_values[a][ k ].max ); // zero_pos_y + box_values[a][ k ].max * cy;
			mv = ps1.scale( box_values[a][ k ].mv  ); // zero_pos_y + box_values[a][ k ].mv * cy;
			q1 = ps1.scale( box_values[a][ k ].q25 ); // zero_pos_y + box_values[a][ k ].q25 * cy;
			q2 = ps1.scale( box_values[a][ k ].q50 ); // zero_pos_y + box_values[a][ k ].q50 * cy;
			q3 = ps1.scale( box_values[a][ k ].q75 ); // zero_pos_y + box_values[a][ k ].q75 * cy;
			ym = ps1.scale( box_values[a][ k ].min ); // zero_pos_y + box_values[a][ k ].min * cy;
			if ( ( box_values[a][ k ].max - box_values[a][ k ].q75 ) > 1.5 * dq ) yb = ps1.scale( 1.5 * dq + box_values[a][ k ].q75 ); // zero_pos_y + ( 1.5 * dq + box_values[a][ k ].q75 ) * cy;
			if ( ( box_values[a][ k ].q25 - box_values[a][ k ].min ) > 1.5 * dq ) ym = ps1.scale( box_values[a][ k ].q25 - 1.5 * dq + box_values[a][ k ].q75 ); // zero_pos_y + ( box_values[a][ k ].q25 - 1.5 * dq + box_values[a][ k ].q75 ) * cy;
	//		gDV.Rectangle( xs( xb1 ), ys( zero_pos_y ), xs( xb2 ), ys( yb ) );
            gDV.Line2D( xs( xbc ), ys( ym ), xs( xbc ), ys( yb ) );
            gDV.Line2D( xs( xb1 ), ys( ym ), xs( xb2 ), ys( ym ) );
            gDV.Line2D( xs( xb1 ), ys( yb ), xs( xb2 ), ys( yb ) );
	//		gDV.SetPredefinedBrush( BRUSH_HOLLOW );
			gDV.Rectangle( xs( xb1 ), ys( q1 ), xs( xb2 ), ys( q3) );
            gDV.Line2D( xs( xb1 ), ys( q2 ), xs( xb2 ), ys( q2 ) );
			unsigned nu=0,nl=0;
			for (l=0; l<box_values[a][k].samples->size(); l++)
			{
				sy = (*box_values[a][k].samples)[l];
				if ( ( ( sy - box_values[a][ k ].q75 )  > hborder * dq) && ( sy > hmax ) ) nu++;
				else if  ( ( box_values[a][ k ].q25 - sy  ) > hborder * dq  && ( sy < hmin )) nl++;
					else	
					{
						if ( ( ( sy - box_values[a][ k ].q75 ) > 1.5 * dq ) || ( ( box_values[a][ k ].q25 - sy  ) > 1.5 * dq ) )
						if ( ( ( sy - box_values[a][ k ].q75 ) > 3.0 * dq ) || ( ( box_values[a][ k ].q25 - sy  ) > 3.0 * dq ) )
						{
							stringstream sst;
							sst.precision(2);
							sst << sy;
		//					gDV.Write( xs( xbc  ), ys( zero_pos_y + sy * cy ), sst.str().c_str( ) );
							gDV.SetPredefinedPen( PEN_LIGHTRED );
                            gDV.Line2D( xs( xbc-dm ), ys( ps1.scale(sy) - dm ), xs( xbc+dm ), ys( ps1.scale(sy) + dm ) );
                            gDV.Line2D( xs( xbc-dm ), ys( ps1.scale(sy) + dm ), xs( xbc+dm ), ys( ps1.scale(sy) - dm ) );
						}
						else
						{
						//	stringstream sst;
						//		sst << sy;
						//	gDV.Write( xs( xbc  ), ys( zero_pos_y + sy * cy ), sst.str().c_str( ) );

							gDV.SetPredefinedPen( PEN_LIGHTGREEN );
							gDV.Ellipse( xs( xbc-dm ), ys( ps1.scale(sy)-dm ), xs( xbc + dm ), ys( ps1.scale(sy) + dm ) );
							gDV.Ellipse( xs( xbc-dm ), ys( ps1.scale(sy)+dm ), xs( xbc + dm ), ys( ps1.scale(sy) - dm ) );
						}
					}
			}
			gDV.SetPen( DPen( DColor( 128, 128, 128 ), 0, 4 ) );
            gDV.Line2D( xs( xb1 ), ys( mv ), xs( xb2 ), ys( mv ) );
			dsize = gDV.GetTextWidth( box_values[a][k].box_descriptions.c_str( ) ) / tscale;
            if ( box_values[a][k].max >= 0.0 ) gDV.Write2D( xs( xbc -  dsize / 2.0 ), ys( yb + 0.2 ), box_values[a][k].box_descriptions.c_str( ) );
            else gDV.Write2D( xs( xbc -  dsize / 2.0 ), ys( yb - 0.1 - setup.box_values_font_height ), box_values[a][k].box_descriptions.c_str( ) );

			stringstream sst,sg,sd;
			sst << box_values[ a ][ k ].ncals;
			sg.precision(2);
			sd.precision(2);
			sg << '[' << nu << "]" << box_values[ a ][ k ].max;
			sd << '[' << nl << "]" << box_values[ a ][ k ].min;
		//	gDV.SetFontHeight( scale( setup.desc_font_height * 0.8  )  );
			double dheight = gDV.GetFontHeight(  );

			dsize = gDV.GetTextWidth( sg.str().c_str( ) ) / tscale;
            if (nu>0) gDV.Write2D( xs( xbc -  dsize / 2.0 ), ys( ps.scale( hmax ) ), sg.str().c_str() );
			dsize = gDV.GetTextWidth( sd.str().c_str( ) ) / tscale;
            if (nl>0) gDV.Write2D( xs( xbc -  dsize / 2.0 ), ys( ps.scale( hmin ) ), sd.str().c_str() );

		//	gDV.SetFontHeight( scale( setup.desc_font_height  ) * 1.1 );

			dsize = gDV.GetTextWidth( sst.str().c_str( ) ) / tscale;
            gDV.Write2D( xs( xbc -  dsize / 2.0 ), ys( ym  ) - gDV.GetFontHeight() * 1.1, sst.str().c_str() );


			gDV.SetPen( setup.default_pen );
	}
	
	gDV.SetPredefinedPen( PEN_BLACK );
	gDV.SetPredefinedBrush( BRUSH_HOLLOW );

	}
}
