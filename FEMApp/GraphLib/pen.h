// Class automatically generated by Dev-C++ New Class wizard

#ifndef DPEN_H
#define DPEN_H

#include "color.h"

/**
 * No description
 */
class DPen
{
public:
    DPen():style(-1),thickness(0.1) {  }
    DPen( const DColor &c, int s, double t):col( c ), style( s ), thickness( t ) { }
	DPen( const DPen &p ):col( p.col ), style( p.style ), thickness( p.thickness ) { }
	const DPen& operator=( const DPen &p ) { col = p.col; style = p.style;  thickness = p.thickness; return p; }
	const DColor& GetColor() const { return col; }
	int GetStyle( ) const { return style; }
	double GetThickness( ) const { return thickness; }
    bool operator <( const DPen &pen ) const { return thickness < pen.GetThickness() || col < pen.GetColor(); }
protected:
	DColor col;
	int style;
	double thickness;
};


#endif // DPEN_H
