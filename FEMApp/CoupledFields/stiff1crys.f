        subroutine stiff1crys(igrp,cc,a)
 
c       written by Pawel Dluzewski, http://www.ippt.gov.pl/~pdluzew  
c-----------------------------------------------------------------------
c      Generates the 1st-order stiffness tensor
c      Input:  igrp - point group number of crystal symmetry
c                 a - strandard crystallographic matrix
c                 c - elastic moduli (read inside the subroutine) 
c      Output:   cc - stiffness tensor in the laboratory coord. system
c
c  Point groups, according to    Int. Tables X-Ray Crystallography, Kynoch Press
c                   rewritten by Bojarski,et al. Krystalografia str. 136
c
c  Classification of elastic properties based on Table 5.2 in Teodosiu (1982),
c  Elastic models of crystal defects, Editura Acad., Bucursti & Springer-Vlg.
c                                                                   
c igrp:
c  0 -isotropy
c  1 -triclinic,1       2 -triclinic,-1      
c  3 -monoclinic,m      4 -monoclinic,2                 5 -monoclinic,2/m    
c  6 -rhombic,mm2       7 -rhombic,222                  8 -rhombic,2/m2/m2/m 
c  9 -tetragonal,4     10 -tetragonal,-4               11 -tetragonal,4/m
c 12 -tetragonal,-42/m 13 -tetragonal,4mm              14 -tetragonal,422
c 15 -tetragonal,4/m2/m/2m 
c 16 -cubic,23         17 -cubic,2/m-3                 18 -cubic,-43m (A4,sfaleryt)
c 19 -cubic,432        20 -cubic,4/m-32/m(A1,A2,B1,B2)   
c 21 -trygonal,3       22 -trygonal,-3                 23 -trygonal,32
c 24 -trygonal,3m      25 -trygonal,-32/m
c 26 -hexagonal,6      27 -hexagonal,-6                28 -hexagonal,6/m
c 29 -hexagonal,-6m2   30 -hexagonal,6mm(A3,A9,wurcyt) 31 -hexagonal,622
c 32 -hexagonal,6/m2/m2/m
c-----------------------------------------------------------------------
      implicit none    
      include 'iofile.h' 

      integer i,j,k,igrp,isym(6,6,6),icgrp(0:32),icsym(0:32)
      real*8 a(3,3),b(6,6),c(0:21),cc(6,6)
      data icsym/2,21,21,13,13,13,9,9,9,7,7,7,6,6,6,6,3,3,3,3,3,7,7,6,6,
     *     6,5,5,5,5,5,5,5/,icgrp/5,1,1,2,2,2,3,3,3,4,4,4,4,4,4,4,5,5,5,
     $     5,5,6,6,6,6,6,6,6,6,6,6,6,6/,isym/
c 1: subsystem (1) triclinic 
c c11,c12,c13,c14,c15,c16,c22,c23,c24,c25,c26,c33,c34,c35,c36,c44,c45,c46,c55,c56,c66seeTeodosiu
     $  1,  2,  3,  4,  5,  6,  
     $  2,  7,  8,  9, 10, 11,
     $  3,  8, 12, 13, 14, 15,
     $  4,  9, 13, 16, 17, 18,
     $  5, 10, 14, 14, 19, 20,
     $  6, 11, 15, 15, 20, 21,

c 2: subsystem(2) monoclinic 
c      c11,c12,c13,c16,c22,c23,c26,c33,c36,c44,c45,c55,c66
c       1   2   3   4   5   6   7   8   9  10  11  12  13
     $  1,  2,  3,  0,  0,  4, ! c11 c12 c13  0   0  c16 
     $  2,  5,  6,  0,  0,  7, !     c22 c23  0   0  c26
     $  3,  6,  8,  0,  0,  9, !         c33  0   0  c36
     $  0,  0,  0, 10, 11,  0, !             c44 c45  0 
     $  0,  0,  0, 11, 12,  0, !                 c55  0 
     $  4,  7,  9,  0,  0, 13, !                     c66

c 3: subsystem(3) rhombic c11,c12,c13,c22,c23,c33,c44,c55,c66
c                          1   2   3   4   5   6   7   8   9
     $  1,  2,  3,  0,  0,  0, ! c11 c12 c13  0   0   0 
     $  2,  4,  5,  0,  0,  0, !     c22 c23  0   0   0 
     $  3,  5,  6,  0,  0,  0, !         c33  0   0   0 
     $  0,  0,  0,  7,  0,  0, !             c44  0   0 
     $  0,  0,  0,  0,  8,  0, !                 c55  0 
     $  0,  0,  0,  0,  0,  9, !                     c66

c 4: subsystems(4,5) tetragonal c11,c12,c13,c33,c44,c66,c16| ,-c16
c                                1   2   3   4   5   6   7      8
     $  1,  2,  3,  0,  0,  7, ! c11 c12 c13  0   0  c16
     $  2,  1,  3,  0,  0,  8, !     c11 c13  0   0 -c16
     $  3,  3,  4,  0,  0,  0, !         c33  0   0   0 
     $  0,  0,  0,  5,  0,  0, !             c44  0   0 
     $  0,  0,  0,  0,  5,  0, !                 c44  0 
     $  7,  8,  0,  0,  0,  6, !                     c66

c 5: subsystems(0,6,7) cubic (including isotropic=0 )
c     for cubic reads                  c11,       c12,c44   
c     for isotropic reads c12,c44 and next generates c11=c12+2c44,c12,c44 
c                          1   2          =>            1          2   3
     $  1,  2,  2,  0,  0,  0, ! c11 c12 c12  0   0   0 
     $  2,  1,  2,  0,  0,  0, !     c11 c12  0   0   0 
     $  2,  2,  1,  0,  0,  0, !         c11  0   0   0 
     $  0,  0,  0,  3,  0,  0, !             c44  0   0 
     $  0,  0,  0,  0,  3,  0, !                 c44  0 
     $  0,  0,  0,  0,  0,  3, !                     c44

c 6: subsystems (8,9,10,11) trigonal (including hexagonal) 
c         c11,c12,c13,c33,c44,c14,c15,| c66=1/2(c11-c12),-c14,-c15
c          1   2   3   4   5   6   7       8               9   10
     $  1,  2,  3,  6,  7,  0, ! c11 c12 c13 c14 c15  0 
     $  2,  1,  3,  9, 10,  0, !     c11 c13-c14-c15  0 
     $  3,  3,  4,  0,  0,  0, !         c33  0   0   0 
     $  6,  9,  0,  5,  0, 10, !             c44  0 -c15 
     $  7, 10,  0,  0,  5,  6, !                 c44 c14
     $  0,  0,  0,  0,  6,  8/ !                  (c11-c12)/2
 

      c = 0.
      k  = icsym(igrp)
      read(ior,*) (c(i),i=1,k)

      if(igrp.eq.0) then
         c(3) = c(2)
         c(2) = c(1) 
         c(1) = c(2) + 2.d0*c(3)
      else 
         go to (12,12, 12,12,12, 12,12,12, 4,4,4, 12,12,12,12, 
     *        12,12,12,12,12, 8,8, 9,9,9, 10,10,10, 10,10,10,10)igrp
 4       c(8)= -c(7)  
         go to 12
 8       c(10) = -c(7)
 9       c(9)  = -c(6)
 10      c(8)  = 0.5 * (c(1)-c(2))
 12      continue
      endif

      k = icgrp(igrp)
      do i = 1,6
         do j = 1,6
            cc(i,j) = c(isym(i,j,k))
         enddo
      enddo

      if(igrp.eq.0) then
         write(iow,1020) (c(i),i=1,2)
      else
         go to(21,21, 22,22,22, 23,23,23, 24,24,24, 25,25,25,25,
     $        26,26,26,26,26, 27,27, 28,28,28, 29,29,29,29,29,29,29)igrp
 21      write(iow,1021) (c(i),i=1,21)
         go to 30
 22      write(iow,1022) (c(i),i=1,13)
         go to 30
 23      write(iow,1023) (c(i),i=1,9)
         go to 30
 24      write(iow,1024) (c(i),i=1,7)
         go to 30
 25      write(iow,1025) (c(i),i=1,6)
         go to 30
 26      write(iow,1026) (c(i),i=1,3)
         go to 30
 27      write(iow,1027) (c(i),i=1,7)
         go to 30
 28      write(iow,1028) (c(i),i=1,6)
         go to 30
 29      write(iow,1029) (c(i),i=1,5)
 30      continue
      endif
      write(iow,1010) ((cc(i,j),j=1,6),i=1,6)
      call sym33to66(a,b)
      cc = matmul(b,cc)
      b = transpose(b)
      cc = matmul(cc,b)
      write(iow,1015) ((cc(i,j),j=1,6),i=1,6)

 1010 format('in the main crystallographic axes:'/
     $     (6es11.4,1x))
 1015 format('and in the global coordinate system:'/(6es11.4,1x))
 1020 format(/'Lame constants λ =',es11.4,' μ',es11.4//)     
 1021 format(/'Elastic moduli for triclinic system (subsystem 2)'/ 
     *  ' c14 =',es11.4,',  c12 =',es11.4,',  c13 =',es11.4,
     *',  c22 =',es11.4,',  c23 =',es11.4,',  c33 =',es11.4,
     *',  c44 =',es11.4,',  c55 =',es11.4,',  c66 =',es11.4,
     *',  cxx =',es11.4,',  cxx =',es11.4,',  cxx =',es11.4,
     *',  cxx =',es11.4,',  cxx =',es11.4,',  cxx =',es11.4,
     *',  cxx =',es11.4,',  cxx =',es11.4,',  cxx =',es11.4,
     *',  cxx =',es11.4,',  cxx =',es11.4,',  cxx =',es11.4)
 1022 format(/'Elastic moduli for monoclinic system (subsystem 2)'/
     *  ' c14 =',es11.4,',  c12 =',es11.4,',  c13 =',es11.4,
     *',  c22 =',es11.4,',  c23 =',es11.4,',  c33 =',es11.4,
     *',  c44 =',es11.4,',  c55 =',es11.4,',  c66 =',es11.4,
     *',  cxx =',es11.4,',  cxx =',es11.4,',  cxx =',es11.4,
     *',  cxx =',es11.4)
 1023 format(/'Elastic moduli for rhombic system (subsystem 3)'/
     *  ' c14 =',es11.4,',  c12 =',es11.4,',  c13 =',es11.4,
     *',  c22 =',es11.4,',  c23 =',es11.4,',  c33 =',es11.4,
     *',  c44 =',es11.4,',  c55 =',es11.4,',  c66 =',es11.4)
 1024 format(/'Elastic moduli for tetragonal system (subsystem 4)'/
     *  ' c11 =',es11.4,',  c12 =',es11.4,',  c13 =',es11.4,
     *',  c33 =',es11.4,',  c44 =',es11.4,',  c66 =',es11.4,
     *',  c16 =',es11.4)
 1025 format(/'Elastic moduli for tetragonal system (subsystem 5)'/
     *  ' c11 =',es11.4,',  c12 =',es11.4,',  c13 =',es11.4,
     *',  c33 =',es11.4,',  c44 =',es11.4,',  c66 =',es11.4)
 1026 format(/'Elastic moduli for cubic system (subsystems 6,7)'/
     *  ' c11 =',es11.4,',  c12 =',es11.4,',  c44 =',es11.4)
 1027 format(/'Elastic moduli for trigonal system (subsystem 8)'/
     *  ' c11 =',es11.4,',  c12 =',es11.4,',  c13 =',es11.4,
     *',  c33 =',es11.4,',  c44 =',es11.4,',  c14 =',es11.4,
     *',  c15 =',es11.4)
 1028 format(/'Elastic moduli for trigonal system (subsystem 9)'/
     *  ' c11 =',es11.4,',  c12 =',es11.4,',  c13 =',es11.4,
     *',  c33 =',es11.4,',  c44 =',es11.4,',  c14 =',es11.4)
 1029 format(/'Elastic moduli for hexagonal system (subsystems 10,11)'/
     *  ' c11 =',es11.4,',  c12 =',es11.4,',  c13 =',es11.4,
     *',  c33 =',es11.4,',  c44 =',es11.4)
       return
       end
