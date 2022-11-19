        subroutine stiff2crys(igrp,cc,a)
 
c       written by Paweł Dłużewski, http://www.ippt.pan.pl/~pdluzew  
c-----------------------------------------------------------------------
c      Generates the 2rd-order stiffness tensor
c      Input: 
c       igrp - point group number of crystal symmetry
c          a - standard crystallographic matrix projecting from covariant local crystallogrgraphic coord. to lab. system
c          c - elastic moduli read inside, required sequence given in the output write format, e.g. see 1030 for igrp = 30  
c      Output:
c         cc - 3rd order stiffness tensor in the laboratory (global orthonormal coordinate system)
c
c  Point groups, according to Int. Tables X-Ray Crystallography, Kynoch Press
c                   rewritten by Bojarski,et al. Krystalografia str. 136
c
c  Calasification of elastic properties according Table 5.2 from [1] revised by Pawel Dluzewski 
c  [1] Teodosiu (1982), Elastic models of crystal defects, Editura Academiei, Bucursti & Springer-Verlag, Berlin
c igrp:
c  0 -isotropy
c  1 -triclinic,1     2 -triclinic,-1      
c  3 -monoclinic,m    4 -monoclinic,2     5 -monoclinic,2/m    
c  6 -rhombic,mm2     7 -rhombic,222      8 -rhombic,2/m2/m2/m 
c  9 -tetragonal,4   10 -tetragonal,-4   11 -tetragonal,4/m           12 -tetragonal,-42/m 13 -tetragonal,4mm
c 14 -tetragonal,422 15 -tetragonal,4/m2/m/2m 
c 16 -cubic,23       17 -cubic,2/m-3     18 -cubic,-43m (A4,sfaleryt) 19 -cubic,432        20 -cubic,4/m-32/m(A1,A2,B1,B2)
c 21 -trigonal,3     22 -trigonal,-3     23 -trigonal,32              24 -trigonal,3m      25 -trigonal,-32/m
c 26 -hexagonal,6    27 -hexagonal,-6    28 -hexagonal,6/m            29 -hexagonal,-6m2   30 -hexagonal,6mm(A3,A9,wurcyt)
c 31 -hexagonal,622  32 -hexagonal,6/m2/m2/m
c-----------------------------------------------------------------------
      implicit none    
      include 'iofile.h' 

      integer i,j,k,k1,l,m,n,igrp,isym(56,0:11)
      real*8 a(3,3),b(6,6),c(56),cc(6,6,6),c0(6,6,6)
      data isym/
c 0: crystal symmetry group (isotropy for tests) 
     $ 1,2,2,0,0,0,2,3,0,0,  0,2,0,0,0,4,0,0,5,0,  5,1,2,0,0,0,2,0,0,0,
     $ 5,0,0,4,0,5,1,0,0,0,  5,0,0,5,0,4,0,0,0,0,  6,0,0,0,0,0,

c 1: crystal symmetry group (1, -1) triclinic 
     $ 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11,12,13,14,15,16,17,18,19,20,
     $21,22,23,24,25,26,27,28,29,30, 31,32,33,34,35,36,37,38,39,40,
     $41,42,43,44,45,46,47,48,49,50, 51,52,53,54,55,56,

c 2: crystal symmetry group (m, 2, 2/m) monoclinic 
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

c 3: crystal symmetry group ( mm2 ,222, 2/m2/m2/m ) rhombic 
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      
c 4: crystal symmetry group ( 4, -4, 4/m ) tetragonal
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

c 5: crystal symmetry group ( -42/m, 4mm, 422, 4/m2/m/2m ) tetragonal
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

c 6: crystal symmetry group ( 23, 2/m-3 ) cubic
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

c 7: crystal symmetry group (-43m, 432, 4/m-32/m ) cubic
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 1, 2, 2, 0, 0, 0, 2, 3, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 5, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $ 5, 1, 2, 0, 0, 0, 2, 0, 0, 0, 5, 0, 0, 4, 0, 5, 1, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $ 5, 0, 0, 0, 0, 4, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0,

c 8: crystal symmetry group ( 3, -3 ) trigonal
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

c 9: crystal symmetry group (32 , 3m, -32/m) trigonal
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

c10: crystal symmetry group ( 6, -6, 6/m ) hexagonal
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 1, 4, 5, 0, 0, 6,13, 7, 0, 0, 6, 5, 0, 0, 0, 9,11, 0,10, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $14, 2, 5, 0, 0, 6, 8, 0, 0, 0,10,11, 0, 9, 0,15, 3, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $12, 0, 0,12, 0,16, 0, 0,11, 0,17, 0, 0,18, 0,19,

c11: crystal symmetry group (-6m2, 6mm, 622, 6/m2/m2/m) hexagonal
c      1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,
     $ 1, 4, 5, 0, 0, 0,11, 6, 0, 0, 0, 5, 0, 0, 0, 8, 0, 0, 9, 0,
c     21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
     $12, 2, 5, 0, 0, 0, 7, 0, 0, 0, 9, 0, 0, 8, 0,13, 3, 0, 0, 0,
c     41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,
     $10, 0, 0,10, 0,14, 0, 0, 0, 0,15, 0, 0, 0, 0, 0/

      c = 0.d0
      write(iow,'(/"The 3rd order Elastic Constants")') 
      if(igrp.eq.0) then 
         read(ior,*) (c(i),i=1,3)
         write(iow,'("isotropic case, subsystem 0"/
     $ " 111 =",es10.3,", 112 =",es10.3,", 113 =",es10.3)') (c(i),i=1,3)
         c(4) = (c(2) - c(3)) / 2.d0
         c(5) = (c(1) - c(2)) / 4.d0
         c(6) = (c(1) - 3.d0*c(2) +2.d0*c(3)) / 8.d0 
         k = 6              ! number of mutually different coeffiecients
         m = 0
      elseif(igrp.eq.1) then
         read(ior,*)  (c(i),i=1,56)
         write(iow,1) (c(i),i=1,56) 
 1       format('triclinic crystal symmetry, subsystem 1'/
     $           (8(a5,es10.3)/))
         k = 56
         m = 1
      elseif(igrp.ge.18.and.igrp.le.20)then  !  cubic (-43m, 432, 4/m-32/m ) e.g. B3
 7       format('cubic crystal symmetry, subsystem 7'/ 
     *        ' 111 =',es10.3,', 112 =',es10.3,', 123 =',es10.3,
     *       ', 144 =',es10.3,', 155 =',es10.3,', 456 =',es10.3)
         read(ior,*)  (c(i),i=1,6) 
         write(iow,7) (c(i),i=1,6) 
         k = 6
         m = 7
      elseif(igrp.ge.26.and.igrp.le.28)then  ! hexagonal ( 6, -6, 6/m )
 10      format('hexagonal crystal symmetry, subsystem 10'/ 
     *         ' 111 =',es10.3,', 222 =',es10.3,', 333 =',es10.3,
     $        ', 112 =',es10.3,', 113 =',es10.3/', 116 =',es10.3,
     $        ', 123 =',es10.3,', 133 =',es10.3,', 144 =',es10.3,
     $        ', 155 =',es10.3,', 145 =',es10.3,', 344 =',es10.3)
         read(ior,*)   (c(i),i=1,12)
         write(iow,10) (c(i),i=1,12)
         c(13) = (     c(1) + c(4) -      c(2))
         c(14) =-(2.d0*c(1) + c(4) - 3.d0*c(2)) / 4.d0
         c(15) = (2.d0*c(1) - c(4) -      c(2)) / 4.d0
         c(16) =      (c( 5) - c(7)) / 2.d0
         c(17) =      (c(10) - c(9)) / 2.d0
         c(18) = -c(11)
         c(19) = -c( 6)
         k = 19
         m = 10
      elseif(igrp.ge.29.and.igrp.le.32) then ! hexagonal (-6m2, 6mm, 622, 6/m2/m2/m)
 11      format('hexagonal crystal symmetry, subsystem 11'/
     *         ' 111 =',es10.3,', 222 =',es10.3,', 333 =',es10.3,
     $        ', 112 =',es10.3,', 113 =',es10.3/ ' 123 =',es10.3,
     $        ', 133 =',es10.3,', 144 =',es10.3,', 155 =',es10.3,
     $        ', 344 =',es10.3)
         read(ior,*)   (c(i),i=1,10)
         write(iow,11) (c(i),i=1,10)         
         c(11) = (     c(1) + c(4) -      c(2))
         c(12) =-(2.d0*c(1) + c(4) - 3.d0*c(2)) / 4.d0
         c(13) = (2.d0*c(1) - c(4) -      c(2)) / 4.d0
         c(14) =      (c(5) - c(6)) / 2.d0
         c(15) =      (c(9) - c(8)) / 2.d0
         k = 15
         m = 11
      else
      stop 'ERROR: Point symmetry group exceeds 0-32 or it
     $has not been programmed yet, consult stiff2crys.f' 
      endif

c-----------------------------------------------------------------------
      l = 1
      do i = 1,6
         do j = i,6
            do k = j,6
               cc(i,j,k) = c(isym(l,m))
               cc(j,i,k) = c(isym(l,m))
               cc(j,k,i) = c(isym(l,m))
               cc(i,k,j) = c(isym(l,m))
               cc(k,i,j) = c(isym(l,m))
               cc(k,j,i) = c(isym(l,m))
               l = l+1
            enddo
         enddo
      enddo
c-----------------------------------------------------------------------
      write(iow,*) 'in the main crystallographic axes:' 
      write(iow,12) (((k,j,i,cc(k,j,i),i=j,6),j=k,6),k=1,6)
      call sym33to66(a,b)
      c0 = 0.
      do i = 1,6
         do j = 1,6
            do k = 1,6       
               do l = 1,6
                  do m = 1,6
                     do n = 1,6
                  c0(i,j,k) = c0(i,j,k) + b(i,l)*b(j,m)*b(k,n)*cc(l,m,n)
                     enddo
                  enddo
               enddo
            enddo
         enddo
      enddo
      cc = c0
      write(iow,*)'in the global coordinate system:'
      write(iow,12) (((k,j,i,cc(k,j,i),i=j,6),j=k,6),k=1,6)
 12   format(6(" ",3i1,"=",es10.3,","))
      return
      end
