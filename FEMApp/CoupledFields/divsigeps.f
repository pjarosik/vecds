      subroutine divsigeps(sl,nl,sr,nr,d,s,ndf,nst)

c     input: sl = N_i,k *dv -- weight function * volume element
c            sr = N_j,l     -- shape function
c       dd(6,0) = sig(6)    -- Kirchhoff stress
c       dd(6,6) = A^T *dsig/deps* A + B^T *Sig  -- Sig conjugate stress
c     output: s -- tangent matrix;    written by Paweł Dłużewski (2013)        
      implicit none
      integer i0,j0,ik,jk,j1,j2,j3,i,j,k,ndf,nst,nl,nr
      real*8 sl(nl,3),sr(nr,3),s(nst,nst),d(6,0:6),a(3,0:6)

      i0 = 0
      do i = 1,nl
         do k = 0,6
            a(1,k) = sl(i,1)*d(1,k) + sl(i,2)*d(6,k) + sl(i,3)*d(5,k)
            a(2,k) = sl(i,1)*d(6,k) + sl(i,2)*d(2,k) + sl(i,3)*d(4,k)
            a(3,k) = sl(i,1)*d(5,k) + sl(i,2)*d(4,k) + sl(i,3)*d(3,k)
         enddo
         j0 = 0
         do j = 1,nr
            j1 = j0 + 1
            j2 = j0 + 2
            j3 = j0 + 3
            do k = 1,3
               ik = i0+k
               jk = j0+k
          s(ik,j1)=s(ik,j1)+a(k,1)*sr(j,1)+a(k,6)*sr(j,2)+a(k,5)*sr(j,3)
          s(ik,j2)=s(ik,j2)+a(k,6)*sr(j,1)+a(k,2)*sr(j,2)+a(k,4)*sr(j,3)
          s(ik,j3)=s(ik,j3)+a(k,5)*sr(j,1)+a(k,4)*sr(j,2)+a(k,3)*sr(j,3)
c         Geometric matrix 
          s(ik,jk)=s(ik,jk)+a(1,0)*sr(j,1)+a(2,0)*sr(j,2)+a(3,0)*sr(j,3)
            enddo
            j0 = j0 + ndf
         enddo                  ! i
         i0 = i0 + ndf
      enddo                     ! j
      end
