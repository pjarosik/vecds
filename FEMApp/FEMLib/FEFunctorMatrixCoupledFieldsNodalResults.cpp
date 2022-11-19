#include "FEFunctorMatrixCoupledFieldsNodalResults.h"

#include "feap.h"


extern "C"
{
//       int __stdcall SUMIT(int *,int*);
  //     float __stdcall MEAN(float*,int*);
  // //        coupledfields(ul,xl,ix,txt,s,ndf,nel,nst,isw)
        void  elmt14_(double*,double*,double*,int*,double*,double*,double*,int*,int*,int*,int*);
        void  elmt15_(double*,double*,double*,double*,double*,double*,double*,int*,int*,int*,int*,int*,int*);
        void  coupledfields_(double*,double*,int*,char*,const double*,int*,int*,int*,int*);
        void  openfiles_(double*);
}

CFEFunctorMatrixCoupledFieldsNodalResults::CFEFunctorMatrixCoupledFieldsNodalResults( CFEFunctorVector *qe, const vector<string> &nms ):CFEFunctorVector("MatrixFunctorCoupledFieldsNodalResults"),q( qe )
{
   AddNode( q );
   mnames = nms;
   al = q->GetAllocator();
}

CFEFunctorMatrixCoupledFieldsNodalResults::~CFEFunctorMatrixCoupledFieldsNodalResults()
{

}

CFEFunctorMatrixCoupledFieldsNodalResults::CFEFunctorMatrixCoupledFieldsNodalResults( const CFEFunctorMatrixCoupledFieldsNodalResults &mf ):CFEFunctorVector(mf)
{
    CLONE_MEMBER( mf, q )
}

void CFEFunctorMatrixCoupledFieldsNodalResults::Compute( CFiniteElementInstance *fe1 )
{
    const int indof = 3;
    int ndf = 5,ndm = indof, nst = 27, isw = 4, nel = fe1->GetNodes().size(), ma = 1;
    double d[20], ul[ 6 ][ 27 ][ 12 ], xl[ 27 ][ 3 ], tl[ 27 ],sc[28][27];
    int dim = fe1->GetNodes().size() * 3,k,l,n=0, inds[ 81 ], ind=0, ix[ 28 ];
    char txt[10][11];
    matrix s( nst , nst + 1 );
    unsigned map[27] = { 0, 2, 8, 6, 18, 20, 26, 24, 1, 5, 7, 3, 19, 23, 25, 21, 9, 11, 17, 15, 4, 22, 12, 14, 10, 16, 13 };
    unsigned dofmap[ 27 * indof ];


    for (k=0; k<27; k++)
        for (l=0; l<ndm; l++)
            dofmap[ ind++ ] = map[ k ] * ndm + l;

    ind=0;
    ix[0]=fe1->GetNumber();
    for (k=0; k<27; k++)
    {
        for (l=0; l<3; l++) inds[ ind++ ] = k * 12  + l;
        ix[k+1] = fe1->GetNodes()[k]->GetNumber();
    }



    for (l=0; l<(int)fe1->GetNodes().size(); l++)
        for (k=0; k<12; k++)
            for (n=0; n<6; n++)
                ul[ n ] [  l  ][  k  ] =  0.0;

    fv.SetDim(dim,nst);

    n=0;

    for (l=0; l<(int)fe1->GetNodes().size(); l++)
    {
        for (k=0; k<3; k++)
             ul[ 0 ][ l ][ k ] =  (*q)[ dofmap[ n++ ] ];
    }

    mvector X;
    for (l=0; l<(int)fe1->GetNodes().size(); l++)
    {
        fe1->GetNodes()[ l ]->GetX(X);
        for (k=0; k<3; k++)
            xl[  l  ][  k  ] = X[ k ];
    }


// subroutine coupledfields(ul,xl,ix,txt,s,ndf,nel,nst,isw)
//        subroutine elmt14_(d,ul,xl,ix,tl,s,p,ndf,ndm,nst,isw)

    //elmt14(  d,     ul,           xl,       ix,  tl,     s,        p,     ndf,  ndm,  nst,  isw)
     elmt14_( &d[0], &ul[0][0][0], &xl[0][0], ix, &tl[0], &sc[0][0], &sc[1][0], &ndf, &ndm, &nst, &isw);

    //coupledfields_(&ul[0][0][0],&xl[0][0],ix,&txt[0][0], s.GetDataPointer(),&ndf,&nel,&nst,&isw);
    //coupledfields_(&ul[0][0][0],&xl[0][0],ix,&txt[0][0], s.GetPointer().GetPointer(),&ndf,&nel,&nst,&isw);
//    coupledfields_(&ul[0][0][0],&xl[0][0],ix,&txt[0][0], &sc[0][0],&ndf,&nel,&nst,&isw);

    for (k=0; k<10; k++)
    {
        txt[k][10]=0;
        mnames[k]=txt[k];
    }

//    s.print("coupledfields results");

    const vector< CFENode* > &nodes = fe1->GetNodes();
    for (k=0; k<nodes.size(); k++)
    {
        mvector nr(10);
        double ve = s(k,0);
        for (l=1; l<=10; l++)
            nr[ l - 1 ] = s(k,l) / ve;
        nodes[ k ]->AccumulateResults( mnames, nr, 1.0 );
    }

}
