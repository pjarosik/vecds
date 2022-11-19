      function chem(xch,nch,mu_x,mu_xx)

c     Chemical energy function

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

c     f(x) = (x-0.5)**4 -0.02*(x-0.5)**2 -0.029  ! InGaN
c     f(x) = 30.5*(x-0.5)**6 -0.025*(x-0.5)**4 +1. 
c      f(x) = 1. + 0. *  x   
c      mu_x(1,1) = mu_x(1,1) * f(xch(0,1))
c      mu_xx(1,1,1) = mu_x(1,1)*( f(xch(0,1)+1.e-6)-f(xch(0,1)-1.e-6) )
c     $     /2.e-6
      chem = 0.

      end
 
c            New organization  chem(x,nch,mu)

