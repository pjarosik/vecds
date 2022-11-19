#include "FiniteElementCoupledFields.h"
#include "DOFs.h"
#include "FEFunctorMatrixCoupledFieldsNodalResults.h"
#include "FEFunctorMatrixCoupledFieldsNodalResults.h"
#include "FEFunctorVectorElemDOFs.h"
#include "FEFunctorVectorResidualElem14.h"
#include "feap.h"

extern "C"
{
//       int __stdcall SUMIT(int *,int*);
  //     float __stdcall MEAN(float*,int*);
        void  elmt14_(double*,double*,double*,int*,double*,double*,double*,int*,int*,int*,int*);
        void  coupledfields_(double*,double*,int*,char*,double*,int*,int*,int*,int*);
        void  openfiles_(double*);
}

CFiniteElementCoupledFields::CFiniteElementCoupledFields(
        vector< FEDomain* > vdom,
        CMaterial &mat,
        CIntegrationManager &IM
        ):CFiniteElement("CoupledFields",vdom,mat,IM)
{
}

void CFiniteElementCoupledFields::CreateElement()
{
    int ndf=5, ndm=3, nst=324, isw=1, nel=0, ma=1;
//    double d[20],ul[ 3 * 64 * 6 ], xl[ 3 * 64 ], ix[ 64 ], tl[ 64 ],s[ 192 ][ 192 ],p[ 192 ];

    double d[20], ul[ 6 ][ 27 ][ 12 ], xl[ 27 ][ 3 ], tl[ 27 ], s[ 325 ][ 324 ],p[ 325 ];
    int k,l,n=0, inds[ 81 ], ind=0, ix[ 28 ];
    char txt[10][10];
    for (k=0; k<10;k++)
        for (l=0; l<10; l++)txt[k][l]=0;
    unsigned map[27] = { 0, 2, 8, 6, 18, 20, 26, 24, 1, 5, 7, 3, 19, 23, 25, 21, 9, 11, 17, 15, 4, 22, 12, 14, 10, 16, 13 };

    //elmt14(  d,     ul,           xl,       ix,  tl,     s,        p,     ndf,  ndm,  nst,  isw)
     elmt14_( &d[0], &ul[0][0][0], &xl[0][0], ix, &tl[0], &s[0][0], &p[0], &ndf, &ndm, &nst, &isw);
     //coupledfields_(&ul[0][0][0],&xl[0][0],ix,&txt[0][0],&s[0][0],&ndf,&nel,&nst,&isw);
     double d1 = d[0], d2 = d[1];

    CDOF* tp[] = { &CDOFs::Ux, &CDOFs::Uy, &CDOFs::Uz };
    CFEFunctorVectorElemDOFs q( tp, 3, "Solid displacement vector" ); // q.printdiag=true;
    V["q"] = dynamic_cast< CFEFunctorVector* >( q.CloneTree() );

    CFEFunctorMatrixTangentElem14 Kte( &q, d[0], d[1] );      Kte.printdiag  = true;
    CFEFunctorVectorResidualElem14 Rs( &q, d[0], d[1] );       Rs.printdiag  = true;
    vector<string> names( 10 );
    for (k=0; k<10; k++) names[ k ] = txt[ k ];

    M["Kt"] = dynamic_cast< CFEFunctorMatrix* >( Kte.CloneTree() );
    V["R"]  = dynamic_cast< CFEFunctorVector* >( Rs.CloneTree()  );

    CFEFunctorMatrixCoupledFieldsNodalResults res( &q, names );
    VR["nR"] = dynamic_cast< CFEFunctorVector* >( res.CloneTree() );
}
