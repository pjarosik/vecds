      subroutine shape3d(ss,detf,shp,xl,nel)
c      Purpose: Compute 3-d shape functions and their 1st-3rd order covariant derivatives

c      Inputs:
c         ss(3)     - Natural coordinates of point
c         detf      - Determinant of deformation gradient from normal to Eucleadean confs
c         shp       - shape function
c         xl(3,nel) - Nodal coordinates for element
c         nel       - Number of nodes attached to element
c      Outputs:
c           g       - Jacobian matrix in the Gauss point
c         shp(nel,0:19)  - Shape function and its derivatives at Gauss point
c         shp(i,0) =  N_i

c     shp(i, 1) = dN_i/dx          shp(i, 4) = d^2N_i/dxdx     shp(i, 7) = d^2N_i/dzdy
c     shp(i, 2) = dN_i/dy          shp(i, 5) = d^2N_i/dydy     shp(i, 8) = d^2N_i/dxdz
c     shp(i, 3) = dN_i/dz          shp(i, 6) = d^2N_i/dzdz     shp(i, 9) = d^2N_i/dydx
c     shp(i,10) = d^3N_i/dxdxdx    shp(i,13) = d^3N_i/dxdxdz   shp(i,16) = d^3N_i/dzdydy
c     shp(i,11) = d^3N_i/dydydy    shp(i,14) = d^3N_i/dydydx   shp(i,17) = d^3N_i/dxdzdz  
c     shp(i,12) = d^3N_i/dzdzdz    shp(i,15) = d^3N_i/dzdzdy   shp(i,18) = d^3N_i/dydxdx
c     written by Marcin Maździarz & Paweł Dłużewski            shp(i,19) = d^3N_i/dxdydz
      implicit  none

      include  'iofile.h'

      integer   nel 
      real*8    ss(3),shp(nel,0:9),xl(3,nel),g(3,3),ch(6,3)
     $         ,detf,det3d
      common/ttshp/ g  ! for tests only, used by subroutine testshp

      if(nel.eq.8) then
         call brick8(ss, shp(1,0))       ! - shape function and its 1st grad in the normal coordinates
         call shifter(shp(1,1),nel,xl,g) ! - shifter (Jacobian matrix) 
         call isopar1(shp(1,1),nel,g)    ! - 1st derivative transition from normal to current conf

      elseif(nel.eq.27) then
         call brick27(ss,shp(1,0))
         call shifter(shp(1,1),nel,xl,g)
         call christo(shp(1,4),nel,xl,g,ch)
         call isopar2(shp(1,1),nel,g,ch)
         call isopar1(shp(1,1),nel,g)

      elseif(nel.eq.64) then
         call brick64(ss,shp(1,0))
         call shifter(shp(1,1),nel,xl,g)
         call christo(shp(1,4),nel,xl,g,ch)
         call isopar3(shp(1,1),nel,xl,g,ch)
         call isopar2(shp(1,1),nel,g,ch)
         call isopar1(shp(1,1),nel,g)

      else
         write(iow,2000) nel
 2000    format(/' ** Error ** Element with',i3,' nodes not available'/)
         stop
      endif
      detf = 1/det3d(g)

      end
