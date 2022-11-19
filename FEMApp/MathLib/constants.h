
#ifndef __MATRIX_CONSTANTS_H
#define __MATRIX_CONSTANTS_H

// CMatrix constants

const double meps = 1.e-6;

enum MAX_ARRAY_NUMBERS
{
	MAX_ELEM_NODES  = 16,
	MAX_GAUSS_NODES = 20,
	MAX_GAUSS_POINTS = MAX_GAUSS_NODES * MAX_GAUSS_NODES * MAX_GAUSS_NODES,
};

enum SHAPE_TYPES
{
    ST_RECTANGLE,
    ST_TRIANGLE,
    ST_POLYGON,
    ST_USER
};

enum LARGE_MATRIX_MAXS
{
	MAX_DIM							= 200000
};

#endif

