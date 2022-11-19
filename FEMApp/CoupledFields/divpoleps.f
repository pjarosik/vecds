      subroutine divpoleps(sl,nl,sr,nr,d,s,ndf,nst)

      implicit none
      integer i1,j1,j2,j3,i,j,k,nl,nr,ndf,nst
      real*8 sl(nl,3),sr(nr,3),s(nst,nst),d(6,3),a(6)
   
      i1 = 1
      do i = 1,nl
         do k = 1,6
            a(k) = -sl(i,1)*d(k,1) -sl(i,2)*d(k,2) -sl(i,3)*d(k,3)
         enddo
         j1 = 1
         do j = 1,nr
            j2 = j1 + 1
            j3 = j1 + 2
            s(i1,j1)= s(i1,j1) +a(1)*sr(j,1) +a(6)*sr(j,2) +a(5)*sr(j,3)
            s(i1,j2)= s(i1,j2) +a(6)*sr(j,1) +a(2)*sr(j,2) +a(4)*sr(j,3)
            s(i1,j3)= s(i1,j3) +a(5)*sr(j,1) +a(4)*sr(j,2) +a(3)*sr(j,3)
            j1 = j1 + ndf
         enddo                  ! i
         i1 = i1 + ndf
      enddo                     ! j
      end
