      subroutine elect(shp,ul,el,ndf,ndm,nel)

      implicit none       
      integer i,k,ndf,nel,ndm

      real*8 shp(nel,0:ndm),ul(ndf,*),el(ndm)

      do i = 1,ndm
         el(i) = 0.d0
         do k = 1,nel
            el(i) = el(i) - ul(1,k) * shp(k,i) 
         enddo
      enddo

      end
