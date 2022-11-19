/*
 * File:   CFEFunctorMatrixTangentElem14.cpp
 * Author: piotrek
 *
 * Created on May 12, 2010, 3:19 PM
 */

#include "FEFunctorMatrixTangentElem14.h"

#include "feap.h"

extern "C"
{
//       int __stdcall SUMIT(int *,int*);
  //     float __stdcall MEAN(float*,int*);
  // //        coupledfields(ul,xl,ix,txt,s,ndf,nel,nst,isw)
// subroutine elmt14(d,ul,xl,ix,tl,s,p,ndf,ndm,nst,isw)
        void  elmt14_(double*,double*,double*,int*,double*,double*,double*,int*,int*,int*,int*);
        void  elmt15_(double*,double*,double*,double*,double*,double*,double*,int*,int*,int*,int*,int*,int*);
        void  coupledfields_(double*,double*,int*,char*,double*,int*,int*,int*,int*);
        void  user_init(int*,int*);
        void  openfiles_(double*);
}



CFEFunctorMatrixTangentElem14::CFEFunctorMatrixTangentElem14( CFEFunctorVector *qe, double a, double b ):CFEFunctorMatrix("FEAPTangentMatrixElem14"),q(qe),d1(a),d2(b)
{

    AddNode( q );
    alr = alc = qe->GetAllocator();

}

CFEFunctorMatrixTangentElem14::CFEFunctorMatrixTangentElem14( const CFEFunctorMatrixTangentElem14 &mf ):CFEFunctorMatrix(mf),d1(mf.d1),d2(mf.d2)
{
    CLONE_MEMBER( mf, q )
}


void CFEFunctorMatrixTangentElem14::Compute( CFiniteElementInstance *fe1 )
{
    const int indof = 3;
    int ndf = 5, ndm = indof, nst = 324, isw = 3, nel = fe1->GetNodes().size(), ma = 1;
    double d[20], ul[ 6 ][ 27 ][ 12 ], xl[ 27 ][ 3 ], tl[ 27 ], s[ 325 ][ 324 ];
    int dim = fe1->GetNodes().size() * ndm, k,l,n=0, inds[ 81 ], ind=0, ix[ 28 ];
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
        ix[k+1] = fe1->GetNodes()[ map[ k ] ]->GetNumber() ;
    }

    for (l=0; l<(int)fe1->GetNodes().size(); l++)
        for (k=0; k<12; k++)
            for (n=0; n<6; n++)
                ul[ n ][  l  ][  k  ] =  0.0;

    matrix &Kt = *this;

    for (k=0; k< nst; k++)
    {
        s[0][k]=0.0;
        for (l=0; l<nst; l++)
             s[ l + 1 ][ k ] = 0.0;
    }

    Rt.SetDim( dim );
    fv.SetDim(dim,dim);

    n=0;

    for (l=0; l<(int)fe1->GetNodes().size(); l++)
    {
        for (k=0; k<3; k++)
        {
             ul[ 0 ][ l ][ k ] = (*q)[ dofmap[ n++ ] ];
             if ( ul[ 0 ][ l ][ k ] < 0.0 ) {
                 int a=1;
             }
        }
    }

    mvector X;
    for (l=0; l<(int)fe1->GetNodes().size(); l++)
    {
        fe1->GetNodes()[ map[ l ] ]->GetX(X);
        for (k=0; k<3; k++)
            xl[  l  ][  k  ] = X[ k ];
    }

//    elmt15_(d,&ul[0][0][0],&xl[0][0],ix,tl,&s[0][0],p,&nel,&ma,&ndf,&ndm,&nst,&isw);

// subroutine coupledfields(ul,xl,ix,txt,s,ndf,nel,nst,isw)
//  elmt14(d,ul,xl,ix,tl,s,p,ndf,ndm,nst,isw)

   //elmt14(  d,     ul,           xl,       ix,  tl,     s,        p,     ndf,  ndm,  nst,  isw)
     elmt14_( &d[0], &ul[0][0][0], &xl[0][0], ix, &tl[0], &s[1][0], &s[0][0], &ndf, &ndm, &nst, &isw);

    //coupledfields_(&ul[0][0][0],&xl[0][0],ix,&txt[0],&s[0][0],&ndf,&nel,&nst,&isw);

    for (k=0; k<dim; k++)
    {
        Rt[ dofmap[ k ] ] = -s[0][ inds[ k ] ];
        for (l=0; l<dim; l++)
            Kt( dofmap[k], dofmap[l] ) = s[ inds[ l ] + 1 ][ inds[ k ] ];
    }
//    Rt.print("Residual vector");
//    Kt.print("Tangent matrix");
}
