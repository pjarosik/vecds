      subroutine constx(v,x,nch,dch,mu_x,mu_xx,fe)        

      implicit none          
      integer i,k,l,nch
      real*8 lambda(7),x(0:3,nch),v(3,nch),dch(6,nch),mu_x(nch,nch),
     $       mu_xx(nch,nch,nch),fe(9,3),lambda0(7)
      save   lambda0
      data   lambda0/1.e-28,1.e-28,5*0./

      do k = 1,nch
      lambda(k) = lambda0(k)  
      if(x(0,k)*(1.-x(0,k)).lt.0.) lambda(k) = -lambda(k)
         do i = 1,6
            dch(i,k)  = - lambda(k) * dch(i,k)
         enddo
         do l = 1,nch
            mu_x(k,l) =  - lambda(k) * mu_x(k,l)
            do i = 1,nch
               mu_xx(k,l,i) = - lambda(k) * mu_xx(k,l,i)
            enddo
         enddo
         do i = 1,3
            v(i,k) =   dch(1,k) *  fe(1,i)
     $               + dch(2,k) *  fe(5,i)          
     $               + dch(3,k) *  fe(9,i)          
     $               + dch(4,k) * (fe(8,i) + fe(6,i))
     $               + dch(5,k) * (fe(3,i) + fe(7,i))
     $               + dch(6,k) * (fe(4,i) + fe(2,i))
            do l = 1,nch 
               v(i,k) = v(i,k) + mu_x(k,l) * x(i,l)
            enddo
         enddo
      enddo
      end
