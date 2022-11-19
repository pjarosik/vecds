#include "TopOptFrame.h"

CTopOptFrame::CTopOptFrame( unsigned gr ):ground_size( gr )
{
}

bool CTopOptFrame::Read( ifstream &fin )
{
    string str;
    char eq;
    fin >> str >> eq >> iteration;
    if ( fin.eof() ) return false;
    if ( str!="iteration" || eq!= '=' ) return false;

    fin >> str >> eq >> penalty;
    if ( str!="p" || eq!= '=' ) return false;

    fin >> str >> eq >> active;
    if ( str!="active" || eq!= '=' ) return false;

    fin >> str >> eq >> pasive;
    if ( str!="pasive" || eq!= '=' ) return false;

    fin >> str >> eq >> C;
    if ( str!="C" || eq!= '=' ) return false;

    fin >> str >> eq >> V;
    if ( str!="V" || eq!= '=' ) return false;

    fin >> str >> eq >> Vrel;
    if ( str!="Vrel" || eq!= '=' ) return false;

    x.SetDim( active + pasive );
    unsigned k;
    for (k=0; k<x.GetDim(); k++) fin >> x[k];

    return true;
}
