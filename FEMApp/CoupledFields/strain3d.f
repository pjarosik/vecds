      subroutine strain3d(fe,eps,sth)

c     isotropic tensor function of tensor variable 
c        input: fe(3,3) - elastic or lattice strain tensor
c                   sth - Seth strain exponent
c       output: eps(6,0,0) = f^Æ(u^”©¦) strain f. in the Voight notation 
c               eps(6,6,0) = A^Æ_”©Æ = FF_”©Æ^”©¦ * deps^Æ/dE^”©¦
c               eps(6,6,6) = B^Æ_”©Æœô¢Ó = FF_”©Æ^”©¦*FF_œô¢Ó^œô¢Ñ*d^2eps^Æ/dE^”©¦dE^œô¢Ñ
c     For checking correctness, compare with tfun6(eps,u)
c     Written by Pawe‚³ D‚³u‚¿ewski & Marcin Ma‚¼dziarz (2005-2013)
      
      implicit none
      integer i,j,k,l,m,n,ij,rot,inxvs(3,3)
      real*8 ff(6,6),cc(6,6),eps(6,0:6,0:6),fe(3,3),r(3,3),sth,
     $       u(3,3),w(3),e(3),a(6),b(3),bb(3,3,3),x,eps1d(0:2),f,er
      save inxvs, er
      data inxvs/1,6,5,6,2,4,5,4,3/, er/1.e-8/
   
      f(i,j) = 2.*(e(j)-e(i) -a(i)*(w(j)-w(i)))/((w(j)-w(i))**2)
      call polard3d(fe,r,u)! symmetric u^ÿ(6) in Voight notation 

c---------------------from-tfun6.f--(do-not-edit)--------------------- 
      call eig3(u,w,rot)    ! u: stretch => eigen vectors 

c     Obliczenia nie (!) podwojonych sk‚³adowych macierzy A i B
      a = 0.
      bb = 0.
      do i = 1,3                
         call seth(w(i),eps1d,sth) ! scalar v.funct.of stretch and deriv
         e(i) = eps1d(0)                  ! eps - strain
         a(i) = eps1d(1) / w(i)           ! d eps/ d Green
         b(i) =(eps1d(2) - a(i))/w(i)/w(i)! d2 eps/ d Green^2
         w(i) = 1/2.*(w(i)*w(i)-1.)       ! Green strain
      enddo
      do i = 1,2
         do j = i+1,3
            k = inxvs(i,j)
            if((w(i)-w(j))**2.lt.er**2) then
               a(k) = (a(i)+a(j))/4.
            else
               a(k) = (e(i)-e(j))/(w(i)-w(j))/2.
            endif
         enddo
      enddo
      do i = 1,3
         do j = 1,3
            do k = 1,3
               if(((w(i)-w(j))**2.lt.er**2).and.
     $            ((w(j)-w(k))**2.lt.er**2).and.
     $            ((w(k)-w(i))**2.lt.er**2))then
                  x = (b(i)+b(j)+b(k))/3.
               elseif((w(i)-w(j))**2.lt.er**2)then
                  x = (f(i,k)+f(j,k))/2.
               elseif((w(j)-w(k))**2.lt.er**2)then
                  x = (f(j,i)+f(k,i))/2.
               elseif((w(k)-w(i))**2.lt.er**2)then
                  x = (f(k,j)+f(i,j))/2.
               else
                  x = -2.*
     $              (e(i)*(w(j)-w(k))+e(j)*(w(k)-w(i))+e(k)*(w(i)-w(j)))
     $                  /((w(j)-w(k))     *(w(k)-w(i))     *(w(i)-w(j)))
               endif
               if(i.ne.j) x = x/2.
               if(j.ne.k) x = x/2.
               if(k.ne.i) x = x/2.
               bb(i,j,k) = x
            enddo
         enddo
      enddo
c-------------above---from-tfun6.f--(do-not-edit)--------------------- 
      
c     Rzutowanie skadowych na osie laboratoryjne
      call sym33to66(u,cc) ! direction consinuses 6x6
      call sym33to66(fe,ff)
      ff = matmul(ff,cc)

c-------------below---from-tfun6.f--(do-not-edit)--------------------
      do l = 1,6    ! with revised uu into cc or ff  
         eps(l,0,0) = e(1)*cc(l,1) +e(2)*cc(l,2) +e(3)*cc(l,3) 
         do m = 1,6
            eps(l,m,0) = 0.
            do n = 1,6
               eps(l,m,0) = eps(l,m,0) + a(n) * cc(l,n) * ff(m,n)
               eps(l,m,n) = cc(l,1)*ff(m,1)*ff(n,1) * bb(1,1,1)
     $                     +cc(l,2)*ff(m,2)*ff(n,2) * bb(2,2,2)
     $                     +cc(l,3)*ff(m,3)*ff(n,3) * bb(3,3,3)
     $                     +(cc(l,6)*ff(m,4)*ff(n,5)
     $                      +cc(l,6)*ff(m,5)*ff(n,4)
     $                      +cc(l,5)*ff(m,6)*ff(n,4)
     $                      +cc(l,5)*ff(m,4)*ff(n,6)
     $                      +cc(l,4)*ff(m,6)*ff(n,5)
     $                      +cc(l,4)*ff(m,5)*ff(n,6)) * bb(1,2,3)
               do i = 1,2
                  do j = i+1,3
                     ij = inxvs(i,j)
                     eps(l,m,n) = eps(l,m,n)
     $                    +(cc(l,ij)*ff(m, j)*ff(n,ij)
     $                     +cc(l,ij)*ff(m,ij)*ff(n, j)
     $                     +cc(l, j)*ff(m,ij)*ff(n,ij)) * bb(i,j,j)
     $                    +(cc(l, i)*ff(m,ij)*ff(n,ij)
     $                     +cc(l,ij)*ff(m, i)*ff(n,ij)
     $                     +cc(l,ij)*ff(m,ij)*ff(n, i)) * bb(i,i,j)
                  enddo
               enddo
            enddo
         enddo
      enddo
c     Podwajanie niediagonalnych skladowych kontrawariantnych
c     (tylko po pierwszym wska‚¼niku)
      do l = 4,6
         do m = 0,6
            do n = 0,6
               eps(l,m,n) = eps(l,m,n) + eps(l,m,n)
            enddo
         enddo
      enddo
      end
