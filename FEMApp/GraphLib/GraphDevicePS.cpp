// GDC.H definition of graphic device class

#include "GraphDevicePS.h"
#include <cmath>
#include <cassert>

GraphDevicePS::GraphDevicePS(const char* fl, const CPaintFrame *f ):CGraphDevice( f ),	filename( fl ), text_color("col_black"), pen("col_black"),
												fontname("Arial"), brush("null"),
												aspect(1.0), thickness(0.75)
{
    SetFontMetrics( );
    l = 1000;  h = 1000;
}

GraphDevicePS::~GraphDevicePS( ) { }

void GraphDevicePS::SetBrush( const DBrush &br )
{

    brush  = doubletostring53( double( br.GetColor().GetR() )  ) + ' ';
    brush += doubletostring53( double( br.GetColor().GetG() )  ) + ' ';
    brush += doubletostring53( double( br.GetColor().GetB() )  ) + " srgb\n";

}

void GraphDevicePS::CreatePenArray( const vector<DPen> &arr  )
{
	unsigned k;
	string p;
	for ( k=0; k<arr.size(); k++)
	{
        p  = doubletostring53( double( arr[ k ].GetColor().GetR() )  ) + ' ';
        p += doubletostring53( double( arr[ k ].GetColor().GetG() )  ) + ' ';
        p += doubletostring53( double( arr[ k ].GetColor().GetB() )  ) + " srgb\n";
		p += doubletostring53( arr[k].GetThickness() ) + " slw\n";
		penarray.push_back( p );
	}
}

void GraphDevicePS::CreateBrushArray( const vector<DBrush> &arr )
{

	string b;
	unsigned k;
	for ( k=0; k<arr.size(); k++)
	{
        b  = doubletostring53( double( arr[ k ].GetColor().GetR() )  ) + ' ';
        b += doubletostring53( double( arr[ k ].GetColor().GetG() )  ) + ' ';
        b += doubletostring53( double( arr[ k ].GetColor().GetB() )  ) + " srgb\n";
		penarray.push_back( b );
	}

}


void GraphDevicePS::SelectArrayBrush( unsigned int col )
{
	#ifdef _DEBUG
		assert( col < brusharray.size() );
		assert( col > -1 );
	#endif
	brush = brusharray[ col ];
}

void GraphDevicePS::SelectArrayPen( unsigned int col )
{
	#ifdef _DEBUG
		assert( col < penarray.size() );
	#endif
	pen   = penarray[ col ];
}

void GraphDevicePS::SetPredefinedBrush( unsigned int col )
{
	if ( prev_brush == col ) return;

	switch( col )
	{
						case BRUSH_SOLID_BLACK        : brush = "col_black";        break;
						case BRUSH_SOLID_BLUE         : brush = "col_blue";         break;
						case BRUSH_SOLID_GREEN        : brush = "col_green";        break;
						case BRUSH_SOLID_CYAN         : brush = "col_cyan";         break;
						case BRUSH_SOLID_RED          : brush = "col_red";          break;
						case BRUSH_SOLID_MAGENTA      : brush = "col_magenta";      break;
						case BRUSH_SOLID_BROWN        : brush = "col_yellow";       break;
						case BRUSH_SOLID_LIGHTGRAY    : brush = "col_darkgray";     break;
						case BRUSH_SOLID_DARKGRAY     : brush = "col_darkgray";     break;
						case BRUSH_SOLID_LIGHTBLUE    : brush = "col_lightblue";    break;
						case BRUSH_SOLID_LIGHTGREEN   : brush = "col_lightgreen";   break;
						case BRUSH_SOLID_LIGHTCYAN    : brush = "col_lightcyan";    break;
						case BRUSH_SOLID_LIGHTRED     : brush = "col_lightred";     break;
						case BRUSH_SOLID_LIGHTMAGENTA : brush = "col_lightmagenta"; break;
						case BRUSH_SOLID_YELLOW       : brush = "col_lightyellow";  break;
						case BRUSH_SOLID_WHITE        : brush = "col_white";        break;
						case BRUSH_HOLLOW			  : brush = "null";             break;

	}
//	if (pen=="null") pen = brush;
	prev_brush = col;
}

void GraphDevicePS::SetHollowBrush( )
{
	brush = "null";
}

void GraphDevicePS::SetPen( const DPen &pn )
{
    pen  = doubletostring53( double( pn.GetColor().GetR() ) ) + ' ';
    pen += doubletostring53( double( pn.GetColor().GetG() ) ) + ' ';
    pen += doubletostring53( double( pn.GetColor().GetB() ) ) + " srgb\n";
	penarray.push_back( pen );
	file << doubletostring53( pn.GetThickness() ) << " slw\n";

}

void GraphDevicePS::SetNullPen()
{
	pen = "null";
}

void GraphDevicePS::SetPredefinedPen( unsigned int col )
{
	if ( prev_pen == col ) return;

	switch( col )
	{
						case PEN_BLACK        : pen = "col_black";        break;
						case PEN_BLUE         : pen = "col_blue";         break;
						case PEN_GREEN        : pen = "col_green";        break;
						case PEN_CYAN         : pen = "col_cyan";         break;
						case PEN_RED          : pen = "col_red";          break;
						case PEN_MAGENTA      : pen = "col_magenta";      break;
						case PEN_BROWN        : pen = "col_yellow";       break;
						case PEN_LIGHTGRAY    : pen = "col_darkgray";     break;
						case PEN_DARKGRAY     : pen = "col_darkgray";     break;
						case PEN_LIGHTBLUE    : pen = "col_lightblue";    break;
						case PEN_LIGHTGREEN   : pen = "col_lightgreen";   break;
						case PEN_LIGHTCYAN    : pen = "col_lightcyan";    break;
						case PEN_LIGHTRED     : pen = "col_lightred";     break;
						case PEN_LIGHTMAGENTA : pen = "col_lightmagenta"; break;
						case PEN_YELLOW       : pen = "col_lightyellow";  break;
						case PEN_WHITE        : pen = "col_white";        break;

						case PEN_THICK_BLACK        : pen = "col_black";        break;
						case PEN_THICK_BLUE         : pen = "col_blue";         break;
						case PEN_THICK_GREEN        : pen = "col_green";        break;
						case PEN_THICK_CYAN         : pen = "col_cyan";         break;
						case PEN_THICK_RED          : pen = "col_red";          break;
						case PEN_THICK_MAGENTA      : pen = "col_magenta";      break;
						case PEN_THICK_BROWN        : pen = "col_yellow";       break;
						case PEN_THICK_LIGHTGRAY    : pen = "col_darkgray";     break;
						case PEN_THICK_DARKGRAY     : pen = "col_darkgray";     break;
						case PEN_THICK_LIGHTBLUE    : pen = "col_lightblue";    break;
						case PEN_THICK_LIGHTGREEN   : pen = "col_lightgreen";   break;
						case PEN_THICK_LIGHTCYAN    : pen = "col_lightcyan";    break;
						case PEN_THICK_LIGHTRED     : pen = "col_lightred";     break;
						case PEN_THICK_LIGHTMAGENTA : pen = "col_lightmagenta"; break;
						case PEN_THICK_YELLOW       : pen = "col_lightyellow";  break;
						case PEN_THICK_WHITE        : pen = "col_white";        break;
	}
	prev_pen = col;
}


void GraphDevicePS::AssignFont(const char *nm, double ws)
{
          font_height_factor = 0.729;  
          font_height = ws;      // * 0.729; 
		  file << "/" << nm << " ff " << fabs( ws / font_height_factor ) << " scf sf\n";
          if ( !strcmp( nm, "Arial"   ) )   font = FN_ARIAL;
          if ( !strcmp( nm, "Helvetica"   ) )   font = FN_ARIAL;
          if ( !strcmp( nm, "Courier" ) )   font = FN_COURIER;
          if ( !strcmp( nm, "Times"   ) )   font = FN_TIMES;
          if ( !strcmp( nm, "Symbol"  ) )   font = FN_SYMBOL;
          fontname = nm;
}

void GraphDevicePS::Rectangle( double x1, double y1, double x2, double y2 )
{
    if ( brush != "null" ) file << " n " << x1 << " " << y1 << " m " << x2 << " " << y1 << " l " << x2 << " " << y2 << " l " << x1 << " " << y2 << " l cp gs " << brush << " ef\n";
    if ( pen != "null" )   file << " n " << x1 << " " << y1 << " m " << x2 << " " << y1 << " l " << x2 << " " << y2 << " l " << x1 << " " << y2 << " l cp gs " << pen   << " s gr\n";
}

void GraphDevicePS::Line2D( double a, double b, double c, double d )
{
	file << " n " << a << " " << b << " m " << c << " " << d << " l gs " << pen << " s gr\n";
}

void GraphDevicePS::Polygon2D( vector<DPoint2D> &pts )
{
    unsigned int k, n = (unsigned)pts.size();
	if ( pen != "null" )
	{
		file << " n " << pts[0].GetX() << " " << pts[0].GetY() << " m \n";
		for ( k=1; k<n; k++) file  << pts[k].GetX() << " " << pts[k].GetY() << " l \n";
		file << " cp gs " << pen << " s gr ";
		if ( brush != "null" ) file << brush << "  ef\n";
		else file << "\n";
	} else {
		file << " n " << pts[ 0 ].GetX() << " " << pts[ 0 ].GetY() << " m \n";
		for ( k=1; k<n; k++) file  <<  pts[ k ].GetX() << " " << pts[ k ].GetY() << " l \n";
		file << " cp gs  " << brush << " s gr ";
		if ( brush != "null" ) file << brush << " ef\n";
		else file << "\n";
	}
}

void GraphDevicePS::Triangle2D( double x1, double y1, double x2, double y2, double x3, double y3 )
{
    if ( pen != "null" )
    {
        file << " n " << x1 << " " << y1 << " m \n";
        file  << x2 << " " << y2 << " l \n";
        file  << x3 << " " << y3 << " l \n";
        file << " cp gs " << pen << " s gr ";
        if ( brush != "null" ) file << brush << "  ef\n";
        else file << "\n";
    } else {
        file << " n " << x1 << " " << y1 << " m \n";
        file  <<  x2 << " " << y2 << " l \n";
        file  <<  x3 << " " << y3 << " l \n";
        file << " cp gs  " << brush << " s gr ";
        if ( brush != "null" ) file << brush << " ef\n";
        else file << "\n";
    }
}

void GraphDevicePS::Polyline2D( vector<DPoint2D> &pts )
{
    unsigned int k, n=(unsigned)pts.size();
	file << " n " << pts[0].GetX() << " " << pts[0].GetY() << " m \n";
	for ( k=0; k<n; k++) file  << pts[k].GetX() << " " << pts[k].GetY() << " l \n";
	file << " cp gs " << pen << " s gr ";
}

void GraphDevicePS::Ellipse(double x1,double y1,double x2,double y2)
{
	double x = (x1 + x2) / 2.0, y=(y1+y2)/2.0, a = fabs( x2 - x1 ) / 2.0, b = fabs( y2 - y1 ) / 2.0;
	file << " n " <<  x  << " " <<   y  << " " <<  a << " " <<  b << " 0 360  DrawEllipse gs " << pen << " s gr ";
	if ( brush != "null" ) file << brush << "  ef\n";
	else file << "\n";
}

void GraphDevicePS::Write2D(double x,double y,const char *t)
{
  //Rectangle( x, y, x + GetTextWidth( t ) , y + font_height  );
  file << x << " " << y << " m gs (" << t << ") " << text_color << " sh gr\n";
 
}

void GraphDevicePS::WriteInBox2D(double x1,double y1,double x2,double y2,const string &str, TextJustify tj )
{
    //Rectangle( x1, y1, x2,  y2 );
    double tfh = y2 - y1;
    SetFontHeight( tfh, true );
    double twd=GetTextWidth( str.c_str() );
    file << "/" << fontname << " ff " << fabs( tfh / font_height_factor ) << " scf sf\n";
    switch( tj )
    {
        case TJ_LC : file << x1 << " " << y1 << " m gs (" << str.c_str() << ") " << text_color << " sh gr\n"; break;
        case TJ_CC : file << ( x1 + x2 - twd ) / 2.0 << " " << y1 << " m gs (" << str.c_str() << ") " << text_color << " sh gr\n"; break;
        case TJ_RC : file << x2 - twd << " " << y1 << " m gs (" << str.c_str() << ") " << text_color << " sh gr\n"; break;
    }
}

double  GraphDevicePS::TranslateTextHeight( double w )
{
//	return w / font_height_factor;
	 return -w * 1.5;
}

double  GraphDevicePS::GetHeight()
{
	return  h;
}

double  GraphDevicePS::GetWidth()
{
	return  l;
}

void GraphDevicePS::MakeContext()
{
	double scl = 0.5;
	file.open( filename.c_str() );
//	file.precision( 6 );
	file << "%!PS-Adobe-2.0 EPSF-2.0\n";
	file << "%%Title: " << filename << "\n";
	file << "%%Creator: FemLib \n";
    file << "%%CreationDate: Fr Oct 21 14:17:00 2014\n";
	file << "%%For: FemLib output\n";
	file << "%%Orientation: Landscape\n";
	file << "%%BoundingBox: 0 0 " << l*scl << " " << h*scl << "\n";
	file << "%%Pages: 0\n";
	file << "%%BeginSetup\n";
	file << "%%EndSetup\n";
	file << "%%Magnification: 1.0000\n";
	file << "%%EndComments\n";
	file << "/$F2psDict 200 dict def\n";
	file << "$F2psDict begin\n";
	file << "$F2psDict /mtrx matrix put\n";
	file << "/col_black        {0.000 0.000 0.000 srgb} bind def\n";
	file << "/col_blue         {0.000 0.000 0.500 srgb} bind def\n";
	file << "/col_red          {0.500 0.000 0.000 srgb} bind def\n";
	file << "/col_green        {0.000 0.500 0.000 srgb} bind def\n";
	file << "/col_cyan         {0.000 0.500 0.500 srgb} bind def\n";
	file << "/col_magenta      {0.500 0.000 0.500 srgb} bind def\n";
	file << "/col_yellow       {0.500 0.500 0.000 srgb} bind def\n";
	file << "/col_darkgray     {0.500 0.500 0.500 srgb} bind def\n";
	file << "/col_lightblue    {0.000 0.000 1.000 srgb} bind def\n";
	file << "/col_lightred     {1.000 0.000 0.000 srgb} bind def\n";
	file << "/col_lightgreen   {0.000 1.000 0.000 srgb} bind def\n";
	file << "/col_lightcyan    {0.000 1.000 1.000 srgb} bind def\n";
	file << "/col_lightmagenta {1.000 0.000 1.000 srgb} bind def\n";
	file << "/col_lightyellow  {1.000 1.000 0.000 srgb} bind def\n";
	file << "/col_lightgray    {0.750 0.750 0.750 srgb} bind def\n";
	file << "/col_white        {1.000 1.000 1.000 srgb} bind def\n";
	file << "/col_null         {1.000 1.000 1.000 srgb} bind def\n";
	file << "/DrawEllipse {\n";
	file << " /endangle exch def\n";
	file << " /startangle exch def\n";
	file << " /yrad exch def\n";
	file << " /xrad exch def\n";
	file << " /y exch def\n";
	file << " /x exch def\n";
	file << " /savematrix mtrx currentmatrix def\n";
	file << " x y tr xrad yrad sc 0 0 1 startangle endangle arc\n";
	file << " closepath\n";
	file << " savematrix setmatrix\n";
	file << "} def\n\n\n";

	file << "end\n";
	file << "save\n";
//	file << "0 " << h * scl << " translate\n";
//	file << "1 -1 scale\n\n";

	file << "/cp {closepath} bind def\n";
	file << "/ef {eofill} bind def\n";
	file << "/gr {grestore} bind def\n";
	file << "/gs {gsave} bind def\n";
	file << "/sa {save} bind def\n";
	file << "/rs {restore} bind def\n";
	file << "/l {lineto} bind def\n";
	file << "/m {moveto} bind def\n";
	file << "/rm {rmoveto} bind def\n";
	file << "/n {newpath} bind def\n";
	file << "/s {stroke} bind def\n";
	file << "/sh {show} bind def\n";
	file << "/slc {setlinecap} bind def\n";
	file << "/slj {setlinejoin} bind def\n";
	file << "/slw {setlinewidth} bind def\n";
	file << "/srgb {setrgbcolor} bind def\n";
	file << "/rot {rotate} bind def\n";
	file << "/sc {scale} bind def\n";
	file << "/sd {setdash} bind def\n";
	file << "/ff {findfont} bind def\n";
	file << "/sf {setfont} bind def\n";
	file << "/scf {scalefont} bind def\n";
	file << "/sw {stringwidth} bind def\n";
	file << "/tr {translate} bind def\n";
	file << "/tnt {dup dup currentrgbcolor\n";
	file << "  4 -2 roll dup 1 exch sub 3 -1 roll mul add\n";
	file << "  4 -2 roll dup 1 exch sub 3 -1 roll mul add\n";
	file << "  4 -2 roll dup 1 exch sub 3 -1 roll mul add srgb}\n";
	file << "  bind def\n";
	file << "/shd {dup dup currentrgbcolor 4 -2 roll mul 4 -2 roll mul\n";
	file << "  4 -2 roll mul srgb} bind def\n";
	file << "/$F2psBegin {$F2psDict begin /$F2psEnteredState save def} def\n";
	file << "/$F2psEnd {$F2psEnteredState restore end} def\n";
	file << "%%EndProlog\n\n";
	file << "$F2psBegin\n";
	file << "10 setmiterlimit\n";
    file << "n 0 0 m " << l << " 0 l " << l << " " << h << " l 0 " << h <<" l cp clip\n";
	file << scl << "  " << scl << " sc\n";
    file << thickness << " slw\n";
    AssignFont("Arial", TranslateTextHeight( 10 ) );
}

void GraphDevicePS::DeleteContext()
{
//	Write(0,0," ");
	file.close();
	prev_pen   = 0;
	prev_brush = 0;
}

void GraphDevicePS::SetAspect(double px, double py)
{
    aspect = py / px;
    h = l * aspect;
    if ( h > 1000.0 )
    {
        h = 1000.0;
        l = h / aspect;
    }
}

void GraphDevicePS::WriteVerically2D(double x, double y, const char *t)
{
    file << x << " " << y << " m gs 90 rot (" << t << ") " << text_color << " sh gr\n";
//   Rectangle( x, y, x - fabs(font_height), y + GetTextWidth( t )  );
}

void GraphDevicePS::SetFontMetrics()
{
fonts_width[ FN_TIMES ][ 0 ] = 250;
fonts_width[ FN_TIMES ][ 1 ] = 333;
fonts_width[ FN_TIMES ][ 2 ] = 408;
fonts_width[ FN_TIMES ][ 3 ] = 500;
fonts_width[ FN_TIMES ][ 4 ] = 500;
fonts_width[ FN_TIMES ][ 5 ] = 833;
fonts_width[ FN_TIMES ][ 6 ] = 778;
fonts_width[ FN_TIMES ][ 7 ] = 333 ;
fonts_width[ FN_TIMES ][ 8 ] = 333;
fonts_width[ FN_TIMES ][ 9 ] = 333;
fonts_width[ FN_TIMES ][ 10 ] =500;
fonts_width[ FN_TIMES ][ 11 ] =564;
fonts_width[ FN_TIMES ][ 12 ] =250;
fonts_width[ FN_TIMES ][ 13 ] =333;
fonts_width[ FN_TIMES ][ 14 ] =250;
fonts_width[ FN_TIMES ][ 15 ] =278;
fonts_width[ FN_TIMES ][ 16 ] =500;
fonts_width[ FN_TIMES ][ 17 ] =500;
fonts_width[ FN_TIMES ][ 18 ] =500;
fonts_width[ FN_TIMES ][ 19 ] =500;
fonts_width[ FN_TIMES ][ 20 ] =500;
fonts_width[ FN_TIMES ][ 21 ] =500;
fonts_width[ FN_TIMES ][ 22 ] =500;
fonts_width[ FN_TIMES ][ 23 ] =500;
fonts_width[ FN_TIMES ][ 24 ] =500;
fonts_width[ FN_TIMES ][ 25 ] =500;
fonts_width[ FN_TIMES ][ 26 ] =278;
fonts_width[ FN_TIMES ][ 27 ] =278;
fonts_width[ FN_TIMES ][ 28 ] =564;
fonts_width[ FN_TIMES ][ 29 ] =564;
fonts_width[ FN_TIMES ][ 30 ] =564;
fonts_width[ FN_TIMES ][ 31 ] =444;
fonts_width[ FN_TIMES ][ 32 ] =921;
fonts_width[ FN_TIMES ][ 33 ] =722;
fonts_width[ FN_TIMES ][ 34 ] =667 ;
fonts_width[ FN_TIMES ][ 35 ] =667;
fonts_width[ FN_TIMES ][ 36 ] =722;
fonts_width[ FN_TIMES ][ 37 ] =611;
fonts_width[ FN_TIMES ][ 38 ] =556;
fonts_width[ FN_TIMES ][ 39 ] =722;
fonts_width[ FN_TIMES ][ 40 ] =722;
fonts_width[ FN_TIMES ][ 41 ] =333;
fonts_width[ FN_TIMES ][ 42 ] =389;
fonts_width[ FN_TIMES ][ 43 ] =722;
fonts_width[ FN_TIMES ][ 44 ] =611;
fonts_width[ FN_TIMES ][ 45 ] =889;
fonts_width[ FN_TIMES ][ 46 ] =722;
fonts_width[ FN_TIMES ][ 47 ] =722;
fonts_width[ FN_TIMES ][ 48 ] =556;
fonts_width[ FN_TIMES ][ 49 ] =722;
fonts_width[ FN_TIMES ][ 50 ] =667;
fonts_width[ FN_TIMES ][ 51 ] =556;
fonts_width[ FN_TIMES ][ 52 ] =611;
fonts_width[ FN_TIMES ][ 53 ] =722;
fonts_width[ FN_TIMES ][ 54 ] =722;
fonts_width[ FN_TIMES ][ 55 ] =944;
fonts_width[ FN_TIMES ][ 56 ] =722;
fonts_width[ FN_TIMES ][ 57 ] =722;
fonts_width[ FN_TIMES ][ 58 ] =611;
fonts_width[ FN_TIMES ][ 59 ] =333;
fonts_width[ FN_TIMES ][ 60 ] =278;
fonts_width[ FN_TIMES ][ 61 ] =333;
fonts_width[ FN_TIMES ][ 62 ] =469;
fonts_width[ FN_TIMES ][ 63 ] =500;
fonts_width[ FN_TIMES ][ 64 ] =333;
fonts_width[ FN_TIMES ][ 65 ] =444;
fonts_width[ FN_TIMES ][ 66 ] =500;
fonts_width[ FN_TIMES ][ 67 ] =444 ;
fonts_width[ FN_TIMES ][ 68 ] = 500;
fonts_width[ FN_TIMES ][ 69 ] = 444;
fonts_width[ FN_TIMES ][ 70 ] = 333;
fonts_width[ FN_TIMES ][ 71 ] = 500;
fonts_width[ FN_TIMES ][ 72 ] = 500;
fonts_width[ FN_TIMES ][ 73 ] = 278;
fonts_width[ FN_TIMES ][ 74 ] = 278;
fonts_width[ FN_TIMES ][ 75 ] = 500;
fonts_width[ FN_TIMES ][ 76 ] = 278;
fonts_width[ FN_TIMES ][ 77 ] = 778;
fonts_width[ FN_TIMES ][ 78 ] = 500;
fonts_width[ FN_TIMES ][ 79 ] = 500;
fonts_width[ FN_TIMES ][ 80 ] = 500;
fonts_width[ FN_TIMES ][ 81 ] = 500;
fonts_width[ FN_TIMES ][ 82 ] = 333;
fonts_width[ FN_TIMES ][ 83 ] = 389;
fonts_width[ FN_TIMES ][ 84 ] = 278;
fonts_width[ FN_TIMES ][ 85 ] = 500;
fonts_width[ FN_TIMES ][ 86 ] = 500;
fonts_width[ FN_TIMES ][ 87 ] = 722;
fonts_width[ FN_TIMES ][ 88 ] = 500;
fonts_width[ FN_TIMES ][ 89 ] = 500;
fonts_width[ FN_TIMES ][ 90 ] = 444;
fonts_width[ FN_TIMES ][ 91 ] = 480;
fonts_width[ FN_TIMES ][ 92 ] = 200;
fonts_width[ FN_TIMES ][ 93 ] = 480;
fonts_width[ FN_TIMES ][ 94 ] = 541;
fonts_width[ FN_TIMES ][ 95 ] = 0;


fonts_width[ FN_SYMBOL ][ 0 ] = 250 ;
fonts_width[ FN_SYMBOL ][ 1 ] = 333 ;
fonts_width[ FN_SYMBOL ][ 2 ] = 713 ;
fonts_width[ FN_SYMBOL ][ 3 ] = 500 ;
fonts_width[ FN_SYMBOL ][ 4 ] = 549 ;
fonts_width[ FN_SYMBOL ][ 5 ] = 833 ;
fonts_width[ FN_SYMBOL ][ 6 ] = 778 ;
fonts_width[ FN_SYMBOL ][ 7 ] = 439 ;
fonts_width[ FN_SYMBOL ][ 8 ] = 333 ;
fonts_width[ FN_SYMBOL ][ 9 ] = 333 ;
fonts_width[ FN_SYMBOL ][ 10 ] =500 ;
fonts_width[ FN_SYMBOL ][ 11 ] =549 ;
fonts_width[ FN_SYMBOL ][ 12 ] =250 ;
fonts_width[ FN_SYMBOL ][ 13 ] =549 ;
fonts_width[ FN_SYMBOL ][ 14 ] =250 ;
fonts_width[ FN_SYMBOL ][ 15 ] =278 ;
fonts_width[ FN_SYMBOL ][ 16 ] =500 ;
fonts_width[ FN_SYMBOL ][ 17 ] =500 ;
fonts_width[ FN_SYMBOL ][ 18 ] =500 ;
fonts_width[ FN_SYMBOL ][ 19 ] =500 ;
fonts_width[ FN_SYMBOL ][ 20 ] =500 ;
fonts_width[ FN_SYMBOL ][ 21 ] =500 ;
fonts_width[ FN_SYMBOL ][ 22 ] =500 ;
fonts_width[ FN_SYMBOL ][ 23 ] =500 ;
fonts_width[ FN_SYMBOL ][ 24 ] =500 ;
fonts_width[ FN_SYMBOL ][ 25 ] =500 ;
fonts_width[ FN_SYMBOL ][ 26 ] =278 ;
fonts_width[ FN_SYMBOL ][ 27 ] =278 ;
fonts_width[ FN_SYMBOL ][ 28 ] =549 ;
fonts_width[ FN_SYMBOL ][ 29 ] =549 ;
fonts_width[ FN_SYMBOL ][ 30 ] =549 ;
fonts_width[ FN_SYMBOL ][ 31 ] =444 ;
fonts_width[ FN_SYMBOL ][ 32 ] =549 ;
fonts_width[ FN_SYMBOL ][ 33 ] =722 ;
fonts_width[ FN_SYMBOL ][ 34 ] =667 ;
fonts_width[ FN_SYMBOL ][ 35 ] =722 ;
fonts_width[ FN_SYMBOL ][ 36 ] =612 ;
fonts_width[ FN_SYMBOL ][ 37 ] =611 ;
fonts_width[ FN_SYMBOL ][ 38 ] =763 ;
fonts_width[ FN_SYMBOL ][ 39 ] =603 ;
fonts_width[ FN_SYMBOL ][ 40 ] =722 ;
fonts_width[ FN_SYMBOL ][ 41 ] =333 ;
fonts_width[ FN_SYMBOL ][ 42 ] =631 ;
fonts_width[ FN_SYMBOL ][ 43 ] =722 ;
fonts_width[ FN_SYMBOL ][ 44 ] =686 ;
fonts_width[ FN_SYMBOL ][ 45 ] =889 ;
fonts_width[ FN_SYMBOL ][ 46 ] =722 ;
fonts_width[ FN_SYMBOL ][ 47 ] =722 ;
fonts_width[ FN_SYMBOL ][ 48 ] =768 ;
fonts_width[ FN_SYMBOL ][ 49 ] =741 ;
fonts_width[ FN_SYMBOL ][ 50 ] =556 ;
fonts_width[ FN_SYMBOL ][ 51 ] =592 ;
fonts_width[ FN_SYMBOL ][ 52 ] =611 ;
fonts_width[ FN_SYMBOL ][ 53 ] =690 ;
fonts_width[ FN_SYMBOL ][ 54 ] =439 ;
fonts_width[ FN_SYMBOL ][ 55 ] =768 ;
fonts_width[ FN_SYMBOL ][ 56 ] =645 ;
fonts_width[ FN_SYMBOL ][ 57 ] =795 ;
fonts_width[ FN_SYMBOL ][ 58 ] =611 ;
fonts_width[ FN_SYMBOL ][ 59 ] =333 ;
fonts_width[ FN_SYMBOL ][ 60 ] =863 ;
fonts_width[ FN_SYMBOL ][ 61 ] =333 ;
fonts_width[ FN_SYMBOL ][ 62 ] =658 ;
fonts_width[ FN_SYMBOL ][ 63 ] =500 ;
fonts_width[ FN_SYMBOL ][ 64 ] =500 ;
fonts_width[ FN_SYMBOL ][ 65 ] =631 ;
fonts_width[ FN_SYMBOL ][ 66 ] =549 ;
fonts_width[ FN_SYMBOL ][ 67 ] =549 ;
fonts_width[ FN_SYMBOL ][ 68 ] = 494;
fonts_width[ FN_SYMBOL ][ 69 ] = 439;
fonts_width[ FN_SYMBOL ][ 70 ] = 521;
fonts_width[ FN_SYMBOL ][ 71 ] = 411;
fonts_width[ FN_SYMBOL ][ 72 ] = 603;
fonts_width[ FN_SYMBOL ][ 73 ] = 329;
fonts_width[ FN_SYMBOL ][ 74 ] = 603;
fonts_width[ FN_SYMBOL ][ 75 ] = 549;
fonts_width[ FN_SYMBOL ][ 76 ] = 549;
fonts_width[ FN_SYMBOL ][ 77 ] = 576;
fonts_width[ FN_SYMBOL ][ 78 ] = 521;
fonts_width[ FN_SYMBOL ][ 79 ] = 549;
fonts_width[ FN_SYMBOL ][ 80 ] = 549;
fonts_width[ FN_SYMBOL ][ 81 ] = 521;
fonts_width[ FN_SYMBOL ][ 82 ] = 549;
fonts_width[ FN_SYMBOL ][ 83 ] = 603;
fonts_width[ FN_SYMBOL ][ 84 ] = 439;
fonts_width[ FN_SYMBOL ][ 85 ] = 576;
fonts_width[ FN_SYMBOL ][ 86 ] = 713;
fonts_width[ FN_SYMBOL ][ 87 ] = 686;
fonts_width[ FN_SYMBOL ][ 88 ] = 493;
fonts_width[ FN_SYMBOL ][ 89 ] = 686;
fonts_width[ FN_SYMBOL ][ 90 ] = 494;
fonts_width[ FN_SYMBOL ][ 91 ] = 480;
fonts_width[ FN_SYMBOL ][ 92 ] = 200;
fonts_width[ FN_SYMBOL ][ 93 ] = 480;
fonts_width[ FN_SYMBOL ][ 94 ] = 549;
fonts_width[ FN_SYMBOL ][ 95 ] = 750;

fonts_width[ FN_COURIER ][ 0 ] = 600;
fonts_width[ FN_COURIER ][ 1 ] = 600;
fonts_width[ FN_COURIER ][ 2 ] = 600;
fonts_width[ FN_COURIER ][ 3 ] = 600;
fonts_width[ FN_COURIER ][ 4 ] = 600;
fonts_width[ FN_COURIER ][ 5 ] = 600;
fonts_width[ FN_COURIER ][ 6 ] = 600;
fonts_width[ FN_COURIER ][ 7 ] = 600;
fonts_width[ FN_COURIER ][ 8 ] = 600;
fonts_width[ FN_COURIER ][ 9 ] = 600;
fonts_width[ FN_COURIER ][ 10 ] = 600;
fonts_width[ FN_COURIER ][ 11 ] = 600;
fonts_width[ FN_COURIER ][ 12 ] = 600;
fonts_width[ FN_COURIER ][ 13 ] = 600;
fonts_width[ FN_COURIER ][ 14 ] = 600;
fonts_width[ FN_COURIER ][ 15 ] = 600;
fonts_width[ FN_COURIER ][ 16 ] = 600;
fonts_width[ FN_COURIER ][ 17 ] = 600;
fonts_width[ FN_COURIER ][ 18 ] = 600;
fonts_width[ FN_COURIER ][ 19 ] = 600;
fonts_width[ FN_COURIER ][ 20 ] = 600;
fonts_width[ FN_COURIER ][ 21 ] = 600;
fonts_width[ FN_COURIER ][ 22 ] = 600;
fonts_width[ FN_COURIER ][ 23 ] = 600;
fonts_width[ FN_COURIER ][ 24 ] = 600;
fonts_width[ FN_COURIER ][ 25 ] = 600;
fonts_width[ FN_COURIER ][ 26 ] = 600;
fonts_width[ FN_COURIER ][ 27 ] = 600;
fonts_width[ FN_COURIER ][ 28 ] = 600;
fonts_width[ FN_COURIER ][ 29 ] = 600;
fonts_width[ FN_COURIER ][ 30 ] = 600;
fonts_width[ FN_COURIER ][ 31 ] = 600;
fonts_width[ FN_COURIER ][ 32 ] = 600;
fonts_width[ FN_COURIER ][ 33 ] = 600;
fonts_width[ FN_COURIER ][ 34 ] = 600;
fonts_width[ FN_COURIER ][ 35 ] = 600;
fonts_width[ FN_COURIER ][ 36 ] = 600;
fonts_width[ FN_COURIER ][ 37 ] = 600;
fonts_width[ FN_COURIER ][ 38 ] = 600;
fonts_width[ FN_COURIER ][ 39 ] = 600;
fonts_width[ FN_COURIER ][ 40 ] = 600;
fonts_width[ FN_COURIER ][ 41 ] = 600;
fonts_width[ FN_COURIER ][ 42 ] = 600;
fonts_width[ FN_COURIER ][ 43 ] = 600;
fonts_width[ FN_COURIER ][ 44 ] = 600;
fonts_width[ FN_COURIER ][ 45 ] = 600;
fonts_width[ FN_COURIER ][ 46 ] = 600;
fonts_width[ FN_COURIER ][ 47 ] = 600;
fonts_width[ FN_COURIER ][ 48 ] = 600;
fonts_width[ FN_COURIER ][ 49 ] = 600;
fonts_width[ FN_COURIER ][ 50 ] = 600;
fonts_width[ FN_COURIER ][ 51 ] = 600;
fonts_width[ FN_COURIER ][ 52 ] = 600;
fonts_width[ FN_COURIER ][ 53 ] = 600;
fonts_width[ FN_COURIER ][ 54 ] = 600;
fonts_width[ FN_COURIER ][ 55 ] = 600;
fonts_width[ FN_COURIER ][ 56 ] = 600;
fonts_width[ FN_COURIER ][ 57 ] = 600;
fonts_width[ FN_COURIER ][ 58 ] = 600;
fonts_width[ FN_COURIER ][ 59 ] = 600;
fonts_width[ FN_COURIER ][ 60 ] = 600;
fonts_width[ FN_COURIER ][ 61 ] = 600;
fonts_width[ FN_COURIER ][ 62 ] = 600;
fonts_width[ FN_COURIER ][ 63 ] = 600;
fonts_width[ FN_COURIER ][ 64 ] = 600;
fonts_width[ FN_COURIER ][ 65 ] = 600;
fonts_width[ FN_COURIER ][ 66 ] = 600;
fonts_width[ FN_COURIER ][ 67 ] = 600;
fonts_width[ FN_COURIER ][ 68 ] = 600;
fonts_width[ FN_COURIER ][ 69 ] = 600;
fonts_width[ FN_COURIER ][ 70 ] = 600;
fonts_width[ FN_COURIER ][ 71 ] = 600;
fonts_width[ FN_COURIER ][ 72 ] = 600;
fonts_width[ FN_COURIER ][ 73 ] = 600;
fonts_width[ FN_COURIER ][ 74 ] = 600;
fonts_width[ FN_COURIER ][ 75 ] = 600;
fonts_width[ FN_COURIER ][ 76 ] = 600;
fonts_width[ FN_COURIER ][ 77 ] = 600;
fonts_width[ FN_COURIER ][ 78 ] = 600;
fonts_width[ FN_COURIER ][ 79 ] = 600;
fonts_width[ FN_COURIER ][ 80 ] = 600;
fonts_width[ FN_COURIER ][ 81 ] = 600;
fonts_width[ FN_COURIER ][ 82 ] = 600;
fonts_width[ FN_COURIER ][ 83 ] = 600;
fonts_width[ FN_COURIER ][ 84 ] = 600;
fonts_width[ FN_COURIER ][ 85 ] = 600;
fonts_width[ FN_COURIER ][ 86 ] = 600;
fonts_width[ FN_COURIER ][ 87 ] = 600;
fonts_width[ FN_COURIER ][ 88 ] = 600;
fonts_width[ FN_COURIER ][ 89 ] = 600;
fonts_width[ FN_COURIER ][ 90 ] = 600;
fonts_width[ FN_COURIER ][ 91 ] = 600;
fonts_width[ FN_COURIER ][ 92 ] = 600;
fonts_width[ FN_COURIER ][ 93 ] = 600;
fonts_width[ FN_COURIER ][ 94 ] = 600;
fonts_width[ FN_COURIER ][ 95 ] = 600;

fonts_width[ FN_ARIAL ][ 0 ] = 278; 
fonts_width[ FN_ARIAL ][ 1 ] = 278; 
fonts_width[ FN_ARIAL ][ 2 ] = 355; 
fonts_width[ FN_ARIAL ][ 3 ] = 556; 
fonts_width[ FN_ARIAL ][ 4 ] = 556; 
fonts_width[ FN_ARIAL ][ 5 ] = 889; 
fonts_width[ FN_ARIAL ][ 6 ] = 667; 
fonts_width[ FN_ARIAL ][ 7 ] = 221 ;
fonts_width[ FN_ARIAL ][ 8 ] = 333; 
fonts_width[ FN_ARIAL ][ 9 ] = 333; 
fonts_width[ FN_ARIAL ][ 10 ] = 389; 
fonts_width[ FN_ARIAL ][ 11 ] = 584; 
fonts_width[ FN_ARIAL ][ 12 ] = 278; 
fonts_width[ FN_ARIAL ][ 13 ] = 333; 
fonts_width[ FN_ARIAL ][ 14 ] = 278; 
fonts_width[ FN_ARIAL ][ 15 ] = 278; 
fonts_width[ FN_ARIAL ][ 16 ] = 556; 
fonts_width[ FN_ARIAL ][ 17 ] = 556 ;
fonts_width[ FN_ARIAL ][ 18 ] = 556; 
fonts_width[ FN_ARIAL ][ 19 ] = 556; 
fonts_width[ FN_ARIAL ][ 20 ] = 556 ;
fonts_width[ FN_ARIAL ][ 21 ] = 556; 
fonts_width[ FN_ARIAL ][ 22 ] = 556 ;
fonts_width[ FN_ARIAL ][ 23 ] = 556; 
fonts_width[ FN_ARIAL ][ 24 ] = 556; 
fonts_width[ FN_ARIAL ][ 25 ] = 556; 
fonts_width[ FN_ARIAL ][ 26 ] = 278; 
fonts_width[ FN_ARIAL ][ 27 ] = 278; 
fonts_width[ FN_ARIAL ][ 28 ] = 584; 
fonts_width[ FN_ARIAL ][ 29 ] = 584; 
fonts_width[ FN_ARIAL ][ 30 ] = 584; 
fonts_width[ FN_ARIAL ][ 31 ] = 556; 
fonts_width[ FN_ARIAL ][ 32 ] = 1015;
fonts_width[ FN_ARIAL ][ 33 ] = 667; 
fonts_width[ FN_ARIAL ][ 34 ] = 667; 
fonts_width[ FN_ARIAL ][ 35 ] = 722; 
fonts_width[ FN_ARIAL ][ 36 ] = 722; 
fonts_width[ FN_ARIAL ][ 37 ] = 667 ;
fonts_width[ FN_ARIAL ][ 38 ] = 611; 
fonts_width[ FN_ARIAL ][ 39 ] = 778; 
fonts_width[ FN_ARIAL ][ 40 ] = 722; 
fonts_width[ FN_ARIAL ][ 41 ] = 278; 
fonts_width[ FN_ARIAL ][ 42 ] = 500; 
fonts_width[ FN_ARIAL ][ 43 ] = 667; 
fonts_width[ FN_ARIAL ][ 44 ] = 556; 
fonts_width[ FN_ARIAL ][ 45 ] = 833; 
fonts_width[ FN_ARIAL ][ 46 ] = 722; 
fonts_width[ FN_ARIAL ][ 47 ] = 778; 
fonts_width[ FN_ARIAL ][ 48 ] = 667; 
fonts_width[ FN_ARIAL ][ 49 ] = 778; 
fonts_width[ FN_ARIAL ][ 50 ] = 722; 
fonts_width[ FN_ARIAL ][ 51 ] = 667; 
fonts_width[ FN_ARIAL ][ 52 ] = 611; 
fonts_width[ FN_ARIAL ][ 53 ] = 722; 
fonts_width[ FN_ARIAL ][ 54 ] = 667; 
fonts_width[ FN_ARIAL ][ 55 ] = 944; 
fonts_width[ FN_ARIAL ][ 56 ] = 667; 
fonts_width[ FN_ARIAL ][ 57 ] = 667; 
fonts_width[ FN_ARIAL ][ 58 ] = 611; 
fonts_width[ FN_ARIAL ][ 59 ] = 278; 
fonts_width[ FN_ARIAL ][ 60 ] = 278; 
fonts_width[ FN_ARIAL ][ 61 ] = 278; 
fonts_width[ FN_ARIAL ][ 62 ] = 469; 
fonts_width[ FN_ARIAL ][ 63 ] = 556; 
fonts_width[ FN_ARIAL ][ 64 ] = 222; 
fonts_width[ FN_ARIAL ][ 65 ] = 556;
fonts_width[ FN_ARIAL ][ 66 ] = 556; 
fonts_width[ FN_ARIAL ][ 67 ] = 500; 
fonts_width[ FN_ARIAL ][ 68 ] = 556;
fonts_width[ FN_ARIAL ][ 69 ] = 556;
fonts_width[ FN_ARIAL ][ 70 ] = 278;
fonts_width[ FN_ARIAL ][ 71 ] = 556;
fonts_width[ FN_ARIAL ][ 72 ] = 556;
fonts_width[ FN_ARIAL ][ 73 ] = 222;
fonts_width[ FN_ARIAL ][ 74 ] = 222;
fonts_width[ FN_ARIAL ][ 75 ] = 500;
fonts_width[ FN_ARIAL ][ 76 ] = 222;
fonts_width[ FN_ARIAL ][ 77 ] = 833;
fonts_width[ FN_ARIAL ][ 78 ] = 556;
fonts_width[ FN_ARIAL ][ 79 ] = 556;
fonts_width[ FN_ARIAL ][ 80 ] = 556;
fonts_width[ FN_ARIAL ][ 81 ] = 556;
fonts_width[ FN_ARIAL ][ 82 ] = 333;
fonts_width[ FN_ARIAL ][ 83 ] = 500;
fonts_width[ FN_ARIAL ][ 84 ] = 278;
fonts_width[ FN_ARIAL ][ 85 ] = 556;
fonts_width[ FN_ARIAL ][ 86 ] = 500;
fonts_width[ FN_ARIAL ][ 87 ] = 722;
fonts_width[ FN_ARIAL ][ 88 ] = 500;
fonts_width[ FN_ARIAL ][ 89 ] = 500;
fonts_width[ FN_ARIAL ][ 90 ] = 500;
fonts_width[ FN_ARIAL ][ 91 ] = 334;
fonts_width[ FN_ARIAL ][ 92 ] = 260;
fonts_width[ FN_ARIAL ][ 93 ] = 334;
fonts_width[ FN_ARIAL ][ 94 ] = 584;


}

double GraphDevicePS::GetTextWidth(const char *tx)
{
    double tw=0.0,ln = strlen(tx);
    for(unsigned int k=0; k<ln; k++)
        if ( ( 128 - tx[k] ) * ( tx[k] - 31 ) > 0 ) 
            tw += fonts_width[ font ][ tx[ k ] - 32 ] / 1000.0 *  font_height ;
    return fabs( tw / font_height_factor );
}


double GraphDevicePS::GetFontHeight()
{
    return fabs( font_height );
}

/*
double GraphDevicePS::GetNominalFontHeight()
{
    return font_height / font_height_factor ;
}
*/

void GraphDevicePS::SetFontHeight(double ws, bool absolute )
{
          font_height_factor = 0.729;  
          font_height = absolute ? ws : TranslateTextHeight( ws );      // * 0.729;
		  file << "/" << fontname << " ff " << fabs( ws / font_height_factor ) << " scf sf\n";
}

