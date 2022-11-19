#include "FEFunctorMatrixAAA.h"


CFEFunctorMatrixAAA::CFEFunctorMatrixAAA( CFEFunctorMatrix *a ):CFEFunctorMatrix("AAA"),Fe(a), bb( 6, 6, 6, "bb" )
{
    AddNode( a );
}

CFEFunctorMatrixAAA::CFEFunctorMatrixAAA( const CFEFunctorMatrixAAA &mf ):CFEFunctorMatrix(mf),bb( mf.bb )
{
    CLONE_MEMBER( mf, Fe )
}

void CFEFunctorMatrixAAA::Compute( CFiniteElementInstance *fe1 )
{
    matrix &aa = fv;
    const matrix &fe = Fe->GetValue();
    aa.SetDim( 6, 6 );

    /*
            r(3,3),u(6,6),v(6,6),eps(6),w(3),u0(3,3),v0(3,3),
         $      e(3),aa(6,6),a(6),bb(6,6,6),b(3,3,3),fe(3,3),err,
         $      u1,u2,u3,u11,u22,u33,a4,a5,a6,feps,deps,x

    */

    int i,j,k,l,m,n,ij,ik,jk,rot;
    double x,err = 1.e-8, u1,u2,u3,u11,u22,u33,a4,a5,a6;
    mvector w(3), e(3), a(6);
    matrix r(3,3), u(6,6), v(6,6), u0(3,3), v0(3,3);
    CFEFunctorArray3 b(3,3,3);
//       data inxvs/1,6,5,6,2,4,5,4,3/!,inxv/1,2,3,4,5,6,7,8,9/
    unsigned inxvs[3][3] = {
                            {0,5,4},
                            {5,1,3},
                            {4,3,2}
                        };


    fe.polard3d(r,u0);
    u0.eig3d(w,rot);
/*
    do i = 1,3
             e(i)  = feps(w(i))! obliczanie i-tej warto¶ci w³asnej t.odszt.
                               ! zale¿nej od wart.w³. w(i) stretch tensor
             a(i)  = w(i)*deps(w(i))! obliczanie 3 pierwszych sk³. diag.
                                    ! macierzy ${A^6}_6$ Eq.(16)
          enddo
*/
    for (i=0; i<3; i++)
    {
        e[i] = log(w[i]); //! obliczanie i-tej warto¶ci w³asnej t.odszt.    ! zale¿nej od wart.w³. w(i) stretch tensor
        a[i] = 1.0; //! obliczanie 3 pierwszych sk³. diag. ! macierzy ${A^6}_6$ Eq.(16)
    }

/*
    c     Obliczanie sk³adowych 4,5,6 macierzy A
          do i = 1,2
             do j = i+1,3
                ij = inxvs(i,j)
                x  = e(i)-e(j)
                if(x*x.gt.err) then
                   a(ij) = w(i)*w(j)*x/(w(i)*w(i)-w(j)*w(j))
                else
                   a(ij) = 0.25*(a(i)+a(j))
                endif
             enddo
          enddo

*/

    for (i=0; i<2; i++)
       for (j=i+1; j<3; j++)
       {
          ij = inxvs[i][j];
          x  = e[i]-e[j];
          if ( x * x > err ) a[ij] = w[i] * w[j] * x / ( w[i] * w[i] - w[j] * w[j] );
          else a[ij] = 0.25*( a[i] + a[j] );
       }

/*

c     Obliczenia sk³adowych macierzy B w bazie wektorów w³asnych
      do i = 1,3
         do j = i,3
            do k = j,3
               u1  = w(i)
               u11 = u1*u1
               a4 = e(j)-e(k)

               u2  = w(j)
               u22 = u2*u2
               a5 = e(k)-e(i)

               u3  = w(k)
               u33 = u3*u3
               a6 = e(i)-e(j)

               if(a6*a6.lt.err.and.a4*a4.lt.err) then
                  b(i,j,k) = - 0.25
               elseif(a4*a4.lt.err)then
                 b(i,j,k) = (2.*u22*a(6)-u11+u22)/(2*(u11-u22)**2)*u1*u3
               elseif(a5*a5.lt.err) then
                 b(i,j,k) = (2.*u33*a(4)-u22+u33)/(2*(u22-u33)**2)*u3*u2
               elseif(a6*a6.lt.err)then
                 b(i,j,k) = (2.*u11*a(5)-u33+u11)/(2*(u33-u11)**2)*u3*u3
               else
               b(i,j,k) = (e(i)*(u22-u33)+e(j)*(u33-u11)+e(k)*(u11-u22))
     $                 /((u11-u33)*(u22-u11)*(u33-u22)) *u1*u2*u3*u3
               endif
            enddo
         enddo
         b(i,i,i) = -2.
      enddo
*/

    for ( i=0; i<3; i++)
    {
       for ( j=0; j<3; j++)
          for ( k=j; i<3; i++)
          {
             u1  = w[i];
             u11 = u1*u1;
             a4 = e[j]-e[k];

             u2  = w[j];
             u22 = u2*u2;
             a5 = e[k]-e[i];

             u3  = w[k];
             u33 = u3*u3;
             a6 = e[i]-e[j];

             if ( a6 * a6 < err && a4 * a4 < err )  b(i,j,k) = - 0.25;
             else if ( a4*a4 < err )  b(i,j,k) = ( 2.0 * u22 * a[5] - u11 + u22 ) / ( 2.0 * pow( u11 - u22 ,2 ) ) * u1 * u3;
                  else if( a5*a5 < err) b(i,j,k) = ( 2.0 * u33 * a[3] - u22 + u33 ) / ( 2.0 * pow( u22 - u33, 2 ) ) *u3 * u2;
                       else if( a6 * a6 < err ) b(i,j,k) = ( 2.0 * u11 * a[4] - u33 + u11 ) / ( 2.0 * pow( u33 -u11 , 2 ) ) * u3 * u3;
                            else b(i,j,k) = ( e[i] * ( u22 - u33 ) + e[ j ] * ( u33 - u11 ) + e[ k ] * ( u11 - u22 ) ) / ( ( u11 - u33 ) * ( u22 - u11) * ( u33 - u22 ) )  * u1 * u2 * u3 * u3;

          }
       b(i,i,i) = -2.0;
    }

/*


c     Rotation of stretch eigenvector to current configuration
      do i = 1,3
         do j = 1,3
            v0(i,j) = r(i,1)*u0(1,j) + r(i,2)*u0(2,j) + r(i,3)*u0(3,j)
         enddo
      enddo
*/
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            v0(i,j) = r(i,0)*u0(0,j) + r(i,1)*u0(1,j) + r(i,2)*u0(2,j);

/*
      call sym33to66(u0,u)
      call sym33to66(v0,v)
*/

    u0.sym33to66(u);
    v0.sym33to66(v);

/*

      do i = 1,6
         eps(i) = e(1)*u(i,1) +e(2)*u(i,2) +e(3)*u(i,3)
*/
    for (i=0; i<6; i++)
        eps[i] = e[0]*u(i,0) +e[1]*u(i,1) +e[2]*u(i,2);
/*

c        Podwajanie eps(4:6) na koncu procedury!!!
      enddo
c     Rzutowanie skadowych na osie laboratoryjne
      do i = 1,6         !  r - t.obrotu,
         do j = 1,6      ! u0 - t.rzutowania z b.wekt.w³.dobazy.lab.Lag
            aa(i,j) = 0. ! v0 - t.rzutowania z b.wekt.w³.dobazy lab.Eul
            do k = 1,6
               aa(i,j) = aa(i,j) + a(k) * u(i,k) * v(j,k) ! Eq. (30a)
            enddo
         enddo
      enddo
*/

    for (i=0; i<6; i++)
       for (j=0; j<6; j++)
       {
          aa(i,j) = 0.0;
          for (k=0; k<6; k++)
             aa(i,j) = aa(i,j) + a[k] * u(i,k) * v(j,k);
        }

/*
      do l = 1,6
         do m = 1,6
            do n = 1,6
               bb(l,m,n) = 0.
               do i = 1,3
                  do j = i,3
                     do k = j,3
                        ij = inxvs(i,j)
                        jk = inxvs(j,k)
                        ik = inxvs(i,k)
                        bb(l,m,n) = bb(l,m,n)
     $                      + u(l,ij)*v(m,jk)*v(n,ik) * (b(i,j,k)+a(jk))
                     enddo
                  enddo
               enddo
            enddo
         enddo
      enddo

*/

     for (l=0; l<6; l++)
        for (m=0; m<6; m++)
           for (n=0; n<6; n++)
           {
             bb(l,m,n) = 0.0;
              for (i=0; i<3; i++)
                for (j=0; j<3; j++)
                   for (k=0; k<3; k++)
                   {
                      ij = inxvs[i][j];
                      jk = inxvs[j][k];
                      ik = inxvs[i][k];
                      bb(l,m,n) = bb(l,m,n) + u(l,ij)*v(m,jk)*v(n,ik) * (b(i,j,k)+a[jk]);
                   }
            }


/*

c     Podwajanie niediagonalnych skladowych
      do l = 4,6
         do m = 1,6
            do n = 1,6
               bb(l,m,n) = bb(l,m,n) + bb(l,m,n)
            enddo
            aa(l,m) = aa(l,m) + aa(l,m)
         enddo
         eps(l) = eps(l) + eps(l)
      enddo
      end
 */

     for (l=0; l<6; l++)
     {
        for (m=0; m<6; m++)
        {
           for (n=0; n<6; n++)
              bb(l,m,n) = bb(l,m,n) + bb(l,m,n);
           aa(l,m) = aa(l,m) + aa(l,m);
        }
        eps[l] += eps[l];
     }

}



