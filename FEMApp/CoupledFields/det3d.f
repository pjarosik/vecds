      function det3d(a)

      implicit none

c	Oblicza macierz odwrotna do macierzy a
c	  Input   a[3,3] - macierz oryginalna 
c	  Output  det(a) - wyznacznik 
      
      real*8 det3d,a(3,3)

      det3d = a(1,1) * a(2,2) * a(3,3) + a(2,1) * a(3,2) * a(1,3)
     *      + a(3,1) * a(1,2) * a(2,3) - a(1,3) * a(2,2) * a(3,1)
     *      - a(2,3) * a(3,2) * a(1,1) - a(3,3) * a(1,2) * a(2,1)
      return
      end
