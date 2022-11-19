      subroutine divsigx(sl,nl,sr,nr,dch,s,ndf,nst)

c     written by Paweł Dłużewski in 20005 (utf8)
      implicit none
      integer i1,i2,i3,j1,i,j,ndf,nst,nl,nr
      real*8 sl(nl,3),sr(nr),s(nst,nst),dch(6),a1,a2,a3

      i1 = 1
      do i = 1,nl
         i2 = i1 + 1
         i3 = i1 + 2
         a1 = sl(i,1)*dch(1) + sl(i,2)*dch(6) + sl(i,3)*dch(5)
         a2 = sl(i,1)*dch(6) + sl(i,2)*dch(2) + sl(i,3)*dch(4)
         a3 = sl(i,1)*dch(5) + sl(i,2)*dch(4) + sl(i,3)*dch(3)
         j1 = 1
         do j = 1,nr
            s(i1,j1)=s(i1,j1) + a1*sr(j)
            s(i2,j1)=s(i2,j1) + a2*sr(j)
            s(i3,j1)=s(i3,j1) + a3*sr(j)
            j1 = j1 + ndf
         enddo             
         i1 = i1 + ndf
      enddo                
      end
