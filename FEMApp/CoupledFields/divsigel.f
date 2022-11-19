      subroutine divsigel(sl,nl,sr,nr,d,s,ndf,nst)

      implicit none
      integer i1,i2,i3,j1,i,j,k,ndf,nst,nl,nr
      real*8 sl(nl,3),sr(nr,3),s(nst,nst),d(6,3),a1,a2,a3

      do k = 1,3
         i1 = 1
         do i = 1,nl
            i2 = i1 + 1
            i3 = i1 + 2
            a1 = - sl(i,1)*d(1,k) - sl(i,2)*d(6,k) - sl(i,3)*d(5,k)! minus because $ {\bf E} = - \nabla \phi $
            a2 = - sl(i,1)*d(6,k) - sl(i,2)*d(2,k) - sl(i,3)*d(4,k)
            a3 = - sl(i,1)*d(5,k) - sl(i,2)*d(4,k) - sl(i,3)*d(3,k)
            j1 = 1
            do j = 1,nr
               s(i1,j1) = s(i1,j1) + a1*sr(j,k) 
               s(i2,j1) = s(i2,j1) + a2*sr(j,k)
               s(i3,j1) = s(i3,j1) + a3*sr(j,k)
               j1 = j1 + ndf
            enddo               ! i
            i1 = i1 + ndf
         enddo
      enddo                     ! j
      end
