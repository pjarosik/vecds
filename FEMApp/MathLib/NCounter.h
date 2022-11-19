// NCounter.h: interface for the CNCounter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCOUNTER_H__17B4B836_7EBA_4AF0_9499_AAA38A34A634__INCLUDED_)
#define AFX_NCOUNTER_H__17B4B836_7EBA_4AF0_9499_AAA38A34A634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

class CNCounter  
{
public:
    CNCounter( unsigned d, unsigned e );
    CNCounter( unsigned d, const vector< unsigned > &s );
	virtual ~CNCounter( );
    bool operator ++( int );
    int GetIndex( unsigned k )  { return i[ k ]; }
    void Reset();
    unsigned GetSize() const { return size; }
    unsigned GetDim()     { return dim; }
    unsigned GetIndex()   { return index; }
private:
    unsigned  dim, index, size;
    vector<unsigned> i, degree;
};

#endif // !defined(AFX_NCOUNTER_H__17B4B836_7EBA_4AF0_9499_AAA38A34A634__INCLUDED_)
