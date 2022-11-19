#include "Renumber.h"
#include "FEException.h"
#include <fstream>
#include "FEAlgorithms.h"

CRenumber::CRenumber( const string &filename, unsigned size ):eoffset( 0 ),inds(size),revinds(size)
{
    unsigned k;
    if ( filename == "" || filename == "default" )
    {
        for (k=0; k< size; k++)
            inds[ k ] = revinds[ k ] = k;
        return;
    }

    ifstream fin(filename.c_str());
    vector<int> mis(size), eis(size);

    for (k=0; k< size; k++)
            inds[k]=revinds[k]=-1;

    if ( !fin.is_open() ) throw CFEException("Can't open filename:"+filename);

    int imin = size, imax = 0, eoffset=0;

    for (k=0; k< size; k++)
    {
        unsigned mi, ei;
        fin >> mis[ k ] >> eis[ k ];
        if ( fin.rdstate() & ios::failbit ) throw CFEException("Improper file content (should consist of two column of integer) :"+filename);
        if ( fin.eof() && k<size-1 ) throw CFEException("Not enough data in renumbering file :"+filename);
        if ( mis[k] < 0 || mis[k] >= size ) throw CFEException("Indices of FEMApp elements in renumbering file should be in range of [0..elemnodenumber-1] in file:"+filename);
        if ( eis[k] < 0 || eis[k] > size ) throw CFEException("Indices of external node renumbering list should be in range of [0..elemnodenumber] in file:"+filename);

        if ( mis[ k ] > imax ) imax = mis[ k ];
        if ( mis[ k ] < imin ) imin = mis[ k ];
    }

    if ( imin == 1 && imax == size ) eoffset= - 1;
        else if ( imin == 0 && imax == size - 1 ) eoffset=0;
            else throw CFEException("Wrong external element indices range in renumber file :" + filename );

    for (k=0; k< size; k++)
    {
        inds[ mis[ k ] ] = eis[ k ] + eoffset;
        revinds[ eis[ k ] + eoffset ] = mis[ k ];
    }



}
