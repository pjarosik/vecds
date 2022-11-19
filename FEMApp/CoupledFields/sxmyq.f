      subroutine sxmyq(igrp,feinv,ainv)
c-----------------------------------------------------------------------
c Reads and writes orientation of cryst axes x,y given in Miller [s](m).
c      Input:  igrp - point group of crystal symmetry
c     Output: feinv - pull back rotation to slip system: x∥s, z⊥m
c              ainv - the inverse of standard crystallographic matrix
c                     (shifter g^k_K = g_K^k from contravarian cryst.
c                     to orthonormal system), written by Paweł Dłużewski
c-----------------------------------------------------------------------
      implicit none
      include 'iofile.h'
      integer igrp,id(8),i,j
      real*8 r(3,3),a(3,3),ainv(3,3),feinv(3,3),s(3),m(3),x(3),z(3)

c  Trigonal and haxagonal
      if(igrp.gt.20) then
         read(ior,*) (id(i),i=1,8)
        if(id(1)+id(2)+id(3).ne.0.) then
           write(*,10) (id(i),i=1,4)
           stop
        elseif(id(5)+id(6)+id(7).ne.0) then
           write(*,11) (id(i),i=4,8)
           stop
        elseif((id(1)-id(3))*id(5)+(id(2)-id(3))*id(6)+id(4)*id(8).ne.0)
     $     then
           write(*,12) id 
           stop
        elseif((id(1)-id(3))*id(4).lt.0)
     $     then
           write(*,13) id 
           stop
        endif
        write(iow,14) id
        s(1) = id(1)-id(3)
        s(2) = id(2)-id(3)
        s(3) = id(4)
        m(1) = id(5)
        m(2) = id(6)
        m(3) = id(8)
      else
c     the rest
         read(ior,*)   (id(i),i=1,6)
         write(iow,15) (id(i),i=1,6)
        s(1) = id(1)
        s(2) = id(2)
        s(3) = id(3)
        m(1) = id(4)
        m(2) = id(5)
        m(3) = id(6)
        if(id(1)*id(4)+id(2)*id(5)+id(3)*id(6).ne.0) then
           write(*,16) (id(i),i=1,6)
           stop
        endif
      endif
      call read_a(igrp,a)

c     Projection of slip and plane vectors onto orthonormal basis {e_i}
      x = matmul(a,s)   ! see Eq. U_E = A U_A in Bojarski p.60
      call inv3d(a,ainv)
      z = matmul(m,ainv)! covariant components to orthonormal basis 
      call qmill(x,z,r) ! rotation tensor e_1 => x/|x|, e_3 => z/|z|
      call inv3d(r,feinv)

      return
 10   format('ERROR: The 1st 3 Miller indices of x-axis does not vanish
     $[',i3,i3,i3,i3']')
 11   format('ERROR: The 1st 3 Miller indices of z-plane does not vanish
     $(',i3,i3,i3,i3')')
 12   format('ERROR: Input x-axis [',i3,i3,i3,i3,
     $'] is not perpendicular to z plane (',i3,i3,i3,i3,')')
 13   format('ERROR: Input z-axis is upside down z⊥(',i3,i3,i3,i3,
     $') x∥(',i3,i3,i3,i3,') please change the sign of x or z axis')
 14   format('Coordinate system: x ∥ [',i3,i3,i3,i3,'],   z ⊥ (',
     *i3,i3,i3,i3,')') 
 15   format('Coordinate system: x ∥ [',i3,i3,i3,'],   z ⊥ (',
     *i3,i3,i3,')') 
 16   format('ERROR: Input x-axis [',i3,i3,i3,
     $'] is not perpendicular to z-plane (',i3,i3,i3,')')
      end
