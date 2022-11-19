      subroutine divsig(sl,nl,sig,p,ndf)

      implicit none
      integer i1,i,ndf,nl
      real*8 sl(nl,3),p(*),sig(6)

c     sig - the Kirchhoff stress
c     dv = dv /detFe   
 
c     Residuum P 
      i1 = 1
      do i = 1,nl
         p(i1  ) = p(i1  ) -sl(i,1)*sig(1)-sl(i,2)*sig(6)-sl(i,3)*sig(5)
         p(i1+1) = p(i1+1) -sl(i,1)*sig(6)-sl(i,2)*sig(2)-sl(i,3)*sig(4)
         p(i1+2) = p(i1+2) -sl(i,1)*sig(5)-sl(i,2)*sig(4)-sl(i,3)*sig(3)
         i1 = i1 + ndf
      enddo       
      end
