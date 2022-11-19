      subroutine divp1c(igrp,el,elch,nch,piez,elel)

      implicit none ! Reads piezoelectric constants and rotates them to
                    ! the global coordinate system of finite elements 
      include 'iofile.h'
      integer igrp,nch,i,j,k,nm
      parameter (nm=2)
      real*8 d(20),el(3),elch(3,nch),piez(3,6),elel(3,3),psp0(3,0:nm),
     $      el0(3,0:nm),piez0(3,6,0:nm),elel0(0:3,0:3,0:nm),elelinv(3,3)

      psp0 = 0.
      piez0 = 0.
      elel0 = 0.
      if(nch.gt.nm) then
         stop 
     $   "Please enlagre the parameter nm to be .ge. nch in divp1c"
      endif 
      do k = 0,nch
         if(igrp.eq.30) then
            read(ior,*) (d(i),i=1,5)
            psp0(3,k) = d(1)
            piez0(3,1,k) = d(2)
            piez0(3,2,k) = d(2)
            piez0(3,3,k) = d(3)
            piez0(2,4,k) = d(4)
            piez0(1,5,k) = d(4)
            elel0(1,1,k) = d(5)
            elel0(2,2,k) = d(5)
            elel0(3,3,k) = d(5)
         else
            write(iow,10) igrp
 10         format('Crystal symmetry group igpr=',i2,
     $                                  ' has not been programmed yet' )
            stop 'See subroutine piezoc'
         endif
      enddo

      call inv3d(elel0(1,1,0),elelinv)

      do k = 0,nch
         do i = 1,3
            el0(i,k) = 0.
            do j = 1,3   ! multiplied by referece dielectric tensor
               el0(i,k) = el0(i,k) - elelinv(i,j) * psp0(j,k) 
            enddo                                               
         enddo
      enddo

      do i = 1,3
         el(i) = el0(i,0)
         do j = 1,3
            elel(i,j) = elel0(i,j,0)
         enddo
         do j = 1,nch
            elch(i,j) = el0(i,j)-el0(i,0)
         enddo
         do j = 1,6   
            piez(i,j) = piez0(i,j,0)
         enddo
      enddo
      write(iow,1000) 
      write(iow,1025)piez0(3,1,0),piez0(3,3,0),piez0(1,5,0)
      write(iow,1100)((piez(i,j),j=1,6),i=1,3)
      write(iow,2000) psp0(1,0),psp0(3,0)
      write(iow,3000) elel0(1,1,0),elel0(3,3,0)
      return

 1000 format('Piezoelectric constants in crystallographic axes:')
 1025 format('for hexagonal 6mm group'/
     *  ' e13=',e10.3,1x,',  e33 =',e10.3,1x,',  e53 =',e10.3)

 1100 format('and in the global coordiante system x,y,z'/
     *' e_11=',e10.3,1x,' e_12=',e10.3,1x,' e_13=',e10.3,1x,
     *' e_14=',e10.3,1x,' e_15=',e10.3,1x,' e_16=',e10.3,1x,/
     *' e_21=',e10.3,1x,' e_22=',e10.3,1x,' e_23=',e10.3,1x,
     *' e_24=',e10.3,1x,' e_25=',e10.3,1x,' e_26=',e10.3,1x,/
     *' e_31=',e10.3,1x,' e_32=',e10.3,1x,' e_33=',e10.3,1x,
     *' e_34=',e10.3,1x,' e_35=',e10.3,1x,' e_36=',e10.3,1x)
 2000 format('Spontanenous polarisation in crystallographic axes:'
     *  ' Psp_1=',e10.3,1x,',  Psp_3 =',e10.3,1x)
 3000 format('Dielectric constants in crystallographic axes:'
     *  ' d_11=',e10.3,1x,',  d_33 =',e10.3,1x)
      end 
