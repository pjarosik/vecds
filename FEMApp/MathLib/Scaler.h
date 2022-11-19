#pragma once

class CScaler
{
public:
	CScaler(double a1,double b1,double a2,double b2):min1(a1),min2(a2),max1(b1),max2(b2) { d1 = max1 - min1; d2 = max2 - min2; }
	CScaler( const CScaler &s ):min1(s.min1),min2(s.min2),max1(s.max1),max2(s.max2),d1(s.d1),d2(s.d2) { }
    ~CScaler(void);
	const CScaler& operator=( const CScaler &s ) { min1=s.min1; min2=s.min2; max1=s.max1; max2 = s.max2; d1=s.d1; d2=s.d2; return *this; }
	double scale(double x)  const  { return min2 + ( x - min1 ) * d2 / d1; }
	double rescale(double y) const { return min1 + ( y - min2 ) * d1 / d2; }
	double GetMin1() const { return min1; }
	double GetMin2() const { return min2; }
	double GetMax1() const { return max1; }
	double GetMax2() const { return max2; }
    bool IsInX( double x ) const { return min1 < x < max1; }
    bool IsInY( double y ) const { return min2 < y < max2; }

protected:

	CScaler();
	double min1, min2, max1, max2, d1, d2;
};
