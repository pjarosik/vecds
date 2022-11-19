      subroutine qmill(x,z,q)

c     This subroutine generates the rotation tensor for the coordinate set, 
c                    e1 => x, and e3 => z, 
      implicit none
      integer i,k,l
      real*8  q(3,3),e(3,3,3),x0,z0,x(3),z(3)
c                 111   211  311   121  221   321   131  231  331
      data     e/0.d0, 0.d0,0.d0, 0.d0,0.d0,-1.d0, 0.d0,1.d0,0.d0,
c                 112   212  312   122  222   322   132  232  332
     *           0.d0, 0.d0,1.d0, 0.d0,0.d0, 0.d0,-1.d0,0.d0,0.d0,
c                 113   213  313   123  223   323   133  233  333
     *           0.d0,-1.d0,0.d0, 1.d0,0.d0, 0.d0, 0.d0,0.d0,0.d0/    

      x0 = sqrt(x(1)**2+x(2)**2+x(3)**2+0.d0)
      z0 = sqrt(z(1)**2+z(2)**2+z(3)**2+0.d0)

      do i=1,3
         q(1,i)=x(i)/x0
         q(3,i)=z(i)/z0
      enddo

      do i=1,3
         q(2,i)=0.d0
         do k=1,3
            do l=1,3
               q(2,i) = q(2,i) + q(3,k)*q(1,l)*e(k,l,i)
            enddo
         enddo
      enddo

      end
