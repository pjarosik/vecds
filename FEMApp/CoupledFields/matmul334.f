      subroutine matmul334(f,fpinv,fl)
c     Multiplication rule for gradients of 3x3 tensors
c      input: fpinv(3,3,0:3)
c                 f(3,3,0:3)
c     output:    fl(3,3,0:3)                      programmed by Pawel Dluzewski, Christmas 2008

      implicit none
      real*8 f(3,3,0:3), fpinv(3,3,0:3), fl(3,3,0:3)
      integer i,j,m

      do i = 1,3
         do j = 1,3
            fl(i,j,0) = f(i,1,0)*fpinv(1,j,0)
     $                 +f(i,2,0)*fpinv(2,j,0)+f(i,3,0)*fpinv(3,j,0)
            do m = 1,3
               fl(i,j,m) = f(i,1,0)*fpinv(1,j,m) + f(i,1,m)*fpinv(1,j,0)
     $                    +f(i,2,0)*fpinv(2,j,m) + f(i,2,m)*fpinv(2,j,0)
     $                    +f(i,3,0)*fpinv(3,j,m) + f(i,3,m)*fpinv(3,j,0)
            enddo
         enddo
      enddo
      end

