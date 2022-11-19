      subroutine brick8(ss,shp)

c      Inputs:
c         ss(3)     - Natural coordinates of point
c         xl(3,8) - Nodal coordinates for element

c      Outputs:
c         detf       - Jacobian determinant at point
c         shp(8,0:3) - Shape functions and derivatives at point
c                     shp(i,0) =  N_i
c                     shp(i,1) = dN_i/dx
c                     shp(i,2) = dN_i/dy
c                     shp(i,3) = dN_i/dz
c-----[--.----+----.----+----.-----------------------------------------]
      implicit  none

      real*8    ap1,am1,ap2,am2,ap3,am3, c1,c2,c3,ss(3),shp(8,0:3)

c       Compute shape functions and their natural coord. derivatives

      ap1 = 1.0d0 + ss(1)
      am1 = 1.0d0 - ss(1)
      ap2 = 1.0d0 + ss(2)
      am2 = 1.0d0 - ss(2)
      ap3 = 1.0d0 + ss(3)
      am3 = 1.0d0 - ss(3)

c       Compute for ( - , - ) values

      c1      = 0.125d0*am1*am2
      c2      = 0.125d0*am2*am3
      c3      = 0.125d0*am1*am3
      shp(1,1) = -c2
      shp(2,1) =  c2
      shp(1,2) = -c3
      shp(4,2) =  c3
      shp(1,3) = -c1
      shp(5,3) =  c1
      shp(1,0) =  c1*am3
      shp(5,0) =  c1*ap3

c       Compute for ( + , + ) values

      c1      = 0.125d0*ap1*ap2
      c2      = 0.125d0*ap2*ap3
      c3      = 0.125d0*ap1*ap3
      shp(8,1) = -c2
      shp(7,1) =  c2
      shp(6,2) = -c3
      shp(7,2) =  c3
      shp(3,3) = -c1
      shp(7,3) =  c1
      shp(3,0) =  c1*am3
      shp(7,0) =  c1*ap3

c       Compute for ( - , + ) values

      c1      = 0.125d0*am1*ap2
      c2      = 0.125d0*am2*ap3
      c3      = 0.125d0*am1*ap3
      shp(5,1) = -c2
      shp(6,1) =  c2
      shp(5,2) = -c3
      shp(8,2) =  c3
      shp(4,3) = -c1
      shp(8,3) =  c1
      shp(4,0) =  c1*am3
      shp(8,0) =  c1*ap3

c       Compute for ( + , - ) values

      c1      = 0.125d0*ap1*am2
      c2      = 0.125d0*ap2*am3
      c3      = 0.125d0*ap1*am3
      shp(4,1) = -c2
      shp(3,1) =  c2
      shp(2,2) = -c3
      shp(3,2) =  c3
      shp(2,3) = -c1
      shp(6,3) =  c1
      shp(2,0) =  c1*am3
      shp(6,0) =  c1*ap3
      end
