      subroutine slcn3dpd(sig,iste,sl,n,p,s,nen) ! usunąć nen po testach
c     Purpose: Project element variables to nodes
c     Inputs:                                 | Outputs:                                    
c     sig(iste)- Stresses at quadrature point | p(nen)   - Weights for 'lumped' projection
c     shp(n) - Weight functions for q.point | s(nen,*) - Integral of variables          

      implicit  none
      integer   i,j,iste,n,nen
      real*8    p(nen),s(nen,iste),sl(n),sig(iste)

      do i = 1,n
         p(i) = p(i) + sl(i)
         do j = 1,iste
            s(i,j) = s(i,j) + sl(i)* sig(j) 
         enddo
      enddo
      end
