      subroutine seth(u,eps,m)  ! scalar strain function, written by Paweł Dłużewski (utf-8)
c      input:   u - stretch
c               m - strain expotential coefficient in the Seth formula
c     output:   eps(0) - f(u)  strain
c               eps(1) - f'(u) its 1st derivative   
c               eps(2) - f"(u) its 2nd derivative
      
      implicit none
      real*8 u,eps(0:2), m, err
      parameter ( err = 1.d-16 )

      if (m*m.lt.err*err) then      
         eps(0) = log(u)             
      else                           
         eps(0) = 1./m*(u**m -1.0d0) 
      endif 
      eps(1) = u**(m-1)       
      eps(2) = (m-1)*u**(m-2)         
      end
