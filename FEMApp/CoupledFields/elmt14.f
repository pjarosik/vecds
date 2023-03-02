      subroutine elmt14(d,ul,xl,ix,tl,s,p,ndf,ndm,nst,isw)
c      ul - nodal displacements           nel - number of nodes
c      xl - global coordinates of nodes   ndm - space dimension
c     ndf - number of freedom degrees    lint - number of Gauss points

c      3D Anisotropic Hyperelastic Element for calculation of residual s
c                 Computational Materials Science Group
c                       http://cmsg.ippt.gov.pl
c      Constitutive model:  P.Dluzewski, J.Elasticity 60 (2000) 119-129.

      use material_parameters
      implicit none

      include 'bdata.h'
      include 'cdata.h'
      include 'eldata.h'
      include 'iofile.h'
      include 'prstrs.h'
      include 'ptdat6.h'
      include 'strnum.h'
      include 'eltran.h'
      logical nvpf(20)
      character*8 prob
      character*10 title(10)
      integer i,j,k,l,q,ndf,ndm,nst,lint,isw,nvp,ix(*),ixx,igrp,kinem,
     $     cnstv,
     $     isy,isx,
     $     idx,idy,nb,
     $     ichx,ichy,nch,
     $     ipx,ipy
      real*8 dv,xl(ndm,*),xu(3,27),tl(*),d(*),ul(ndf,nen,*),xsj,det3d,
     $       sr(27,0:9),sr1(8,0:3),sl(27,0:9),sl1(8,0:3),rho,cfac,
     $       s(nst,nst),p(nst),sg(4,27), eb(6,0:3,9),fb(9,9),
     $       sig(6,0:6,0:6),sigp(10),fl(3,3,0:3),flinv(3,3),f(3,3,0:3),
     $       fchinv(3,3),alpha0(3,3),alpha(3,3),detfl,a0(3,3),
     $       dch(6,7),chch(7,7),chchch(7,7,7),x(0:3,7),v(3,7),energy,
     $       pol(3),del(6,3),poch(3,7),popo(3,3),seth
      save isx,isy,idx,idy,ichx,ichy,nch,ipx,ipy,fb,nb,seth,rho,cfac
      data isx/0/,isy/0/,nvpf/20*.false./,
     $     idx/0/,idy/0/,
     $     ipx/0/,ipy/0/,
     $     ichx/0/,ichy/0/,nch/0/,
     $     title/' Sigma_11 ',' Sigma_22 ',' Sigma_33 ',
     $           ' Sigma_32 ',' Sigma_21 ',' Sigma_12 ',
     $           '          ','          ','          ',' Det(Fl)  '/
c     Material data
      if(isw.eq.1) then
         read(ior,*) d(1),d(2),nvp,nch,igrp,rho! nvp - number of variables/problems, rho  - mass or molar density in ref. conf.      
         l    = d(1) ! Gauss points            !  nch  - number of chemical components  cfac - consistent/lumpen projection factor      
         k    = d(2) ! G.points for stresses   !  igrp - crystal symmetry point group  
         call sxmyq(igrp,flinv,a0)
         do i = 1,nvp
            read(ior,*)prob,kinem,cnstv,ixx
            if(ixx.eq.0) then
               write(iow,2005) prob,kinem,cnstv,ixx ! odwrócić kolejność
               stop
            elseif(prob.eq.'div(x_n)'.and.iabs(ixx)-1+nch.le.ndf) then
               call readxy(ichx,ichy,ixx)
               if(isx.ne.0) call vegard(igrp,nch,a0,ech0(1,1),seth)
               do j = 1,nch
                  nvpf(iabs(ixx)-1+j) = .true.
               enddo
            elseif(prob.eq.'div(sig)'.and.iabs(ixx)-1+ndm.le.ndf) then
               call readxy(isx, isy ,ixx)!kinem= 1 Eulerian small strain
               if(kinem.eq.-2)then       !      -1 Lagrangian small str.
                  read(ior,*)seth        !      -2 Lagrangian Seth-Hill
               else                      !       2 Eulerian Seth-Hill
                  write(iow,*)'ERROR: strain',kinem,'not programmed yet'
                  stop
               endif
               do j = 0,nch
                  if(cnstv.eq.1) then
                     call stiff1crys(igrp,c0(1,1,0,j),flinv)
                  elseif(cnstv.eq.2) then
                     call stiff1crys(igrp,c0(1,1,0,j),flinv)
                     call stiff2crys(igrp,c0(1,1,1,j),flinv)
                  else
                  write(iow,*)'ERROR: elastic constitutive model cnstv='
     $                    ,cnstv,'not programmed yet'
                  stop
                  endif
               enddo
               if(nch.ne.0.and.ichx.ne.0) then
                  write(iow,2004)
                  stop
               endif
               do j = 1,ndm
                  nvpf(iabs(ixx)-1+j) = .true.
               enddo
            elseif(prob.eq.'cur(bet)'.and.iabs(ixx)-1+nb.le.ndf) then
               call readxy(idx, idy ,ixx) ! dislocations
               read(ior,*) rde0,nb
               do j = 1,nb
               read(ior,*) (fb(k,j),k=1,9)
               nvpf(iabs(ixx)-1+j) = .true.
               enddo
            elseif (prob.eq.'div(pol)'.and.iabs(ixx).le.ndf) then
               if(ipx.ne.0) then
                  write(iow,*)'ERROR:multipled electric equilibrium Eq.'
                  stop
               endif
               call readxy(ipx, ipy ,ixx)
               call divp1c(igrp,el0(1),elch0(1,1),nch,
     $                                            piez0(1,1),elel0(1,1))
               nvpf(iabs(ixx)) = .true.
            else
               write(iow,2005) prob,ixx
               stop
            endif
c           call material(prob,kinem,cnstv,igrp,nch,iow)
         enddo
         do j = 1,ndf
            if(.not.nvpf(j)) then
              write(*,2006)j
              stop
            endif
         enddo

         write(iow,2000) int(d(1)),int(d(2))
         lint = 0
         mct = 4

c        Set plot sequence for 8/27-node bricks
         if(nen.eq.8) then
            call plbrk8(iel)
         elseif(nen.eq.27) then
            call plbk27(iel)
c         elseif(nen.eq.64) then
c            call plbk64(iel)
         endif
      else
         l  = int(d(1))
         call int3d(l,lint,sg) ! p-kty Gaussa
         do i = 1,ndm
            do j = 1,nel
               xu(i,j) = xl(i,j) + ul(i,j,1)
            enddo
         enddo
         do l = 1,lint
c     Shape function in the current configuration
            call shape3d(sg(1,l),xsj,sr1(1,0),xu,8)
            call shape3d(sg(1,l),xsj,sr(1,0),xu,nel)
            do j = 1,nel
               if(isw.eq.3) then !if(ix(j).eq.3137) then
                  continue
               endif
            enddo

c     Thermodynamic variables
            if(ichx.ne.0) then  ! chemical var.
               call chemxi(sr1(1,0),ul(ichx,1,1),x(0,1),nch,ndf,ndm,8)
            else
               x = 0.
            endif
            if(isx.ne.0) then
               if(idx.ne.0) then
                  call  disl(sr(1,1),nel,sr1(1,0),8,ul(isx,1,1),ndf,
     $                     ul(idx,1,1),nb,fb(1,1),fl,f,eb)
                  call inv3d(fl(1,1,0),flinv)
                  detfl = det3d(fl(1,1,0))
                  do i = 1,3
                     alpha(i,1) = fl(i,2,3) - fl(i,3,2)
                     alpha(i,2) = fl(i,3,1) - fl(i,1,3)
                     alpha(i,3) = fl(i,1,2) - fl(i,2,1)
                  enddo
                  do i = 1,3
                     do j = 1,3
                        alpha0(i,j) = 0.
                        do k = 1,3
                           do q = 1,3
                              alpha0(i,j) = alpha0(i,j)
     $                             + flinv(i,k)*alpha(k,q)*flinv(j,q)
                           enddo
                        enddo
                        alpha0(i,j) = alpha0(i,j) * detfl
                     enddo
                  enddo
               else
                  call elas(sr(1,1),ul(isx,1,1),fl,ndf,nel)
                  alpha0 = 0.d0
               endif
               call strain3d(fl(1,1,0),sig,seth)
            else
               sig = 0.
            endif

            if(ipx.ne.0) then ! elactric variables
               call elect(sr(1,0),ul(ipx,1,1),pol,ndf,ndm,nel)
            else
               pol = 0.
            endif

            dv = xsj * sg(4,l) / det3d(fl(1,1,0))
            do i = 0,ndm
               do j = 1,8
                  sl1(j,i) = sr1(j,i)*dv
               enddo
            enddo
            do i = 0,ndm*ndm
               do j = 1,nel
                  sl(j,i) = sr(j,i)*dv
               enddo
            enddo

c           Constitutive equations (Kirhhoff stress)
            epl(8) = epl(8) + energy(sig,alpha0,x(0,1),nch,pol,dch(1,1)
     $                   ,del,chch(1,1),poch(1,1),popo,chchch(1,1,1))*dv

            if(isw.eq.6) then
               if(isy .gt.0) call divsig(sl(1,1),nel,sig(1,0,0),p(isy)
     $                                                             ,ndf)
               if(ichy.gt.0) then
                  call constx(v(1,1),x(0,1),nch,dch(1,1),chch(1,1),
     $                                          chchch(1,1,1),fl(1,1,1))
                  do k = 1,nch
                     call divxv(sl1(1,1),8,x(0,k),v(1,k),p(ichy+k-1),ndf
     $                                                             ,ndm)
                  enddo
               endif
               if(ipy.gt.0) call divpol(sl(1,1),nel,pol,p(ipy),ndf)
            elseif(isw.eq.3) then
               if(isy.gt.0) then
                  call divsig(sl(1,1),nel,sig(1,0,0),p(isy),ndf)
                  if(isx.ne.0) call divsigeps(sl(1,1),nel,sr(1,1),nel
     $                                   ,sig(1,0,0),s(isy,isx),ndf,nst)
                  if(idx.ne.0) then
                     do k = 1,nb
                        call divsigbet(sl(1,1),nel,sr1(1,0),8
     $                 ,eb(1,0,k),sig(1,1,0),s(isy,idx+k-1),ndf,nst)
                     enddo
                  endif
                  if(ichx.ne.0) then
                     do k = 1,nch
                        call divsigx(sl(1,1),nel,sr1(1,0),8,dch(1,k),
     $                     s(isy,ichx+k-1),ndf,nst) ! wymagana sekewncja
                     enddo
                  endif
                  if(ipx.ne.0) call divsigel(sl(1,1),nel,sr1(1,1),8
     $                                     ,del(1,1),s(isy,ipx),ndf,nst)
               endif
               if(ichy.gt.0) then
                  call constx(v,x(0,1),nch,dch(1,1),chch(1,1),
     $                                          chchch(1,1,1),fl(1,1,1))
                  do k = 1,nch
                     call divxv(sl1(1,1),8,x(0,k),v(1,k),p(ichy+k-1),ndf
     $                                                             ,ndm)
                     if(isx.ne.0) call divxveps(sl1(1,1),8,sr(1,1),
     $                           nel,x(0,k),dch(1,k),f(1,1,0),fl(1,1,1),
     $                                  s(ichy+k-1,isx),ndf,nst)
                     if(idx.ne.0) then
                        do i = 1,nb
                           call divxvbet(sl1(1,0),8,sr(1,0),8,x(0,k),
     $                   dch(1,k),eb(1,0,i),s(ichy+k-1,idx+i-1),ndf,nst)
                        enddo
                     endif
                     call conlump(sl1(1,0),8,sr1(1,0),8,p(ichy+k-1),
     $                s(ichy+k-1,ichx+k-1),ul(ichx+k-1,1,4),ndf,nst)
                     call divxvx(sl1(1,0),8,sr1(1,0),8,x(0,k),x(0,1),nch
     $                ,v(1,k),chch(nch*k,1),chchch(nch*nch*k,1,1),
     $                                         s(ichy+k-1,ichx),ndf,nst)
                  enddo
               endif
               if(ipy.gt.0) then
                  call divpol(sl(1,1),nel,pol,p(ipy),ndf)
                  if(isx.ne.0) call divpoleps(sl(1,1),nel,sr(1,1),nel
     $                                          ,del,s(ipy,isx),ndf,nst)
                  if(idx.ne.0) then
                     do k = 1,nb
                        call divpolbet(sl(1,1),nel,sr1(1,0),8,eb(1,0,k),
     $                                  del(1,1),s(ipy,idx+k-1),ndf,nst)
                     enddo
                  endif
                  if(ichx.ne.0) then
                     do k = 1,nch
                        call divpolx(sl(1,1),nel,sr1(1,0),8,poch(1,k),
     $                         s(ipy,ichx),ndf,nst) ! wymagana sekewncja
                     enddo
                  endif
                  if(ipx.ne.0) call divpolel(sl(1,1),nel,sr(1,1),nel
     $                                         ,popo,s(ipy,ipx),ndf,nst)
               endif
c              call checksym(s(1,1),nst,ndf)
            elseif( isw.eq.5 ) then
              if(isy.gt.0) then
c                 cfac = 1. ! constistent/lumpen mass matrix factor
                 do i = 1,ndm
                    call conlump(sl(1,0),nel,sr(1,0),nel,p(isy+i-1),
     $              s(isy+i-1,isx+i-1),rho,ndf,nst)
                 enddo
              endif

            elseif((isw.eq.4).or.(isw.eq.8))then
               if(isy.ne.0) then
                  detfl = det3d(fl(1,1,0))
                  do k = 1,6   ! Kirchhoff=>Cauchy in Gauss p.
                     sigp(k) = sig(k,0,0)/detfl
                  enddo
                  sigp(10) = detfl
               endif
               if(idx.ne.0) then
                  sigp(7) = alpha0(1,3)
                  sigp(8) = alpha0(2,3)
                  sigp(9) = alpha0(3,3)
c                 sigp(10) = sqrt( alpha(1,3)**2 + alpha(2,3)**2 )
               endif
               if(ichy.gt.0) then
                  call constx(v,x(0,1),nch,dch(1,1),
     $                                chch(1,1),chchch(1,1,1),fl(1,1,1))
                  sigp(7) = v(1,1) ! velocity v_x of 1st chemical compt
                  sigp(8) = v(2,1) !          v_y
                  sigp(9) = v(3,1) !          v_z
                  sigp(10) = x(0,1)! chemical fraction of 1st element
                  title(7) = '    V_x   '
                  title(8) = '    V_y   '
                  title(9) = '    V_z   '
               endif
               if(ipy.ne.0) then
                  sigp(7) = pol(1) ! electric polarity
                  sigp(8) = pol(2)
                  sigp(9) = pol(3)
                  sigp(10) = x(0,1)! chemical potential
                  title(7) = '    E_x   '
                  title(8) = '    E_y   '
                  title(9) = '    E_z   '
                  title(10)= '    U     '
               endif
               if(isw.ne.4) then
                  iste = 10
                  call slcn3dpd(sigp,iste,sl1(1,0),8,p(1),s(1,1),nel)
               else             ! to output file
                  mct = mct - 2
                  if(mct.gt.0) go to 430
                  write(iow,2001) o,head,title
                  if(ior.lt.0) write(*,2001) o,head,title
                  mct = 60
 430              continue
                  write(iow,2002) n,ma,sigp
                  if(ior.lt.0) write(*,2002) n,ma,sigp
               endif
            elseif(isw.eq.2.) then
               if(xsj.le.0.) then
                  write(iow,2003) n
                  stop
               endif
c              call checkshp(sg(1,l),sr(1,0) ,xu,ndm,27,9)
c              call checkshp(sg(1,l),sr1(1,0),xu,ndm,8,3)
               if (ndf.gt.20)stop
     *             'sig_u,fl_u,fl_u too small dimension in sigma3d'
            endif
         enddo
      endif

 2000 format(/5x,'27-node Hiperelastic - dislocated element:',/
     *  10x,'Number of Gaussa points = ',i4/
     *  10x,'Number of Gaussa points for stresses= ',i4,/
     *  10x,'Mass density = ',g10.3,/)
 2001 format(a1,20a4//5x,'Element: Stress',//' Elmt Matl',10a10)
 2002 format(i4,i4,1x,10(1pe10.2))
 2003 format('*CHECK Wrong definition of nodes in element no=',i7)
 2004 format('*ELMT14: div(x_n) must follow div(sig), please change the
     $div(x_n)/div(sig) into div(sig)/div(x_n) sequence in input file')
 2005 format('*ELMT14 Wrong specification of coupled problem ',
     $       a8,1x,i2,1x,i1,1x,i3)
 2006 format('*ELMT14 Blanck variable no=',i2,' in ndf')
      end

      subroutine readxy(isx, isy ,ix)
      integer isx,isy,ix
      if(ix.gt.0) then
         isx = ix
         isy = ix
      elseif(ix.lt.0) then
         isx = -ix
         isy = 0
      else
         stop 'ERROR: Elmt14, freedom degree number = 0'
      endif
      end
