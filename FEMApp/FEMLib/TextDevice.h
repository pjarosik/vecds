#ifndef TEXTDEVICE_H
#define TEXTDEVICE_H

#include <string>
#include <vector>
#include <strstream>
using namespace std;

enum floatingfrm
{
    FF_FLOAT,
    FF_FIXED,
    FF_EXP
};

enum justifyfrm
{
    JF_LEFT,
    JF_RIGHT
};

class CTextDevice
{
public:

    class CDoubleFormat
    {
        public:
        CDoubleFormat( unsigned p = 8, unsigned w = 12, floatingfrm ff = FF_EXP, justifyfrm jf = JF_RIGHT ):prec(p),width(w),ffrm(ff),jfrm(jf) { }
        string toString(double val);
        unsigned prec, width;
        floatingfrm ffrm;
        justifyfrm jfrm;
    };


    CTextDevice();

    void SetParamSeparator( char s ) { parsep = s; }

    virtual void ParamPrint( const string &name, const string &value );
    virtual void ParamPrint( const string &name, double value );
    virtual void ParamPrint( const string &name, unsigned value );
    virtual void ParamPrint( const string &name, int value );
    virtual void ParamHeader(const string &title);
    virtual void ParamEnd();

    virtual void TableHeader( const string &title );
    virtual void TableSetting( const string &names, vector< CDoubleFormat > frms );
    virtual void TableLinePrint( const vector< double > vals );
    virtual void TableLinePrint( const vector< unsigned > vals );
    virtual void TableLinePrint( const vector< int > vals );
    virtual void TableLinePrint( const vector< string > vals );
    virtual void TableEnd();

    virtual void FlushLine( const string &ln )=0;
    string toString( double val );

protected:

    CDoubleFormat paramfrm, tablefrm, currentfrm;
    char parsep;
    bool is_fixed;
    string line;

};




#endif // TEXTDEVICE_H
