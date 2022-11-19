      module material_parameters
      implicit none
      integer,parameter:: nchm=7 
      real*8 :: c0(6,6,0:6,0:nchm),rde0,ech0(6,nchm),el0(3),
     $          elch0(3,nchm),piez0(3,6),elel0(3,3) 
      save c0,rde0,ech0,el0,elch0,piez0,elel0
      data c0/2016*0./,rde0/0./,ech0/42*0./,el0/3*0./,elch0/21*0./
     $                                         ,piez0/18*0./,elel0/9*0./
      end 
      
      function energy(sig,alpha, x,nch,el,dch,del,chch,elch,elel,chchch)
c            input:  (eps,alpha, x,nch,dch, ...)
c           output:  (sig,alpha,mu,nch,dch, ...) P.Dluzewski  (2005+)

      use material_parameters
      implicit none          

      integer i,j,k,nch
      real*8 eps(6),sig(6,0:6,0:6),ac(6,6),c(6,6),cc(6,6),sig0(6),a(6,6)
     $       ,b(6,6,6),energy,alpha(3,3),rd,rdel,
     $       x(0:3,nch),chem,chch(nch,nch),chchch(nch,nch,nch),
     $       dch(6,nch),ec(6),el(3),pol,del(6,3),elch(3,nch),elel(3,3)

      rd = dsqrt(alpha(1,3)**2 + alpha(2,3)**2 + alpha(3,3)**2)
      rdel = dexp(-rde0 * rd)
      if(rdel.lt.0.1d0) rdel = 0.1d0

      do i = 1,6
         eps(i) = sig(i,0,0)
         do j = 1,6
            a(i,j) = sig(i,j,0)
            do k = 1,6
               b(i,j,k) = sig(i,j,k)
            enddo
         enddo
      enddo

      do i = 1,3
         el(i) = el(i) - el0(i) ! el - el_ch 
      enddo
c     Chemical effect on thermodynamic variables
      do k = 1,nch
         do i = 1,6 
            eps(i) = eps(i) - ech0(i,k) *x(0,k) ! eps - eps_ch
         enddo
         do i = 1,3  !influence of chemical fr on P_sp
            el(i) =   el(i) - elch0(i,k)*x(0,k) 
         enddo
      enddo

c     Elastic-elastic 2nd and 3rd order stiffness tensors
      do i = 1,6
         do j = 1,6
            cc(i,j) = 0.d0
            do k = 1,6 ! 2nd order stiffness multiplied by strain/2
               cc(i,j) = cc(i,j) + rdel * c0(i,j,k,0)*eps(k)/2.
            enddo
            c(i,j) = rdel * c0(i,j,0,0)
         enddo
      enddo

      do i = 1,6
         sig0(i) = 0.  ! conjugate stress
         do j = 1,6
            sig0(i) = sig0(i) + (c(i,j)+cc(i,j)) * eps(j)
            ac(i,j) = 0.
            do k = 1,6
               ac(i,j) = ac(i,j) + a(k,i)*(c(k,j)+cc(k,j))
            enddo
         enddo
      enddo

c     Elastic-electric tangent matrix
      do i = 1,6
         do j = 1,3
            del(i,j) = 0.
            do k = 1,6
               del(i,j) = del(i,j) + piez0(j,k) * a(k,i)
            enddo
         enddo
      enddo
      do i = 1,3
         do j = 1,3
            elel(i,j) = elel0(i,j)
         enddo
      enddo

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
         
c     Chemical-chemical terms
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

      do i = 1,6 ! Kirchhoff stress, and elemental elastic and piezo energy
         sig(i,0,0) = del(i,1)*el(1)+del(i,2)*el(2)+del(i,3)*el(3)
         energy = energy+eps(i)*(piez0(1,i)*el(1)
     $                          +piez0(2,i)*el(2)
     $                          +piez0(3,i)*el(3))
         do j = 1,6 
            sig(i,0,0) = sig(i,0,0) + ac(i,j) * eps(j)
            energy = energy + (c(i,j)/2.+cc(i,j)/3.) *eps(i)*eps(j)
            sig(i,j,0) = 0.
            do k = 1,6   ! Tangent elastic stiffness matrix 
               sig(i,j,0) = sig(i,j,0) +ac(i,k)*a(k,j) +b(k,i,j)*sig0(k)
            enddo
         enddo
      enddo

c     Electric force and elemental energy
      do i = 1,3
         pol = elel(i,1)*el(1)+ elel(i,2)*el(2)+ elel(i,3)*el(3)
         do j = 1,3
            energy = energy + 0.5 * el(i)*elel(i,j)*el(j)
         enddo
         do j = 1,6 
            el(i) = pol + piez0(i,j)* eps(j)  
         enddo
      enddo
      
      return
      end
