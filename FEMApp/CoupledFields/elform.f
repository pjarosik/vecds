      subroutine elforms(sl,nl,ml,sr,nr,mr,d,s,ndf,nst,mdt)
c     Projection of elemental tangent matrix from the integration point
c                                                              to nodes
c     input:   sl(nl,ml) - left shape function with nl nodes and ml grad
c              sr(nr,mr) - right shape funct., generally sr = sl*dv
c              d(mdt,mr) - tangent constitutive matrix
c     output:  s(nst,nr) - elemental tangent stiffness matrix
c              written by Paweł Dłużewski (2014)
        implicit none
      integer ndf,nst,mdt,nl,ml,nr,mr,i1,j1,i,j,m,n
      real*8 sl(nl,ml),sr(nr,mr),s(nst,nr),d(mdt,mr)
      do m = 1,mr
         do n = 1,ml
            i1 = 1
            do i = 1,nl
               j1 = 1
               do j = 1,nr
                  s(i1,j1) = s(i1,j1) + sl(i,n) * d(n,m) * sr(j,m)
                  j1 = j1 + ndf
               enddo             
               i1 = i1 + ndf
            enddo                
         enddo
      enddo
      end

      subroutine elformp(sl,nl,ml,d,p,ndf)
c     Projection of elemental tangent matrix from the integration point
c                                                              to nodes
c     input:   sl(nl,ml) - left shape function with nl nodes and ml grad
c                  d(ml) - vector of forces in integration point
c     output:      p(nl) - elemental vector of nodal forces
c              written by Paweł Dłużewski (2014)
      implicit none
      integer ndf,nl,ml,i1,i,n
      real*8 sl(nl,ml),d(ml),p(nl)
      do n = 1,ml
         i1 = 1
         do i = 1,nl
            p(i1) = p(i1) + sl(i,n) * d(n)
            i1 = i1 + ndf
         enddo                
      enddo
      end


