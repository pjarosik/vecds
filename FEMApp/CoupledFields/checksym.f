      subroutine checksym(s,nst,ndf)

c     check the symmetry of tangent matrix s(i,j)
c     ndf    - number of degrees of freedom
c     nst    - dimension of s(nst,nst)

      implicit none
      integer nst,ndf,i,j
      real*8 s(nst,nst)

      do i=1,nst
         do j=i+1,nst
            if((abs(s(i,j)-s(j,i)).gt.1.e-6).and.(2.*abs(s(i,j)-s(j,i))/
     $           (abs(s(i,j))+abs(s(j,i))).gt.1.e-1)) then 
               write(*,*) 'ERROR: s(i,j)=',s(i,j),' s(j,i)=',s(j,i),
     $         'for i=',(((i+0.0e-09)/ndf)-int((i-1.)/ndf))*ndf,
     $         'and j=',(((j+0.0e-09)/ndf)-int((j-1.)/ndf))*ndf
c               pause 1
            endif
         enddo
      enddo
      
      end
