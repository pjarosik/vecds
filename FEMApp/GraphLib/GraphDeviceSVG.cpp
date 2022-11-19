// GDC.H definition of graphic device class

#include "GraphDeviceSVG.h"
#include <cmath>
#include <cassert>

GraphDeviceSVG::GraphDeviceSVG(const char* fl, const CPaintFrame *f, bool eh  ):CGraphDevice( f ),	filename( fl ), text_color("fill:black"), pen("stroke:black;stroke-width:1"),
                                                fontname("Arial"), brush("fill:none"),
                                                aspect(1.0), thickness(1.0), creator(0), xmlout(0), embend_in_html( eh )
{
    SetFontMetrics( );
    l = 1000;  h = 1000;
    font_height_factor = 1.4;
}

GraphDeviceSVG::~GraphDeviceSVG( ) { }

void GraphDeviceSVG::SetBrush( const DBrush &br )
{

#ifdef _DEBUG
    assert( br.GetColor().GetR() <= 1.0 );
    assert( br.GetColor().GetG() <= 1.0 );
    assert( br.GetColor().GetB() <= 1.0 );

    assert( br.GetColor().GetR() >= 0.0 );
    assert( br.GetColor().GetG() >= 0.0 );
    assert( br.GetColor().GetB() >= 0.0 );
#endif

    brush = "fill:rgb(" + inttostring( br.GetColor().GetR() * 255.0 ) + ","
                        + inttostring( br.GetColor().GetG() * 255.0 ) + ","
                        + inttostring( br.GetColor().GetB() * 255.0 ) + ")";
/*
    brush  = doubletostring53( double( br.GetColor().GetR() )  ) + ' ';
    brush += doubletostring53( double( br.GetColor().GetG() )  ) + ' ';
    brush += doubletostring53( double( br.GetColor().GetB() )  ) + " srgb\n";
*/
}

void GraphDeviceSVG::CreatePenArray( const vector<DPen> &arr  )
{
	unsigned k;
	string p;
	for ( k=0; k<arr.size(); k++)
	{

        p = "stroke:rgb("   + inttostring( arr[k].GetColor().GetR() * 255.0 ) + ","
                            + inttostring( arr[k].GetColor().GetG() * 255.0 ) + ","
                            + inttostring( arr[k].GetColor().GetB() * 255.0 ) + ");"
                            + "stroke-width:" + doubletostring53( arr[ k ].GetThickness() );

		penarray.push_back( p );
	}
}

void GraphDeviceSVG::CreateBrushArray( const vector<DBrush> &arr )
{

	string b;
	unsigned k;
	for ( k=0; k<arr.size(); k++)
	{
        "fill:rgb("  + inttostring( arr[k].GetColor().GetR() * 255.0 ) + ","
                     + inttostring( arr[k].GetColor().GetG() * 255.0 ) + ","
                     + inttostring( arr[k].GetColor().GetB() * 255.0 ) + ")";
        brusharray.push_back( b );
	}

}


void GraphDeviceSVG::SelectArrayBrush( unsigned int col )
{
	#ifdef _DEBUG
		assert( col < brusharray.size() );
		assert( col > -1 );
	#endif
	brush = brusharray[ col ];
}

void GraphDeviceSVG::SelectArrayPen( unsigned int col )
{
	#ifdef _DEBUG
		assert( col < penarray.size() );
	#endif
	pen   = penarray[ col ];
}

void GraphDeviceSVG::SetPredefinedBrush( unsigned int col )
{
	if ( prev_brush == col ) return;

	switch( col )
	{
                        case BRUSH_SOLID_BLACK        : brush = "fill:black";        break;
                        case BRUSH_SOLID_BLUE         : brush = "fill:blue";         break;
                        case BRUSH_SOLID_GREEN        : brush = "fill:green";        break;
                        case BRUSH_SOLID_CYAN         : brush = "fill:cyan";         break;
                        case BRUSH_SOLID_RED          : brush = "fill:red";          break;
                        case BRUSH_SOLID_MAGENTA      : brush = "fill:magenta";      break;
                        case BRUSH_SOLID_BROWN        : brush = "fill:darkyellow";   break;
                        case BRUSH_SOLID_LIGHTGRAY    : brush = "fill:darkgray";     break;
                        case BRUSH_SOLID_DARKGRAY     : brush = "fill:darkgray";     break;
                        case BRUSH_SOLID_LIGHTBLUE    : brush = "fill:lightblue";    break;
                        case BRUSH_SOLID_LIGHTGREEN   : brush = "fill:lightgreen";   break;
                        case BRUSH_SOLID_LIGHTCYAN    : brush = "fill:lightcyan";    break;
                        case BRUSH_SOLID_LIGHTRED     : brush = "fill:lightred";     break;
                        case BRUSH_SOLID_LIGHTMAGENTA : brush = "fill:lightmagenta"; break;
                        case BRUSH_SOLID_YELLOW       : brush = "fill:yellow";  break;
                        case BRUSH_SOLID_WHITE        : brush = "fill:white";        break;
                        case BRUSH_HOLLOW			  : brush = "fill:none";         break;

	}
	prev_brush = col;
}

void GraphDeviceSVG::SetHollowBrush( )
{
    brush = "fill:none";
}

void GraphDeviceSVG::SetPen( const DPen &pn )
{
    pen = "stroke:rgb("   + inttostring( pn.GetColor().GetR() * 255.0 ) + ","
                          + inttostring( pn.GetColor().GetG() * 255.0 ) + ","
                          + inttostring( pn.GetColor().GetB() * 255.0 ) + ");"
                          + "stroke-width:" + doubletostring53( pn.GetThickness() );

}

void GraphDeviceSVG::SetNullPen()
{
    pen = "stroke:none";
}

void GraphDeviceSVG::SetPredefinedPen( unsigned int col )
{
	if ( prev_pen == col ) return;

	switch( col )
	{
                        case PEN_BLACK        : pen = "stroke:black;stroke-width:1";        break;
                        case PEN_BLUE         : pen = "stroke:blue;stroke-width:1";         break;
                        case PEN_GREEN        : pen = "stroke:green;stroke-width:1";        break;
                        case PEN_CYAN         : pen = "stroke:cyan;stroke-width:1";         break;
                        case PEN_RED          : pen = "stroke:red;stroke-width:1";          break;
                        case PEN_MAGENTA      : pen = "stroke:magenta;stroke-width:1";      break;
                        case PEN_BROWN        : pen = "stroke:yellow;stroke-width:1";       break;
                        case PEN_LIGHTGRAY    : pen = "stroke:darkgray;stroke-width:1";     break;
                        case PEN_DARKGRAY     : pen = "stroke:darkgray;stroke-width:1";     break;
                        case PEN_LIGHTBLUE    : pen = "stroke:lightblue;stroke-width:1";    break;
                        case PEN_LIGHTGREEN   : pen = "stroke:lightgreen;stroke-width:1";   break;
                        case PEN_LIGHTCYAN    : pen = "stroke:lightcyan;stroke-width:1";    break;
                        case PEN_LIGHTRED     : pen = "stroke:lightred;stroke-width:1";     break;
                        case PEN_LIGHTMAGENTA : pen = "stroke:lightmagenta;stroke-width:1"; break;
                        case PEN_YELLOW       : pen = "stroke:lightyellow;stroke-width:1";  break;
                        case PEN_WHITE        : pen = "stroke:white;stroke-width:1";        break;

                        case PEN_THICK_BLACK        : pen = "stroke:black;stroke-width:2";       break;
                        case PEN_THICK_BLUE         : pen = "stroke:bluestroke-width:2";         break;
                        case PEN_THICK_GREEN        : pen = "stroke:greenstroke-width:2";        break;
                        case PEN_THICK_CYAN         : pen = "stroke:cyanstroke-width:2";         break;
                        case PEN_THICK_RED          : pen = "stroke:redstroke-width:2";          break;
                        case PEN_THICK_MAGENTA      : pen = "stroke:magentastroke-width:2";      break;
                        case PEN_THICK_BROWN        : pen = "stroke:yellowstroke-width:2";       break;
                        case PEN_THICK_LIGHTGRAY    : pen = "stroke:darkgraystroke-width:2";     break;
                        case PEN_THICK_DARKGRAY     : pen = "stroke:darkgraystroke-width:2";     break;
                        case PEN_THICK_LIGHTBLUE    : pen = "stroke:lightbluestroke-width:2";    break;
                        case PEN_THICK_LIGHTGREEN   : pen = "stroke:lightgreenstroke-width:2";   break;
                        case PEN_THICK_LIGHTCYAN    : pen = "stroke:lightcyanstroke-width:2";    break;
                        case PEN_THICK_LIGHTRED     : pen = "stroke:lightredstroke-width:2";     break;
                        case PEN_THICK_LIGHTMAGENTA : pen = "stroke:lightmagentastroke-width:2"; break;
                        case PEN_THICK_YELLOW       : pen = "stroke:lightyellowstroke-width:2";  break;
                        case PEN_THICK_WHITE        : pen = "stroke:whitestroke-width:2";        break;
	}
	prev_pen = col;
}


void GraphDeviceSVG::AssignFont( const char *nm, double ws )
{
          font_height = ws * font_height_factor;      // * 0.729;
		  file << "/" << nm << " ff " << fabs( ws / font_height_factor ) << " scf sf\n";
          if ( !strcmp( nm, "Arial"   ) )       font = FN_ARIAL;
          if ( !strcmp( nm, "Helvetica"   ) )   font = FN_ARIAL;
          if ( !strcmp( nm, "Courier" ) )       font = FN_COURIER;
          if ( !strcmp( nm, "Times"   ) )       font = FN_TIMES;
          if ( !strcmp( nm, "Symbol"  ) )       font = FN_SYMBOL;
          fontname = nm;
}

void GraphDeviceSVG::Rectangle( double x1, double y1, double x2, double y2 )
{
// <rect width="300" height="100" style="fill:rgb(0,0,255);stroke-width:3;stroke:rgb(0,0,0)" />
    double hr = y2-y1;
    creator->OpenNode("rect");
    creator->AddParameter("x",x1);
    creator->AddParameter("y",h-y1-hr);
    creator->AddParameter("width",x2-x1);
    creator->AddParameter("height",y2-y1);
    creator->AddParameter("style",brush+";"+pen);
    creator->CloseNode();

//    if ( brush != "null" ) file << " n " << x1 << " " << y1 << " m " << x2 << " " << y1 << " l " << x2 << " " << y2 << " l " << x1 << " " << y2 << " l cp gs " << brush << " ef\n";
//    if ( pen != "null" )   file << " n " << x1 << " " << y1 << " m " << x2 << " " << y1 << " l " << x2 << " " << y2 << " l " << x1 << " " << y2 << " l cp gs " << pen   << " s gr\n";
}

void GraphDeviceSVG::Line2D( double a, double b, double c, double d )
{
    creator->OpenNode("line");
    creator->AddParameter("x1",a);
    creator->AddParameter("y1",h-b);
    creator->AddParameter("x2",c);
    creator->AddParameter("y2",h-d);
    creator->AddParameter("style",brush+";"+pen);
    creator->CloseNode();

}

void GraphDeviceSVG::Polygon2D( vector<DPoint2D> &pts )
{
    unsigned int k, n = (unsigned)pts.size();
//<polygon points="200,10 250,190 160,210" style="fill:lime;stroke:purple;stroke-width:1" />

    stringstream spts;

    for (k=0; k<pts.size(); k++)
        spts << ( k ? " " : "" ) << pts[k].x << "," << h - pts[k].y;
    creator->OpenNode("polygon");
    creator->AddParameter("points",spts.str());
    creator->AddParameter("style",brush+";"+pen);
    creator->CloseNode();

}

void GraphDeviceSVG::Triangle2D( double x1, double y1, double x2, double y2, double x3, double y3 )
{
    stringstream pts;

    pts << x1 << "," << h - y1 << " " << x2 << "," << h - y2 << " " << x3 << "," << h - y3;
    creator->OpenNode("polygon");
    creator->AddParameter("points",pts.str());
    creator->AddParameter("style",brush+";"+pen);
    creator->CloseNode();
}

void GraphDeviceSVG::Polyline2D( vector<DPoint2D> &pts )
{
    unsigned int k, n = (unsigned)pts.size();
    stringstream spts;

    for (k=0; k<pts.size(); k++)
        spts << ( k ? " " : "" ) << pts[k].x << "," << h - pts[k].y;
    creator->OpenNode("polygon");
    creator->AddParameter("points",spts.str());
    creator->AddParameter("style","fill:none;"+pen);
    creator->CloseNode();
}

void GraphDeviceSVG::Ellipse(double x1,double y1,double x2,double y2)
{
// <ellipse cx="200" cy="80" rx="100" ry="50" style="fill:yellow;stroke:purple;stroke-width:2" />

    creator->OpenNode("ellipse");
    creator->AddParameter("cx",0.5*(x2+x1));
    creator->AddParameter("cy",h-0.5*(y2+y1));
    creator->AddParameter("rx",0.5*fabs(x2-x1));
    creator->AddParameter("ry",0.5*fabs(y2-y1));
    creator->AddParameter("style",brush+";"+pen);
    creator->CloseNode();

}

void GraphDeviceSVG::Write2D(double x,double y,const char *t)
{
//      <text x = "10" y = "25" font-size = "20" font-family = "Arial" style="fill:red">Dupa zimna</text>
  //  Rectangle( x, y, x + GetTextWidth( t ) , y + font_height / font_height_factor  );
    creator->OpenNode("text");
    creator->AddParameter("x",x);
    creator->AddParameter("y",h-y);
    creator->AddParameter("font-family",fontname);
    creator->AddParameter("font-size",font_height);
    creator->AddParameter("style",text_color);
    creator->AddText(t);
    creator->CloseNode();
}

void GraphDeviceSVG::WriteInBox2D(double x1,double y1,double x2,double y2,const string &str, TextJustify tj )
{
    Rectangle( x1, y1, x2,  y2 );
    double tfh = y2 - y1;
    SetFontHeight( tfh , false );
    double twd=GetTextWidth( str.c_str() );

    creator->OpenNode("text");
    creator->AddParameter("font-family",fontname);
    creator->AddParameter("font-size",font_height);
    creator->AddParameter("style",text_color);

    switch( tj )
    {
        case TJ_LC : creator->AddParameter("x",x1);             creator->AddParameter("y",h-y1);  break;
        case TJ_CC : creator->AddParameter("x", (x1 + x2 - twd) * 0.5); creator->AddParameter("y",h-y1);  break;
        case TJ_RC : creator->AddParameter("x",x2 - twd);       creator->AddParameter("y",h-y1);  break;
    }

    creator->AddText(str);
    creator->CloseNode();
}

double  GraphDeviceSVG::TranslateTextHeight( double w )
{
    return w / font_height_factor;
}

double  GraphDeviceSVG::GetHeight()
{
	return  h;
}

double  GraphDeviceSVG::GetWidth()
{
	return  l;
}

void GraphDeviceSVG::MakeContext()
{
	double scl = 0.5;
    xmlout  = new CXML_OutputDevice( embend_in_html ?  filename+".html" : filename );
    creator = new CXML_Creator( *xmlout );

    if ( embend_in_html )
    {
        creator->OpenNode("html");
        creator->OpenNode("body");
    }

    creator->OpenNode("svg");
    creator->AddParameter("width",l);
    creator->AddParameter("height",h);
    AssignFont("Arial", TranslateTextHeight( 10 ) );

}

void GraphDeviceSVG::DeleteContext()
{
//	Write(0,0," ");
    creator->CloseNode();

    if ( embend_in_html )
    {
        creator->CloseNode();
        creator->CloseNode();
    }

    delete creator; creator = 0;
    delete xmlout; xmlout   = 0;
	file.close();
	prev_pen   = 0;
	prev_brush = 0;
}

void GraphDeviceSVG::SetAspect(double px, double py)
{
    aspect = py / px;
    h = l * aspect;
    if ( h > 1000.0 )
    {
        h = 1000.0;
        l = h / aspect;
    }
}

void GraphDeviceSVG::WriteVerically2D(double x, double y, const char *t)
{

    stringstream ss;
    ss << "(" << -90 << " " << x << "," << h-y << ")";
    creator->OpenNode("text");
    creator->AddParameter("x",x);
    creator->AddParameter("y",h-y);
    creator->AddParameter("font-family",fontname);
    creator->AddParameter("font-size",font_height);
    creator->AddParameter("style",text_color);
    creator->AddParameter("transform",ss.str() );
    creator->AddText(t);
    creator->CloseNode();


//   Rectangle( x, y, x - fabs(font_height), y + GetTextWidth( t )  );
}

void GraphDeviceSVG::SetFontMetrics()
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

double GraphDeviceSVG::GetTextWidth(const char *tx)
{
    double tw=0.0,ln = strlen(tx);
    for(unsigned int k=0; k<ln; k++)
        if ( ( 128 - tx[k] ) * ( tx[k] - 31 ) > 0 ) 
            tw += fonts_width[ font ][ tx[ k ] - 32 ] / 1000.0 *  font_height ;
    return fabs( tw );
}


double GraphDeviceSVG::GetFontHeight()
{
    return fabs( font_height / font_height_factor );
}


void GraphDeviceSVG::SetFontHeight(double ws, bool absolute )
{
          font_height = absolute ? ws :  ws * font_height_factor;      // * 0.729;
}

