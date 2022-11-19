      subroutine divxv(shp,nel,x,v,p,ndf,ndm)

      implicit none
      integer i,k,k1,ndm,ndf,nel
      real*8 shp(nel,ndm),p(*),x,v(ndm),xvdv

      do i = 1,ndm
         xvdv = v(i)*x*(1.-x)
         k1 = 1
         do k = 1,nel
            p(k1) = p(k1) + shp(k,i)*xvdv
            k1 = k1 + ndf
         enddo
      enddo

      end
