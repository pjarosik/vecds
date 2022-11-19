      subroutine divpolel(sl,nl,sr,nr,d,s,ndf,nst)

      implicit none
      integer i1,j1,i,j,ndf,nst,nl,nr
      real*8 sl(nl,3),sr(nr,3),s(nst,nst),d(3,3),a1,a2,a3

      i1 = 1
      do i = 1,nl
         a1 = sl(i,1)*d(1,1) +sl(i,2)*d(2,1) +sl(i,3)*d(3,1) ! plus dispate of $ {\bf E} = - \nabla \phi $
         a2 = sl(i,1)*d(1,2) +sl(i,2)*d(2,2) +sl(i,3)*d(3,2) ! but -div(P) = 0
         a3 = sl(i,1)*d(1,3) +sl(i,2)*d(2,3) +sl(i,3)*d(3,3)
         j1 = 1
         do j = 1,nr
            s(i1,j1)= s(i1,j1) +a1*sr(j,1) +a2*sr(j,2) +a3*sr(j,3)
            j1 = j1 + ndf
         enddo                 
         i1 = i1 + ndf
      enddo

      end
