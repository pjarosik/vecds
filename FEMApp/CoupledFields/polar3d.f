      subroutine polard3d(f,r,u)

c     Polar decomposition of 3d F into R * U
c       R - ortogonal matrix
c       U - symmetric matrix                             

      implicit none

      integer i,j,k,rot
      real*8 f(3,3),r(3,3),u(3,3),uinv(3,3),ulam(3),uu(3,3)	
	
      do i=1,3
         do j=1,3
            uu(i,j) = 0.d0
            do k=1,3
               uu(i,j) = uu(i,j) + f(k,i)*f(k,j) 
            enddo
         enddo
      enddo
	
      call eig3(uu,ulam,rot)
      
      do i=1,3
         ulam(i) = dsqrt(ulam(i))
      enddo


      do i = 1,3
         do j = 1,3
            u(i,j) = 0.d0
	    do k = 1,3
               u(i,j) = u(i,j) + ulam(k) * uu(i,k) * uu(j,k)
	    enddo
         enddo
      enddo

      call inv3d(u,uinv)

      r = Matmul(f,uinv)

      return
      end
