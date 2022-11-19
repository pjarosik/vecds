      subroutine read_a(igrp,a0)
c      Input: igrp - number of point group 
c     Inside:    d - crystallographic lenghts&angles: a,b,c,alpha,beta,gamma 
c     Output:   a0 - the standard crystallographic matrix (shifter
c                    g^K_k = g_k^K =  g_kK from contravarian cryst. k to
c                    orthonormal system K, by Paweł Dłużewski
c     Eq. (1.46)&Table 1.3 in Bojarski et al. Krystalografia PWN, 1996

      implicit none

      include 'iofile.h'
      integer igrp,k
      real*8 d(6),a0(3,3),pi2,a,b,c,ca,sa,cb,sb,cg,sg

      d(4) = 90.
      d(5) = 90.
      d(6) = 90.

      if(igrp.eq.1.or.igrp.eq.2) then      ! triclinic symmetry
         read(ior,*) (d(k),k=1,6)

      elseif(igrp.ge.3.and.igrp.le.5) then ! monoclinic 
         read(ior,*) (d(k),k=1,4)
         d(5) = d(4)
          
      elseif(igrp.ge.6.and.igrp.le.8) then ! rombic 
         read(ior,*) (d(k),k=1,2)
         d(4) = d(2)
         d(5) = d(2)
         d(6) = d(2)
         d(2) = d(1)
         d(3) = d(1)

      elseif(igrp.ge.9.and.igrp.le.15) then ! tetragonal 
         read(ior,*) (d(k),k=1,2)
         d(3) = d(2)
         d(2) = d(1)

      elseif((igrp.ge.16.and.igrp.le.20).or.(igrp.eq.0)) then
         read(ior,*) (d(k),k=1,1) ! cubic or isotropic 
         d(2) = d(1)
         d(3) = d(1)

      elseif(igrp.ge.26.and.igrp.le.32) then
         read(ior,*) (d(k),k=1,2) ! hexagonal
         d(3) = d(2)
         d(2) = d(1)
         d(6) = 120.
      else
         stop 'incorrect symmetry group number input, must be 0:32'
      endif

      a = d(1)
      b = d(2)
      c = d(3)
      pi2 = acos(-1.d0)/180.
      ca = dcos(d(4)*pi2)
      cb = dcos(d(5)*pi2)
      cg = dcos(d(6)*pi2)
      sa = dsin(d(4)*pi2)
      sb = dsin(d(5)*pi2)
      sg = dsin(d(6)*pi2)

      a0(1,1) = a
      a0(1,2) = b * cg 
      a0(1,3) = c * cb
      a0(2,1) = 0.
      a0(2,2) = b * sg
      a0(2,3) = c * (ca-cb*cg)/sg
      a0(3,1) = 0.
      a0(3,2) = 0.
      a0(3,3) = c * sqrt(1. + 2.*ca*cb*cg - (ca*ca+cb*cb+cg*cg))/sg

      end
      
