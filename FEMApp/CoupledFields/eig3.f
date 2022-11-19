      subroutine eig3(a,w)
c     input: a(n,n) - symmetric   ! output:  w(n) - ordered eigenvalues
c                     real matrix !        a(n,n) - eigenvectors 
c     written by Paweł Dłużewski & Jan Cholewiński, 2015 (utf-8)

      implicit  none              ! wystarczy zmienić n i otrzymujemy  
      integer, parameter :: n = 3 ! eig* dla dowolnego wymiaru macierzy 

      integer, parameter :: lda = n, lwork= 3*n-1
      integer i,j
      double precision a(n,n), w(n), work(lwork)

c     from LAPACK: 
      call dsyev('Vectors', 'Upper', n, a, lda, w, work, lwork , i)
      
      if(i.eq.0) then
         return
      elseif(i.gt.0) then
         write(*,*) 'ERROR: eig3d',i,' off-diagonal elements of an',
     $        ' intermediate tridiagonal form did not converge to zero'
         stop
      else
         write(*,*)'ERROR: eig3 ',-i,'-th argument had an illegal value'
         do j = 1,n
            write(*,*) (a(i,j),i=1,n)
         enddo
         stop
      endif

      end
