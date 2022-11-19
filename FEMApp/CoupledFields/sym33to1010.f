      subroutine sym33to1010(v,u)
      implicit none
      integer i,j,k,l,ijk,inx(0:9)
      real*8 u(0:9,0:9),v(3,3),v1(3),v2(3),v3(3)
      save inx
c              0,1,2,3,4,5,6,7,8,9
      data inx/0,8,4,1,3,9,6,7,5,2/
c            i x y y y z z z z z z
c            j x x y y x x y y z z
c            k x x x y x x x y y z

      do k = 1,3
         v1(k) = v(k,1)
         v2(k) = v(k,2)
         v3(k) = v(k,3)
      enddo 

      ijk = 0
      do i = 1,3
         do j = 1,i
            do k = 1,j
               l = inx(ijk)
            u(l,0)=v1(i)*v1(j)*v1(k) ! xxx 
            u(l,1)=v2(i)*v2(j)*v2(k) ! yyy
            u(l,2)=v3(i)*v3(j)*v3(k) ! zzz 
            u(l,3)=v1(i)*v1(j)*v3(k)+v1(i)*v3(j)*v1(k)+v3(i)*v1(j)*v1(k) ! xxz
            u(l,4)=v2(i)*v2(j)*v1(k)+v2(i)*v1(j)*v2(k)+v1(i)*v2(j)*v2(k) ! yyx
            u(l,5)=v3(i)*v3(j)*v2(k)+v3(i)*v2(j)*v3(k)+v2(i)*v3(j)*v3(k) ! zzy
            u(l,6)=v2(i)*v2(j)*v3(k)+v2(i)*v3(j)*v2(k)+v3(i)*v2(j)*v2(k) ! zyy
            u(l,7)=v3(i)*v3(j)*v1(k)+v3(i)*v1(j)*v3(k)+v1(i)*v3(j)*v3(k) ! xzz
            u(l,8)=v1(i)*v1(j)*v2(k)+v1(i)*v2(j)*v1(k)+v2(i)*v1(j)*v1(k) ! yxx
            u(l,9)=v1(i)*v2(j)*v3(k)+v1(i)*v3(j)*v2(k)+v3(i)*v1(j)*v2(k) 
     $            +v3(i)*v2(j)*v1(k)+v2(i)*v1(j)*v3(k)+v2(i)*v3(j)*v1(k) ! xyz
            ijk = ijk + 1
            enddo
         enddo
      enddo
      end
