      subroutine conlump(sl,nl,sr,nr,p,s,rho,cfac,ndf,nst)
c     Consistent/lumpen mass projection to nodes
c     cfac - consistent mass matrix factor
c      rho - mass density (or molar concentration)
c            written by Paweł Dłużewski (2014)    

      implicit none
      integer i1,j1,i,j,ndf,nst,nl,nr
      real*8 sl(nl),sr(nr),ul(nl),p(*),s(nst,*),cfac,rho

      if(nl.ne.nr) stop 'ERROR: weighting and shape functions have diffe
     $rent size in conlump subroutine'
      i1 = 1
      do i = 1,nl
         p(i1) =    p(i1)    + sl(i)             * rho
         s(i1,i1) = s(i1,i1) + sl(i) * (1.-cfac) * rho 
         j1 = 1
         do j = 1,nr
            s(i1,j1)  = s(i1,j1)  + sl(i)*sr(j)* cfac * rho
            j1 = j1 + ndf
         enddo               
         i1 = i1 + ndf
      enddo             
      end