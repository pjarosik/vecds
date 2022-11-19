// COLORS.H - definicja dosowych sta³ych

#ifndef __COLORS_H
#define __COLORS_H

enum PREDEFINED_COLORS {
    BLACK,          /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,           /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE,
	NUMBER_OF_COLORS
};

enum PREDEFINED_PENS
{
	PEN_BLACK,          /* dark colors */
    PEN_BLUE,
    PEN_GREEN,
    PEN_CYAN,
    PEN_RED,
    PEN_MAGENTA,
    PEN_BROWN,
    PEN_LIGHTGRAY,
    PEN_DARKGRAY,           /* light colors */
    PEN_LIGHTBLUE,
    PEN_LIGHTGREEN,
    PEN_LIGHTCYAN,
    PEN_LIGHTRED,
    PEN_LIGHTMAGENTA,
    PEN_YELLOW,
    PEN_WHITE,
    PEN_THICK_BLACK,          /* dark colors */
    PEN_THICK_BLUE,
    PEN_THICK_GREEN,
    PEN_THICK_CYAN,
    PEN_THICK_RED,
    PEN_THICK_MAGENTA,
    PEN_THICK_BROWN,
    PEN_THICK_LIGHTGRAY,
    PEN_THICK_DARKGRAY,           /* light colors */
    PEN_THICK_LIGHTBLUE,
    PEN_THICK_LIGHTGREEN,
    PEN_THICK_LIGHTCYAN,
    PEN_THICK_LIGHTRED,
    PEN_THICK_LIGHTMAGENTA,
    PEN_THICK_YELLOW,
    PEN_THICK_WHITE,
	NUMBER_OF_PENS
};


enum BRUSES
{
	BRUSH_SOLID_BLACK,          /* dark colors */
    BRUSH_SOLID_BLUE,
    BRUSH_SOLID_GREEN,
    BRUSH_SOLID_CYAN,
    BRUSH_SOLID_RED,
    BRUSH_SOLID_MAGENTA,
    BRUSH_SOLID_BROWN,
    BRUSH_SOLID_LIGHTGRAY,
    BRUSH_SOLID_DARKGRAY,           /* light colors */
    BRUSH_SOLID_LIGHTBLUE,
    BRUSH_SOLID_LIGHTGREEN,
    BRUSH_SOLID_LIGHTCYAN,
    BRUSH_SOLID_LIGHTRED,
    BRUSH_SOLID_LIGHTMAGENTA,
    BRUSH_SOLID_YELLOW,
    BRUSH_SOLID_WHITE,
    BRUSH_HOLLOW,
	NUMBER_OF_BRUSHES
};


enum FONTS {
    FN_ARIAL,
    FN_COURIER,
    FN_TIMES,
    FN_SYMBOL,
    FN_NUMBER
};


#endif
