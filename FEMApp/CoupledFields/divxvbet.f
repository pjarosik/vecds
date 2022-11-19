      subroutine divxvbet(sl,nl,sr,nr,x,ac,eb,s,ndf,nst)

c      written by Pawel Dluzewski in 2005-2014 
      implicit none
      integer i,j,i1,j1,ndf,nst,nl,nr
      real*8 xn,yn,zn,sl(nl,3),sr(nr,0:3),s(nst,*),eb(6,0:3),x,ac(6)
     $       ,chb(0:3),trshp

      do j = 0,3
         chb(j)=  ac(1)*eb(1,j) + ac(2)*eb(2,j) + ac(3)*eb(3,j)
     $          + ac(4)*eb(4,j) + ac(5)*eb(5,j) + ac(6)*eb(6,j)
      enddo

c     Tangent matrix chemical - source distortions
      i1 = 1
      do i = 1,nl
         xn = sl(i,1) * x*(1.-x) 
         yn = sl(i,2) * x*(1.-x)
         zn = sl(i,3) * x*(1.-x)
         j1 = 1
         do j = 1,nr
            trshp = xn * sr(j,1) + yn * sr(j,2) + zn * sr(j,3)
            s(i1,j1) = s(i1,j1) + trshp * chb(0)  
     $         + ( xn*chb(1) + yn*chb(2) + zn*chb(3) ) * sr(j,0)
            j1 = j1 + ndf
         enddo    
         i1 = i1 + ndf
      enddo       

      end
