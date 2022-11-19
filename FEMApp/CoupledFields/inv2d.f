      subroutine inv2d(f,finv)      

      implicit none
      real*8 detf
      real*8 f(2,2),finv(2,2)

      detf=f(1,1)*f(2,2)-f(1,2)*f(2,1)
      finv(1,1) = f(2,2)/detf
      finv(1,2) =-f(1,2)/detf
      finv(2,1) =-f(2,1)/detf
      finv(2,2) = f(1,1)/detf
      end
