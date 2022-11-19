/*
 * File:   CFEFunctorVectorResidualElem14.cpp
 * Author: piotrek
 *
 * Created on September 10, 2010, 11:22 AM
 */

#include "FEFunctorVectorResidualElem14.h"

#include "feap.h"

extern "C"
{
//       int __stdcall SUMIT(int *,int*);
  //     float __stdcall MEAN(float*,int*);
        void elmt14_(double*,double*,double*,int*,double*,double*,double*,int*,int*,int*,int*);
        void openfiles_(double*);
        void coupledfields_(double*,double*,int*,char*,double*,int*,int*,int*,int*);
}



CFEFunctorVectorResidualElem14::CFEFunctorVectorResidualElem14( CFEFunctorVector *a, double b, double c  ):CFEFunctorVector("CFEFunctorVectorResidualElem14"),q(a),d1(b),d2(c)
{
    AddNode( q );
    al = q->GetAllocator();

}

CFEFunctorVectorResidualElem14::CFEFunctorVectorResidualElem14( const CFEFunctorVectorResidualElem14 &mf ):CFEFunctorVector(mf),d1(mf.d1),d2(mf.d2)
{
    CLONE_MEMBER( mf, q )
}


void CFEFunctorVectorResidualElem14::Compute( CFiniteElementInstance *fe1 )
{
    const int indof = 3;
    int ndf = 5,ndm = indof, nst = 324, isw = 3,nel = fe1->GetNodes().size(), ma = 1;
    double d[20], ul[ 6 ][ 27 ][ 12 ], xl[ 27 ][ 3 ], tl[ 27 ], s[ 325 ][ 324 ];
    int dim = fe1->GetNodes().size() * ndm,k,l,n=0, inds[ 81 ], ind=0, ix[ 28 ];
    char txt[1000];
    unsigned map[27] = { 0, 2, 8, 6, 18, 20, 26, 24, 1, 5, 7, 3, 19, 23, 25, 21, 9, 11, 17, 15, 4, 22, 12, 14, 10, 16, 13 };
    unsigned dofmap[ 27 * indof ];

    d[0] = d1;
    d[1] = d2;

    for (k=0; k<27; k++)
        for (l=0; l<ndm; l++)
            dofmap[ ind++ ] = map[ k ] * ndm + l;

    ind=0;
    ix[0]=fe1->GetNumber();
    for (k=0; k<27; k++)
    {
        for (l=0; l<3; l++) inds[ ind++ ] = k * 12  + l;
        ix[k+1] = fe1->GetNodes()[ map[ k ] ]->GetNumber();
    }

    for (l=0; l<(int)fe1->GetNodes().size(); l++)
        for (k=0; k<12; k++)
            for (n=0; n<6; n++)
                ul[ n ][  l  ][  k  ] =  0.0;


    for (k=0; k< nst; k++)
    {
        s[0][k]=0.0;
        for (l=0; l<nst; l++)
             s[ l + 1 ][ k ] = 0.0;
    }

    fv.SetDim(dim);

    n=0;

    for (l=0; l<(int)fe1->GetNodes().size(); l++)
    {
        for (k=0; k<3; k++)
             ul[ 0 ][ l ][ k ] =  (*q)[ dofmap[ n++ ] ];
    }

    for (l=0; l<(int)fe1->GetNodes().size(); l++)
    {
        mvector X;
        fe1->GetNodes()[ map[ l ] ]->GetX( X );
        for (k=0; k<3; k++)
            xl[  l  ][  k  ] = X[ k ];
    }

    //elmt14(  d,     ul,           xl,       ix,  tl,     s,        p,       ndf,  ndm,  nst,  isw)
     elmt14_( &d[0], &ul[0][0][0], &xl[0][0], ix, &tl[0], &s[1][0], &s[0][0], &ndf, &ndm, &nst, &isw);

//    coupledfields_(&ul[0][0][0],&xl[0][0],ix,&txt[0],&s[0][0],&ndf,&nel,&nst,&isw);

    for (k=0; k<dim; k++)
        fv[ dofmap[ k ] ] = -s[0][ inds[ k ] ];
}
