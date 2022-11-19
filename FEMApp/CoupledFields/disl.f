      subroutine disl(shp,nel,shp8,nel2,ue,ndf,up,nb,fb,fl,f,epsb)

c     input: fl(3,3,0:3) - 1st and 2nd chemical def.
c                           (2nd translated to current conf.)
c             fb(3,3,nb) - codding matrix for 3x3 plast.d.
c     output:
c          fl(i,K,0) = Fl(i,K)
c          fl(i,j,k) = Fl(i,K),k Fl^-1 (K,j) = -Fl(i,K) Fl^-1 (K,j),k
c      epsb(ij,KL,0) = F(i,K) Flt^-1(L,j)             
c      epsb(ij,KL,k) = -Fl(i,M)    Fpl(M,K),k          Flt^-1(L,j)
c                    =   F(i,M) Fpl^-1(M,N),k Fpl(N,K) Flt^-1(L,j)
c                                    written by Pawel Dluzewski (2006+)
      implicit none
      integer nel,nel2,ndf,i,j,k,m,n,nb
      real*8 shp(nel,9),shp8(nel2,0:3),up(ndf,*),ue(ndf,*),
     $       fb(3,3,nb),epsb(6,0:3,9),
     $       f(3,3,0:3), finv(3,3,0:3),fpinv(3,3,0:3),
     $       fl(3,3,0:3),fp(3,3),flinv(3,3),ui(9),g(3,3),ff(3),t1,t2,t3

c     The inverse 1st and 2nd total deformation gradients  F^-1(I,ij) 
      do i = 1,3
         do j = 1,9
            ui(j) = 0.
            do k = 1,nel
               ui(j) = ui(j) - ue(i,k) * shp(k,j)
            enddo
         enddo
         finv(i,1,0) = ui(1)
         finv(i,2,0) = ui(2)
         finv(i,3,0) = ui(3)
         finv(i,1,1) = ui(4)
         finv(i,2,2) = ui(5)
         finv(i,3,3) = ui(6)
         finv(i,3,2) = ui(7)
         finv(i,2,3) = ui(7)
         finv(i,1,3) = ui(8)
         finv(i,3,1) = ui(8)
         finv(i,2,1) = ui(9)
         finv(i,1,2) = ui(9)
         finv(i,i,0) = finv(i,i,0) + 1.
      enddo 

c     Inverse of plastic deformation tensor   
      do j = 0,3      ! j = 0   -- inverse of plastic deformation tensor
         do i = 1,9   ! j = 1,3 -- 2nd gradient with 2nd subscript
            fpinv(i,1,j) = 0. !    referred to current configuration
            do m = 1,nel2     !    Fp^-1(I1,I2),j
               do k = 1,nb
                  fpinv(i,1,j)=fpinv(i,1,j)+fb(i,1,k)*up(k,m)*shp8(m,j)
               enddo
            enddo
         enddo
      enddo
      fpinv(1,1,0) = fpinv(1,1,0) + 1.
      fpinv(2,2,0) = fpinv(2,2,0) + 1.
      fpinv(3,3,0) = fpinv(3,3,0) + 1.

      call inv334(finv,f) 
      call matmul334(f,fpinv,fl)
      call inv3d(fl(1,1,0),g)
      do i = 1,3
         do k = 1,3 
            t1 = fl(i,1,k) 
            t2 = fl(i,2,k) !fl(i,j,k) =  Fl(i,K),k Fl^-1 (K,j) 
            t3 = fl(i,3,k) !          = -Fl(i,K)   Fl^-1 (K,jk)
            do j = 1,3
               fl(i,j,k) = t1*g(1,j) + t2*g(2,j) + t3*g(3,j) 
            enddo
         enddo
      enddo

c     Derivative of strain and strain gradient eps(ij),k
c     over plastic distortion gradient beta(KL),k in Gauss points
c        epsb(ij,KL,0) = F(i,K) Flt^-1(L,j)             
c        epsb(ij,KL,k) = -Flt(i,M)    Fpl(M,K),k          Flt^-1(L,j)
c                      =    F(i,M) Fpl^-1(M,N),k Fpl(N,K) Flt^-1(L,j)  
      call inv3d(fpinv(1,1,0),fp)
      call inv3d(   fl(1,1,0),flinv)

      do k = 0,3
         do j = 1,3
            do i = 1,3 ! pomocnicze ff(I,J,k) =  Fp^-1(I,L),k * Fp(L,J)
               ff(i) = fpinv(i,1,k)*fp(1,j)
     $                +fpinv(i,2,k)*fp(2,j)
     $                +fpinv(i,3,k)*fp(3,j)
            enddo
            do i = 1,3
               g(i,j) = f(i,1,0)*ff(1)+f(i,2,0)*ff(2)+f(i,3,0)*ff(3) 
            enddo
         enddo
         do j = 1,nb
            do i = 1,6
               epsb(i,k,j) = 0.
            enddo
            do m = 1,3
               do n = 1,3
                epsb(1,k,j)=epsb(1,k,j)+ g(1,m)*flinv(n,1) *fb(m,n,j)
                epsb(2,k,j)=epsb(2,k,j)+ g(2,m)*flinv(n,2) *fb(m,n,j)
                epsb(3,k,j)=epsb(3,k,j)+ g(3,m)*flinv(n,3) *fb(m,n,j)
                epsb(4,k,j)=epsb(4,k,j)+(g(2,m)*flinv(n,3)
     $                                +g(3,m)*flinv(n,2))*fb(m,n,j)
                epsb(5,k,j)=epsb(5,k,j)+(g(3,m)*flinv(n,1)
     $                                +g(1,m)*flinv(n,3))*fb(m,n,j)
                epsb(6,k,j)=epsb(6,k,j)+(g(1,m)*flinv(n,2)
     $                                  +g(2,m)*flinv(n,1))*fb(m,n,j)
               enddo
            enddo
         enddo
      enddo
      end
