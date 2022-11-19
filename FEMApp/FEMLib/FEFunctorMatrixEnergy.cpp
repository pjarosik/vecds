#include "FEFunctorMatrixEnergy.h"
#include "FEFunctorArray4.h"


CFEFunctorMatrixEnergy::CFEFunctorMatrixEnergy( CFEFunctorMatrixAAA *a ):CFEFunctorMatrix("Energy"),AAA(a)
{
    AddNode( a );
}

CFEFunctorMatrixEnergy::CFEFunctorMatrixEnergy( const CFEFunctorMatrixEnergy &mf ):CFEFunctorMatrix(mf)
{
    CLONE_MEMBER( mf, AAA )
}

double CFEFunctorMatrixEnergy::chem( const matrix &xch, int nch, matrix &mu_x, CFEFunctorArray3 &mu_xx )
{

/*


 Chemical energy function

c        input: x(0:3,nch)   - molar fractions and their spatial derivatives
c               mu(1,ij,...) - coupled 1st derivative of chemical potential (usefull to
c                              normalize the osmotic driving force in reference
c                              to stress induced driving forces

c       output: chem    - energy input
c               mu      - chemical potentials and their 1st and 2nd molar derivatives
c               mu(nchh)   nch=1 => 1,11,111                     nchh = 3
c                          nch=2 => 1,2,11,22,12,111,222,112,221 nchh = 9

      implicit none
      integer nch
      real*8 chem,xch(0:3,nch),mu_x(nch,nch),mu_xx(nch,nch,nch),f,x

*/

      double f,x;
/*

c     f(x) = (x-0.5)**4 -0.02*(x-0.5)**2 -0.029  ! InGaN
      f(x) = 30.5*(x-0.5)**6 -0.025*(x-0.5)**4 +1.
c      f(x) = 1. + 0. *  x
      mu_x(1,1) = mu_x(1,1) * f(xch(0,1))
      mu_xx(1,1,1) = mu_x(1,1)*( f(xch(0,1)+1.e-6)-f(xch(0,1)-1.e-6) )
     $     /2.e-6
      chem = 0.


 */

//      f = pow(x-0.5,4) - 0.02 * pow( x -0.5 ,2 ) - 0.029;   ! InGaN
        f = 30.5 * pow( x - 0.5, 6 ) - 0.025 * pow( x - 0.5 ,4) + 1.0;
//      c      f(x) = 1. + 0. *  x
        //mu_x(0,0) = mu_x(0,0) * f( xch(0,0) );
        //mu_xx(0,0,0) = mu_x(0,0) * ( f(xch(0,0)+1.e-6)-f(xch(0,0)-1.e-6) ) / 2.e-6;
       return  0.0;

}

void CFEFunctorMatrixEnergy::Compute( CFiniteElementInstance *fe1 )
{


    /*

      module material_parameters
      implicit none
      integer,parameter:: nchm=2
      real*8 :: c0(6,6,0:nchm)=0., cc0(6,6,6,0:nchm)=0., rde0=0.,
     $         el0(3,0:nchm)=0., elel0(3,3,0:nchm)=0.,
     $         lambda0(0:nchm)=0., elch0(3,0:nchm)=0.,ech0(6,0:nchm)=0.,
     $         piez0(3,6,0:nchm)=0.
      save c0,cc0,rde0,ech0,el0,elch0,piez0,elel0,lambda0
      end

      function  energy(eps,alpha,x,nch,el,dd,dch, del,
     $          chch,elch,
     $          elel,chchch)
c     written by P.Dluzewski  (2005) revised by ...
c             input:  (eps,alpha, x,nch,aa,dch,chch)
c            output:  (sig,alpha,mu,nch,dd,dch,ecc)

      use material_parameters
      implicit none


      integer i,j,k,nch
      real*8 eps(6),sig(6),ac(6,6),c(6,6),cc(6,6),energy,
     $       x(0:3,nch),el(3),dd(6,6),dch(6,nch),del(6,3),
     $       chch(nch,nch),chchch(nch,nch,nch),ec(6),alpha(3,3),
     $       a(6,6),rd,rdel,chem,
     $       pol(3),elch(3,nch),elel(3,3)
*/

    int nchm=2;

    CFEFunctorArray4  cc0(6,6,6,nchm + 1 );
    CFEFunctorArray3 elel0(3,3, nchm + 1 ), c0( 6, 6, nchm + 1 ), piez0( 3, 6, nchm+1 );

    matrix el0(3,nchm+1), elch0(3,nchm+1), ech0(6,nchm+1);
    mvector lambda0(nchm+1);


    int i,j,k,nch;
    double rd,rdel,energy, rde0=0.0;
    mvector eps = AAA->eps;
    mvector sig(6), el(3),ec(6), pol(3);
    matrix  ac(6,6), c(6,6), cc(6,6), x(4,nch), dd(6,6), dch(6,nch), del(6,3), chch(nch,nch), alpha(3,3), a(6,6), elch(3,nch), elel(3,3);
    CFEFunctorArray3 chchch(nch,nch,nch);


/*

      rd = dsqrt(alpha(1,3)**2 + alpha(2,3)**2 + alpha(3,3)**2)
      rdel = dexp(-rde0 * rd)
      if(rdel.lt.0.1d0) rdel = 0.1d0
      if(nch.gt.nchm) stop
      $'Error: Variable nch>nchm, see module material_parameters'

*/

        rd = sqrt( pow( alpha(0,2),2) + pow( alpha(1,2),2) + pow( alpha(2,2),2) );
        rdel = exp(-rde0 * rd );

        if( rdel < 0.1) rdel = 0.1;

        if( nch > nchm ) throw CFEException( "Error: Variable nch > nchm, see module material_parameters" );

/*


c     Spontaneous effect on thermodynamic variables
c      if(x(0,1).eq.1.) then
c         write(*,*) sig
c      endif
c     Chemical effect on thermodynamic variables
      do i = 1,3
         el(i) = el(i) - el0(i,0) ! el - el_ch
      enddo
      do k = 1,nch
         do i = 1,6
            eps(i) = eps(i) - ech0(i,k) *x(0,k) ! eps - eps_ch
         enddo
         do i = 1,3
            el(i) =   el(i) - elch0(i,k)*x(0,k) !influence of chemical fr on P_sp
         enddo
      enddo
*/


              for (i=0; i<3; i++ )
                 el[i] = el[i] - el0(i,0); // el - el_ch
              for (k=0; k<nch; k++)
              {
                 for (i=0; i<6; i++)
                    eps[i] = eps[i] - ech0(i,k) * x(0,k); // eps - eps_ch
                 for (i=0; i<3; i++)
                    el[i] =   el[i] - elch0(i,k) * x(0,k); // influence of chemical fr on P_sp
              }


/*


c     Elastic-elastic tangent matrix
      a = dd

      do i = 1,6
         do j = 1,6
            cc(i,j) = 0.d0
            do k = 1,6
               cc(i,j) = cc(i,j) + rdel * cc0(i,j,k,0)*eps(k)
            enddo
            c( i,j) = rdel * c0(i,j,0)
         enddo
      enddo

      do i = 1,6
         do j = 1,6
            ac(i,j) = 0.
            do k = 1,6
               ac(i,j) = ac(i,j) + a(k,i)*(c(k,j)+cc(k,j))
            enddo
         enddo
         do j = 1,6
            dd(i,j) = 0.
            do k = 1,6
               dd(i,j) = dd(i,j) + ac(i,k)*a(k,j) !+ b(k,i,j)*sig(k)
            enddo
         enddo
      enddo
*/

              a = dd;

               for (i=0; i<6; i++)
                  for (j=0; j<6; j++)
                  {
                    cc(i,j) = 0.0;
                    for (k=0; k<6; k++)
                       cc(i,j) = cc(i,j) + rdel * cc0(i,j,k,0) * eps[k];
                    c( i,j) = rdel * c0(i,j,0);
                 }


              for (i=0; i<6; i++)
              {
                  for (j=0; j<6; j++)
                  {
                    ac(i,j) = 0.0;
                    for (k=0; k<6; k++)
                       ac(i,j) = ac(i,j) + a(k,i)*(c(k,j)+cc(k,j));
                 }
                 for (j=0; j<6; j++)
                 {
                    dd(i,j) = 0.0;
                     for (k=0; k<6; k++)
                       dd(i,j) = dd(i,j) + ac(i,k)*a(k,j); // !+ b(k,i,j)*sig(k);
                 }
              }

/*

c     Elastic-electric tangent matrix
      do i = 1,6
         do j = 1,3
            del(i,j) = 0.
            do k = 1,6
               del(i,j) = del(i,j) + piez0(j,k,0) * a(k,i)
            enddo
         enddo
      enddo
      do i = 1,3
         do j = 1,3
            elel(i,j) = elel0(i,j,0)
         enddo
      enddo

*/
              for (i=0; i<6; i++)
                 for (j=0; j<3; j++)
                 {
                    del(i,j) = 0.0;
                    for (k=0; k<6; k++)
                       del(i,j) = del(i,j) + piez0(j,k,0) * a(k,i);
                 }

              for (i=0; i<3; i++)
                 for (j=0; j<3; j++)
                    elel(i,j) = elel0(i,j,0);

/*

c     Chemical-elastic tangent submatrix terms
      do k = 1,nch
         do i = 1,6
            dch(i,k) = 0.
            do j = 1,6
               dch(i,k) = dch(i,k) - ac(i,j) * ech0(j,k)
            enddo
            do j = 1,3
               dch(i,k) = dch(i,k) - del(i,j) * elch0(j,k)
            enddo
         enddo
      enddo

*/

             for (k=0; k<nch; k++)
                 for (i=0; i<6; i++)
                 {
                    dch(i,k) = 0.0;
                    for (j=0; j<6; j++)
                       dch(i,k) = dch(i,k) - ac(i,j) * ech0(j,k);
                    for (j=0; j<3; j++)
                       dch(i,k) = dch(i,k) - del(i,j) * elch0(j,k);
                 }


/*

c    Chemical-chemical terms
      if(nch.ne.0) then
         do i = 1,nch
            do j = 1,6
               ec(j) = 0.
               do k = 1,6
                  ec(j) = ec(j) - ech0(k,i)*c(k,j) ! negative sign
               enddo
            enddo
            do j = 1,nch
               chch(i,j) = 0.
               do k = 1,6
                  chch(i,j) = chch(i,j) - ec(k)*ech0(k,j)
               enddo
               do k = 1,3
                  chch(i,j) = chch(i,j) - elch(k,i)*elch0(k,j)
               enddo
            enddo
         enddo
         energy = chem(x,nch,chch,chchch)
      else
         energy = 0.
      endif

*/

             if(nch != 0)
             {
                for (i=0; i<nch; i++)
                {
                   for (j=0; j<6; j++)
                   {
                      ec[j] = 0.0;
                      for (k=0; k<6; k++)
                         ec[j] = ec[j] - ech0(k,i)*c(k,j); // ! negative sign
                   }
                   for (j=0; j<nch; j++)
                   {
                      chch(i,j) = 0.0;
                      for (k=0; k<6; k++)
                         chch(i,j) = chch(i,j) - ec[k]*ech0(k,j);
                      for (k=0; k<3; k++)
                         chch(i,j) = chch(i,j) - elch(k,i)*elch0(k,j);

                   }
                }
                energy = chem( x, nch, chch,chchch );
             }
             else energy = 0.0;

/*

c     Elastic stress and its elemental energy
      do i = 1,6
         sig(i) = del(i,1)*el(1)+del(i,2)*el(2)+del(i,3)*el(3) ! piezoelectric effect
         do j = 1,6
            ac(i,j) = 0.
            do k = 1,6
               ac(i,j) = ac(i,j) + a(k,i)*(c(k,j)+ 0.5d0* cc(k,j))
            enddo
            sig(i) = sig(i) + ac(i,j) * eps(j)
            energy = energy + 0.5 * eps(i) * c(i,j) * eps(j)
         enddo
         energy = energy + eps(i)*( piez0(1,i,0)*el(1)
     $        +piez0(2,i,0)*el(2)+piez0(3,i,0)*el(3))
      enddo
*/

             for (i=0; i<6; i++)
             {
                sig[i] = del(i,1)*el[1]+del(i,2)*el[2]+del(i,3)*el[3];  //  ! piezoelectric effect
                for (j=0; j<6; j++)
                {
                   ac(i,j) = 0.0;
                   for (k=0; k<6; k++)
                      ac(i,j) = ac(i,j) + a(k,i) * ( c(k,j)+ 0.5 * cc(k,j) );
                   sig[i] = sig[i] + ac(i,j) * eps[ j ];
                   energy = energy + 0.5 * eps[i] * c(i,j) * eps[ j ];
                }
                energy = energy + eps[i]*( piez0(1,i,0)*el[1] + piez0(2,i,0)*el[2]+piez0(3,i,0)*el[3]);
             }

/*

c     Electric force and its elemental energy
      do i = 1,3
         pol(i) = 0.
         do j = 1,3
            pol(i) = pol(i) + elel(i,j)*el(j)
            energy = energy + 0.5 * el(i)*elel(i,j)*el(j)
         enddo
         do j = 1,6
            pol(i) = pol(i) + piez0(i,j,0)* eps(j)
         enddo
      enddo

*/
             for (i=0; i<3; i++)
             {
                pol[ i ] = 0.0;
                for (j=0; j<6; j++)
                {
                   pol[i] = pol[i] + elel(i,j)*el[j];
                   energy = energy + 0.5 * el[i]*elel(i,j)*el[j];
                }
                for (j=0; j<6; j++)
                   pol[i] = pol[i] + piez0(i,j,0)* eps[j];
             }

/*

c     Change input thermodynamic variables into output thermodynamic forces

      do i = 1,6
         eps(i) = sig(i)
      enddo
      do i = 1,3
         el(i) = pol(i)
      enddo

      return
      end

     */


             for (i=0; i<6; i++)
                eps[i] = sig[i];
             for (i=0; i<3; i++)
                el[i] = pol[i];

}
