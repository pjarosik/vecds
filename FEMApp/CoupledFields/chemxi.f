      subroutine chemxi(shp,ul,x,nch,ndf,ndm,nel)

      implicit none       
      integer i,j,k,nch,ndf,nel,ndm

      real*8 shp(nel,0:ndm),ul(ndf,*),x(0:ndm,nch)

      do i = 0,ndm
         do j = 1,nch
            x(i,j) = 0.d0
            do k = 1,nel
               x(i,j) = x(i,j) + shp(k,i) * ul(j,k) 
            enddo
         enddo
      enddo

      end
