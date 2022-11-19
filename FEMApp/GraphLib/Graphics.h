#pragma once

#include "color.h"
#include "pen.h"
#include "brush.h"
#include "graph_constants.h"

class CGraphics
{
public:
	CGraphics(void);
	~CGraphics(void);

	static DColor colBLACK;
	static DColor colBLUE;
    static DColor colGREEN;
    static DColor colCYAN;
    static DColor colRED;
    static DColor colMAGENTA;
    static DColor colBROWN;
    static DColor colLIGHTGRAY;
    static DColor colDARKGRAY;
    static DColor colLIGHTBLUE;
    static DColor colLIGHTGREEN;
    static DColor colLIGHTCYAN;
    static DColor colLIGHTRED;
    static DColor colLIGHTMAGENTA;
    static DColor colYELLOW;
    static DColor colWHITE;
	static DColor colors[ NUMBER_OF_COLORS ];

	static DPen penPEN_BLACK;          /* dark colors */
    static DPen penPEN_BLUE;
    static DPen penPEN_GREEN;
    static DPen penPEN_CYAN;
    static DPen penPEN_RED;
    static DPen penPEN_MAGENTA;
    static DPen penPEN_BROWN;
    static DPen penPEN_LIGHTGRAY;
    static DPen penPEN_DARKGRAY;           /* light colors */
    static DPen penPEN_LIGHTBLUE;
    static DPen penPEN_LIGHTGREEN;
    static DPen penPEN_LIGHTCYAN;
    static DPen penPEN_LIGHTRED;
    static DPen penPEN_LIGHTMAGENTA;
    static DPen penPEN_YELLOW;
    static DPen penPEN_WHITE;
    static DPen penPEN_THICK_BLACK;          /* dark colors */
    static DPen penPEN_THICK_BLUE;
    static DPen penPEN_THICK_GREEN;
    static DPen penPEN_THICK_CYAN;
    static DPen penPEN_THICK_RED;
    static DPen penPEN_THICK_MAGENTA;
    static DPen penPEN_THICK_BROWN;
    static DPen penPEN_THICK_LIGHTGRAY;
    static DPen penPEN_THICK_DARKGRAY;           /* light colors */
    static DPen penPEN_THICK_LIGHTBLUE;
    static DPen penPEN_THICK_LIGHTGREEN;
    static DPen penPEN_THICK_LIGHTCYAN;
    static DPen penPEN_THICK_LIGHTRED;
    static DPen penPEN_THICK_LIGHTMAGENTA;
    static DPen penPEN_THICK_YELLOW;
    static DPen penPEN_THICK_WHITE;
	static DPen pens[ NUMBER_OF_PENS ];

	static DBrush brBRUSH_SOLID_BLACK;     
    static DBrush brBRUSH_SOLID_BLUE;
    static DBrush brBRUSH_SOLID_GREEN;
    static DBrush brBRUSH_SOLID_CYAN;
    static DBrush brBRUSH_SOLID_RED;
    static DBrush brBRUSH_SOLID_MAGENTA;
    static DBrush brBRUSH_SOLID_BROWN;
    static DBrush brBRUSH_SOLID_LIGHTGRAY;
    static DBrush brBRUSH_SOLID_DARKGRAY;  
    static DBrush brBRUSH_SOLID_LIGHTBLUE;
    static DBrush brBRUSH_SOLID_LIGHTGREEN;
    static DBrush brBRUSH_SOLID_LIGHTCYAN;
    static DBrush brBRUSH_SOLID_LIGHTRED;
    static DBrush brBRUSH_SOLID_LIGHTMAGENTA;
    static DBrush brBRUSH_SOLID_YELLOW;
    static DBrush brBRUSH_SOLID_WHITE;
    static DBrush brBRUSH_HOLLOW;
	static DBrush brushes[ NUMBER_OF_BRUSHES ];
};

