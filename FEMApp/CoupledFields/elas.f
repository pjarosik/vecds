      subroutine elas(shp,u,f,ndf,nel)                

c     output:  f(i,J,0) =  F(i,J)
c              f(i,j,k) =  F(i,J),k F^-1 (J,j) = -F(i,J)  F^-1 (J,j),k
c                 written by Paweł Dłużewski (utf-8)

      implicit none
      integer nel,ndf,i,j,k

      real*8 shp(nel,9),u(ndf,*),ui(9),g(3,3),finv(3,3,0:3),f(3,3,0:3)
     $                                                       ,t(3,3,0:3)
 
c     The inverse 1st and 2nd deformation gradients  F^-1(I,ij) 
      do i = 1,3
         do j = 1,9
            ui(j) = 0.
            do k = 1,nel
               ui(j) = ui(j) - u(i,k) * shp(k,j)
            enddo
         enddo
         finv(i,1,0) = ui(1)
         finv(i,2,0) = ui(2)
         finv(i,3,0) = ui(3)
         finv(i,1,1) = ui(4)
         finv(i,2,2) = ui(5)
         finv(i,3,3) = ui(6)
         finv(i,3,2) = ui(7)
         finv(i,2,3) = ui(7)
         finv(i,1,3) = ui(8)
         finv(i,3,1) = ui(8)
         finv(i,2,1) = ui(9)
         finv(i,1,2) = ui(9)
         finv(i,i,0) = finv(i,i,0) + 1.
      enddo 

      call inv334(finv,t) 
      call inv3d(t(1,1,0),g)
      do i = 1,3
         do j = 1,3
            f(i,j,0) = t(i,j,0)   ! f(i,j,k) =  F(i,K),k F^-1(K,j) 
            do k = 1,3            !          = -F(i,K)   F^-1(K,j),k
            f(i,j,k) = t(i,1,k)*g(1,j)+t(i,2,k)*g(2,j)+t(i,3,k)*g(3,j)
            enddo
         enddo
      enddo

      end
