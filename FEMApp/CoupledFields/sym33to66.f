      subroutine sym33to66(v,u)
      implicit none
      integer i,j,k,inxvs(3,3)
      real*8 u(6,6),v(3,3)
      save inxvs
      data inxvs/1,6,5,6,2,4,5,4,3/
      do i = 1,3
         do j = 1,i
            k = inxvs(i,j) 
            u(k,1) = v(i,1) * v(j,1)
            u(k,2) = v(i,2) * v(j,2)
            u(k,3) = v(i,3) * v(j,3)
            u(k,4) = v(i,2) * v(j,3) + v(i,3) * v(j,2)
            u(k,5) = v(i,3) * v(j,1) + v(i,1) * v(j,3)  
            u(k,6) = v(i,1) * v(j,2) + v(i,2) * v(j,1)
         enddo
      enddo
      end
