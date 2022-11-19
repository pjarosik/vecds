      subroutine transpose(g)
      implicit none
      integer i,j
      real*8 g(3,3),h(3,3)

      h = g
      do i = 1,3
         do j = 1,3
            g(i,j) = h(j,i)
         enddo
      enddo

      end
