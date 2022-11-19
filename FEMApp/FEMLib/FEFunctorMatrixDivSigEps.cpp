#include "FEFunctorMatrixDivSigEps.h"

CFEFunctorMatrixDivSigEps::CFEFunctorMatrixDivSigEps(CFEFunctorMatrix *n, CFEFunctorMatrix *s, CFEFunctorMatrix *b):CFEFunctorMatrix("DivSigEps"),N(n),srf(s),df(b)
{
    AddNode(n);
    AddNode(s);
    AddNode(b);
}

CFEFunctorMatrixDivSigEps::CFEFunctorMatrixDivSigEps( const CFEFunctorMatrixDivSigEps &mf ):CFEFunctorMatrix(mf)
{
    CLONE_MEMBER( mf, N )
    CLONE_MEMBER( mf, srf )
    CLONE_MEMBER( mf, df )
}


void CFEFunctorMatrixDivSigEps::Compute(CFiniteElementInstance *param)
{
    /*

integer i0,ik,j1,j2,j3,i,j,k,ndf,nst,nl,nr
      real*8 sl(nl,3),sr(nr,3),s(nst,*),d(6,6),a(3,6)

      i0 = 0
      do i = 1,nl
         do k = 1,6
            a(1,k)=sl(i,1)*d(1,k)+sl(i,2)*d(6,k)+sl(i,3)*d(5,k)
            a(2,k)=sl(i,1)*d(6,k)+sl(i,2)*d(2,k)+sl(i,3)*d(4,k)
            a(3,k)=sl(i,1)*d(5,k)+sl(i,2)*d(4,k)+sl(i,3)*d(3,k)
         enddo
         j1 = 1
         do j = 1,nr
            j2 = j1 + 1
            j3 = j1 + 2
            do k = 1,3
               ik = i0+k
          s(ik,j1)=s(ik,j1)+a(k,1)*sr(j,1)+a(k,6)*sr(j,2)+a(k,5)*sr(j,3)
          s(ik,j2)=s(ik,j2)+a(k,6)*sr(j,1)+a(k,2)*sr(j,2)+a(k,4)*sr(j,3)
          s(ik,j3)=s(ik,j3)+a(k,5)*sr(j,1)+a(k,4)*sr(j,2)+a(k,3)*sr(j,3)
            enddo
            j1 = j1 + ndf
         enddo                  ! i
         i0 = i0 + ndf
      enddo

     */

    matrix a(3,6);
    const matrix &sl = N->GetValue(), &d=df->GetValue(), &sr=srf->GetValue();
    matrix &s = fv;
    unsigned k, j, i, j1, j2, j3, nl = 0, nr = 0, ik, i0, ndf=0;

    for( i=0; i<nl; i++)
    {
       for( k=0; k<6; k++)
       {
          a(1,k)=sl(i,0)*d(0,k)+sl(i,1)*d(5,k)+sl(i,3)*d(4,k);
          a(2,k)=sl(i,0)*d(5,k)+sl(i,1)*d(1,k)+sl(i,3)*d(3,k);
          a(3,k)=sl(i,0)*d(4,k)+sl(i,1)*d(3,k)+sl(i,3)*d(2,k);
       }
       j1 = 0;
       for(j=0; j<nr; j++)
       {
          j2 = j1 + 1;
          j3 = j1 + 2;
          for( k=0; k<3; k++)
          {
                ik = i0 + k;
                s(ik,j1)=s(ik,j1)+a(k,0)*sr(j,0)+a(k,5)*sr(j,1)+a(k,4)*sr(j,2);
                s(ik,j2)=s(ik,j2)+a(k,5)*sr(j,0)+a(k,1)*sr(j,1)+a(k,3)*sr(j,2);
                s(ik,j3)=s(ik,j3)+a(k,4)*sr(j,0)+a(k,3)*sr(j,1)+a(k,2)*sr(j,2);
          }
          j1 = j1 + ndf;
       }
       i0 = i0 + ndf;
    }
}
