#include "TextDevice.h"

CTextDevice::CTextDevice():parsep('.')
{

}

string CTextDevice::CDoubleFormat::toString( double val )
{
    strstream ss;
/*    ios_base::fmtflags frm;
    ss.precision( prec );
    ss.width( width );

    if ( ffrm == FF_FLOAT ) frm |= ios_base::floatfield;
    if ( ffrm == FF_FIXED ) frm |= ios_base::fixed;
    if ( ffrm == FF_EXP )   frm |= ios_base::scientific;

    if ( jfrm == JF_LEFT )  frm |= ios_base::left;
    if ( jfrm == JF_RIGHT ) frm |= ios_base::right;

    ss.setf( frm );
    ss << val;
    return ss.str();*/
}
