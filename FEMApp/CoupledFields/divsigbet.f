      subroutine divsigbet(sl,nl,sr,nr,eb,dd,s,ndf,nst)

      implicit none
      integer i1,i2,i3,j1,i,j,ndf,nst,nl,nr
      real*8 sl(nl,3),sr(nr),s(nst,*),dd(6,6),db(6),a1,a2,a3,eb(6)

      do i = 1,6
         db(i) =  dd(i,1)*eb(1) + dd(i,2)*eb(2) + dd(i,3)*eb(3)
     $          + dd(i,4)*eb(4) + dd(i,5)*eb(5) + dd(i,6)*eb(6)
      enddo

      i1 = 1
      do i = 1,nl
         i2 = i1 + 1
         i3 = i1 + 2
         a1 = sl(i,1)*db(1) + sl(i,2)*db(6) + sl(i,3)*db(5)
         a2 = sl(i,1)*db(6) + sl(i,2)*db(2) + sl(i,3)*db(4)
         a3 = sl(i,1)*db(5) + sl(i,2)*db(4) + sl(i,3)*db(3)
         j1 = 1
         do j = 1,nr
            s(i1,j1) = s(i1,j1) + a1*sr(j)
            s(i2,j1) = s(i2,j1) + a2*sr(j)
            s(i3,j1) = s(i3,j1) + a3*sr(j)
            j1 = j1 + ndf
         enddo
         i1 = i1 + ndf
      enddo
      end
