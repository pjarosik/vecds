      subroutine divpolx(sl,nl,sr,nr,d,s,ndf,nst)

      implicit none
      integer i1,j1,i,j,ndf,nst,nl,nr
      real*8 sl(nl,3),sr(nr),s(nst,nst),d(3),a

      i1 = 1
      do i = 1,nl
         a = - sl(i,1)*d(1) - sl(i,2)*d(2) - sl(i,3)*d(3)
         j1 = 1
         do j = 1,nr
            s(i1,j1) = s(i1,j1) + a * sr(j)
            j1 = j1 + ndf
         enddo             
         i1 = i1 + ndf
      enddo                
      end
