      subroutine inv334(f,finv)
c     Subroutine inverts gradients of tensors F,k => F^-1,k   for k = 0:3
c            
c                input: f(3,3,0:3) 
c               output: finv(3,3,0:3),   programmed by Pawel Dluzewski, Christmas 2008
c                       finv(i,j,k): k=0   => finv(I,j)   =   f^-1(I,j)
c                                    k=1:3 => finv(I,j),k = - finv(I,m) * f(m,N),k * finv(N,j)
      implicit none                  ! f(m,I)*finv(I,j),k = -             f(m,N),k * finv(N,j)
      integer i,j,k,m,n
      real*8 f(3,3,0:3),g(3,3),finv(3,3,0:3)
      call inv3d(f(1,1,0),g)
      do i = 1,3
         do j = 1,3
            finv(i,j,0) = g(i,j)
            do k = 1,3
               finv(i,j,k) = 0.
               do m = 1,3
                  do n = 1,3
                     finv(i,j,k) = finv(i,j,k) - g(i,m)*f(m,n,k)*g(n,j)
                  enddo
               enddo
            enddo
         enddo
      enddo
      end

