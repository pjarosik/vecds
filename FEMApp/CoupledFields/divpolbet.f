      subroutine divpolbet(sl,nl,sr,nr,eb,del,s,ndf,nst)

      implicit none
      integer i1,j1,i,j,ndf,nst,nl,nr
      real*8 sl(nl,3),sr(nr),s(nst,*),del(6,3),pob(3),a,eb(6)

      do i = 1,3
         pob(i)= -del(1,i)*eb(1) -del(2,i)*eb(2) -del(3,i)*eb(3)
     $           -del(4,i)*eb(4) -del(5,i)*eb(5) -del(6,i)*eb(6)
      enddo

      i1 = 1
      do i = 1,nl
         a = sl(i,1)*pob(1) + sl(i,2)*pob(2) + sl(i,3)*pob(3)
         j1 = 1
         do j = 1,nr
            s(i1,j1) = s(i1,j1) + a * sr(j)
            j1 = j1 + ndf
         enddo               
         i1 = i1 + ndf
      enddo

      end
