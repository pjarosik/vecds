      subroutine inv3d(a,b)

      implicit none

c	Oblicza macierz odwrotna do macierzy a
c	  Input   a[3,3] - macierz oryginalna 
c	  Output  b[3,3] - macierz odwrocona 

      integer i,j
      real*8 deta
      real*8 a(3,3),b(3,3),adj(3,3)

c     Compute adjoint to a: adj(i,j) = -1**(i+j) *minor(j,i)

      call adj3d(a,adj)

c     Compute determinant of a

      deta  = a(1,1)*adj(1,1) + a(1,2)*adj(2,1) + a(1,3)*adj(3,1)

c     Compute inverse to a

      deta = 1.d0/deta
      do j = 1,3
        do i = 1,3
          b(i,j) = adj(i,j)*deta
        end do
      end do

      return
      end
