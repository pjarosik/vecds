
#include "Graphics.h"

/*
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
*/

DColor CGraphics::colBLACK(	DColor(0,0,0) );
DColor CGraphics::colBLUE( DColor(0,0,127));
DColor CGraphics::colGREEN( DColor(0,127,0));
DColor CGraphics::colCYAN( DColor(0,127,127));
DColor CGraphics::colRED( DColor(127,0,0));
DColor CGraphics::colMAGENTA( DColor(127,0,127));
DColor CGraphics::colBROWN( DColor(127,127,0));
DColor CGraphics::colLIGHTGRAY( DColor(127,127,127));
DColor CGraphics::colDARKGRAY( DColor(200,200,200));
DColor CGraphics::colLIGHTBLUE( DColor(0,0,255));
DColor CGraphics::colLIGHTGREEN( DColor(0,255,0));
DColor CGraphics::colLIGHTCYAN( DColor(0,255,255));
DColor CGraphics::colLIGHTRED( DColor(255,0,0));
DColor CGraphics::colLIGHTMAGENTA( DColor(255,0,255));
DColor CGraphics::colYELLOW( DColor(255,255,0));
DColor CGraphics::colWHITE(	DColor(255,255,255));

DColor CGraphics::colors[ NUMBER_OF_COLORS ] = {
						CGraphics::colBLACK,      
						CGraphics::colBLUE,
						CGraphics::colGREEN,
						CGraphics::colCYAN,
						CGraphics::colRED,
						CGraphics::colMAGENTA,
						CGraphics::colBROWN,
						CGraphics::colLIGHTGRAY,
						CGraphics::colDARKGRAY, 
						CGraphics::colLIGHTBLUE,
						CGraphics::colLIGHTGREEN,
						CGraphics::colLIGHTCYAN,
						CGraphics::colLIGHTRED,
						CGraphics::colLIGHTMAGENTA,
						CGraphics::colYELLOW,
						CGraphics::colWHITE,
					};

DPen CGraphics::penPEN_BLACK( CGraphics::colBLACK, 0, 1.0 );
DPen CGraphics::penPEN_BLUE( CGraphics::colBLUE, 0, 1.0 );
DPen CGraphics::penPEN_GREEN( CGraphics::colGREEN, 0, 1.0 );
DPen CGraphics::penPEN_CYAN( CGraphics::colCYAN, 0, 1.0 );
DPen CGraphics::penPEN_RED( CGraphics::colRED, 0, 1.0 );
DPen CGraphics::penPEN_MAGENTA( CGraphics::colMAGENTA, 0, 1.0 );
DPen CGraphics::penPEN_BROWN( CGraphics::colBROWN, 0, 1.0 );
DPen CGraphics::penPEN_LIGHTGRAY( CGraphics::colLIGHTGRAY, 0, 1.0 );
DPen CGraphics::penPEN_DARKGRAY( CGraphics::colDARKGRAY, 0, 1.0 );      
DPen CGraphics::penPEN_LIGHTBLUE( CGraphics::colLIGHTBLUE, 0, 1.0 );
DPen CGraphics::penPEN_LIGHTGREEN( CGraphics::colLIGHTGREEN, 0, 1.0 );
DPen CGraphics::penPEN_LIGHTCYAN( CGraphics::colLIGHTCYAN, 0, 1.0 );
DPen CGraphics::penPEN_LIGHTRED( CGraphics::colLIGHTRED, 0, 1.0 );
DPen CGraphics::penPEN_LIGHTMAGENTA( CGraphics::colLIGHTMAGENTA, 0, 1.0 );
DPen CGraphics::penPEN_YELLOW( CGraphics::colYELLOW, 0, 1.0 );
DPen CGraphics::penPEN_WHITE( CGraphics::colWHITE, 0, 1.0 );

DPen CGraphics::penPEN_THICK_BLACK( CGraphics::colBLACK, 0, 2.0 );
DPen CGraphics::penPEN_THICK_BLUE( CGraphics::colBLUE, 0, 2.0 );
DPen CGraphics::penPEN_THICK_GREEN( CGraphics::colGREEN, 0, 2.0 );
DPen CGraphics::penPEN_THICK_CYAN( CGraphics::colCYAN, 0, 2.0 );
DPen CGraphics::penPEN_THICK_RED( CGraphics::colRED, 0, 2.0 );
DPen CGraphics::penPEN_THICK_MAGENTA( CGraphics::colMAGENTA, 0, 2.0 );
DPen CGraphics::penPEN_THICK_BROWN( CGraphics::colBROWN, 0, 2.0 );
DPen CGraphics::penPEN_THICK_LIGHTGRAY( CGraphics::colLIGHTGRAY, 0, 2.0 );
DPen CGraphics::penPEN_THICK_DARKGRAY( CGraphics::colDARKGRAY, 0, 2.0 );         
DPen CGraphics::penPEN_THICK_LIGHTBLUE( CGraphics::colLIGHTBLUE, 0, 2.0 );
DPen CGraphics::penPEN_THICK_LIGHTGREEN( CGraphics::colLIGHTGREEN, 0, 2.0 );
DPen CGraphics::penPEN_THICK_LIGHTCYAN( CGraphics::colLIGHTCYAN, 0, 2.0 );
DPen CGraphics::penPEN_THICK_LIGHTRED( CGraphics::colLIGHTRED, 0, 2.0 );
DPen CGraphics::penPEN_THICK_LIGHTMAGENTA( CGraphics::colLIGHTMAGENTA, 0, 2.0 );
DPen CGraphics::penPEN_THICK_YELLOW( CGraphics::colYELLOW, 0, 2.0 );
DPen CGraphics::penPEN_THICK_WHITE( CGraphics::colWHITE, 0, 2.0 );


DPen CGraphics::pens[ NUMBER_OF_PENS ] = {
	  penPEN_BLACK,       
      penPEN_BLUE,
      penPEN_GREEN,
      penPEN_CYAN,
      penPEN_RED,
      penPEN_MAGENTA,
      penPEN_BROWN,
      penPEN_LIGHTGRAY,
      penPEN_DARKGRAY,    
      penPEN_LIGHTBLUE,
      penPEN_LIGHTGREEN,
      penPEN_LIGHTCYAN,
      penPEN_LIGHTRED,
      penPEN_LIGHTMAGENTA,
      penPEN_YELLOW,
      penPEN_WHITE,
      penPEN_THICK_BLACK, 
      penPEN_THICK_BLUE,
      penPEN_THICK_GREEN,
      penPEN_THICK_CYAN,
      penPEN_THICK_RED,
      penPEN_THICK_MAGENTA,
      penPEN_THICK_BROWN,
      penPEN_THICK_LIGHTGRAY,
      penPEN_THICK_DARKGRAY,         
      penPEN_THICK_LIGHTBLUE,
      penPEN_THICK_LIGHTGREEN,
      penPEN_THICK_LIGHTCYAN,
      penPEN_THICK_LIGHTRED,
      penPEN_THICK_LIGHTMAGENTA,
      penPEN_THICK_YELLOW,
      penPEN_THICK_WHITE
};

	DBrush CGraphics::brBRUSH_SOLID_BLACK( CGraphics::colBLACK, 0 );     
    DBrush CGraphics::brBRUSH_SOLID_BLUE( CGraphics::colBLUE, 0 );
    DBrush CGraphics::brBRUSH_SOLID_GREEN( CGraphics::colGREEN, 0 );
    DBrush CGraphics::brBRUSH_SOLID_CYAN( CGraphics::colCYAN, 0 );
    DBrush CGraphics::brBRUSH_SOLID_RED( CGraphics::colRED, 0 );
    DBrush CGraphics::brBRUSH_SOLID_MAGENTA( CGraphics::colMAGENTA, 0 );
    DBrush CGraphics::brBRUSH_SOLID_BROWN( CGraphics::colBROWN, 0 );
    DBrush CGraphics::brBRUSH_SOLID_LIGHTGRAY( CGraphics::colLIGHTGRAY, 0 );
    DBrush CGraphics::brBRUSH_SOLID_DARKGRAY( CGraphics::colDARKGRAY, 0 ); 
    DBrush CGraphics::brBRUSH_SOLID_LIGHTBLUE( CGraphics::colLIGHTBLUE, 0 );
    DBrush CGraphics::brBRUSH_SOLID_LIGHTGREEN( CGraphics::colLIGHTGREEN, 0 );
    DBrush CGraphics::brBRUSH_SOLID_LIGHTCYAN( CGraphics::colLIGHTCYAN, 0 );
    DBrush CGraphics::brBRUSH_SOLID_LIGHTRED( CGraphics::colLIGHTRED, 0 );
    DBrush CGraphics::brBRUSH_SOLID_LIGHTMAGENTA( CGraphics::colBLACK, 0 );
    DBrush CGraphics::brBRUSH_SOLID_YELLOW( CGraphics::colYELLOW, 0 );
    DBrush CGraphics::brBRUSH_SOLID_WHITE( CGraphics::colWHITE, 0 );
    DBrush CGraphics::brBRUSH_HOLLOW( CGraphics::colBLACK, 0 );

	DBrush CGraphics::brushes[ NUMBER_OF_BRUSHES ] = {
		brBRUSH_SOLID_BLACK,     
		brBRUSH_SOLID_BLUE,
		brBRUSH_SOLID_GREEN,
		brBRUSH_SOLID_CYAN,
		brBRUSH_SOLID_RED,
		brBRUSH_SOLID_MAGENTA,
		brBRUSH_SOLID_BROWN,
		brBRUSH_SOLID_LIGHTGRAY,
		brBRUSH_SOLID_DARKGRAY,  
		brBRUSH_SOLID_LIGHTBLUE,
		brBRUSH_SOLID_LIGHTGREEN,
		brBRUSH_SOLID_LIGHTCYAN,
		brBRUSH_SOLID_LIGHTRED,
		brBRUSH_SOLID_LIGHTMAGENTA,
		brBRUSH_SOLID_YELLOW,
		brBRUSH_SOLID_WHITE,
		brBRUSH_HOLLOW,
	};


CGraphics::CGraphics(void)
{
}

CGraphics::~CGraphics(void)
{
}
