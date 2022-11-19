      subroutine divxvx(sl,nl,sr,nr,x,xm,nch,v,chch,v_xx,s,ndf,nst)

c     written by Paweł Dłużewski (utf-8)
      implicit none
      integer i,j,l,m,i1,jl,nch,ndf,nst,nl,nr
      real*8 xn,yn,zn,sl(nl,0:3),sr(nr,0:3),s(nst,*),x,xm(0:3,nch),
     $       a,b,c,d,v(3),chch(nch),v_xx(nch,nch)

c     Tangent matrix chemical - chemical

      i1 = 1
      do i = 1,nl
         xn = - sl(i,1)*x*(1.-x)
         yn = - sl(i,2)*x*(1.-x)
         zn = - sl(i,3)*x*(1.-x)
         do l = 1,nch
            a = xn * chch(l)
            b = yn * chch(l)
            c = zn * chch(l)
            d = 0.
            do m = 1,nch
               d = d + (xn*xm(1,m)+yn*xm(2,m)+zn*xm(3,m))*v_xx(l,m)
            enddo
            jl = l
            do j = 1,nr
               s(i1,jl) = s(i1,jl) + a*sr(j,1)+b*sr(j,2)
     $                             + c*sr(j,3)+d*sr(j,0)
               jl = jl + ndf
            enddo
         enddo    
         d = - (1.-2.*x) * (sl(i,1)*v(1)+sl(i,2)*v(2)+sl(i,3)*v(3))
         jl = i1
         do j = 1,nr
            s(i1,jl) = s(i1,jl) + d * sr(j,0)
            jl = jl + ndf
         enddo
         i1 = i1 + ndf
      enddo                  

      end
