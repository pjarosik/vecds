      subroutine divpol(sl,nl,po,p,ndf)

      implicit none
      integer i,i1,ndf,nl
      real*8 sl(nl,3),p(*),po(3)

c     pol - the polarity multiplied by detFe^-1 (Kirchhoff polarity)
c     dv = dv /detFe   

      i1 = 1
      do i = 1,nl
         p(i1) = p(i1) + sl(i,1)*po(1) + sl(i,2)*po(2) + sl(i,3)*po(3) ! oposite to div(sig)=0 to have symmetric  
         i1 = i1 + ndf                                                 ! piezoelectric submatrices
      enddo       
      end
