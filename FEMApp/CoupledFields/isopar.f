c        Isoparametric family of subroutines needed for transformation of the shape
c        function gradients from the normal configuration of FE to the Eucleadian one,
c        Useful for all isoparametric (brick, prism, etc) FEs 
c        written by Paweł Dłużewski 2010 (UTF-8)

      subroutine shifter(shp,nel,xl,g)
c         Input:
c           shp       - Shape function gradient in natural conf
c           xl(3,nel) - Nodal coordinates for element
c           nel       - Number of nodes attached to element
c        Output:
c           g(3,3)    - shifter ${g_x}^N$  (Jacobian matrix) 

      implicit none
      integer   nel,i,j,k 
      real*8    shp(nel,3),xl(3,nel),f(3,3),g(3,3)

c     "deformation gradient analog"
      do i = 1,3
         do j = 1,3
            f(i,j) = 0.
            do k = 1,nel
               f(i,j) = f(i,j) + xl(i,k)*shp(k,j)
            enddo
         enddo
      enddo
      call inv3d(f,g)        
      call transpose(g)
      end

      subroutine christo(shp,nel,xl,g,ch)
c         Input:
c                 nel - Number of nodes attached to element
c          shp(nel,6) - Shape function gradients of 2nd order in natural conf
c           xl(3,nel) - Nodal coordinates for element
c              g(3,3) - shifter ${g_x}^N$, x-laboratory system, N-natural one convected with element  
c        Output:
c             ch(6,3)  - Christoffel symbols in natural coordinates   

      implicit none
      integer i,k,nel
      real*8 shp(nel,6),xl(3,nel),ch(6,3),s1,s2,s3,g(3,3)

      do i = 1,6
         s1 = 0.
         s2 = 0.
         s3 = 0.
         do k = 1,nel
            s1 = s1 + xl(1,k)*shp(k,i)
            s2 = s2 + xl(2,k)*shp(k,i)
            s3 = s3 + xl(3,k)*shp(k,i)
         enddo
         ch(i,1) = s1*g(1,1) + s2*g(2,1) + s3*g(3,1)
         ch(i,2) = s1*g(1,2) + s2*g(2,2) + s3*g(3,2)
         ch(i,3) = s1*g(1,3) + s2*g(2,3) + s3*g(3,3)
      enddo
      end

      subroutine isopar1(shp,nel,g)
c         Input: shp(nel,3) - shape function gradient of 1st order in the normal configuration of FE
c                 xl(3,nel) - Nodal coordinates for element
c                       nel - Number of nodes attached to element
c                    g(3,3) - shifter ${g_x}^N$  (Jacobian matrix) 
c        Output: shp(nel,3) - shape function gradient of 1st order in the Eucleadian configuration
c                Both shp (input/output) in the orthonormal coordinate system

      implicit none
      integer nel,k,i
      real*8  shp(nel,3),s1,s2,s3,g(3,3)
      do k = 1,nel
         s1 = shp(k,1)
         s2 = shp(k,2)
         s3 = shp(k,3)
         do i = 1,3
            shp(k,i)= s1*g(i,1) + s2*g(i,2) + s3*g(i,3)
         enddo
      enddo
      end


      subroutine isopar2(shp,nel,g,ch)
c        Input : shp(nel,9) - shape function gradient of 1st+2nd order in the normal configuration of FE
c                 xl(3,nel) - Nodal coordinates for element
c                       nel - Number of nodes attached to element
c                    g(3,3) - shifter ${g_x}^N$  (Jacobian matrix) 
c                 ch(4:9,3) - Christoffel symbols ch(MN,x)
c        Output: shp(nel,4:9) - shape function gradient of 2nd order in the Eulerian configuration
c        Both shp (input/output) in the orthonormal coordinate system
      implicit none
      integer k,i,nel 
      real*8 shp(nel,9),ch(4:9,3),g(3,3),gg(4:9,4:9),s4,s5,s6,s7,s8,s9

      call sym33to66(g,gg)
      do k = 1,nel
         s4 =shp(k,4)-shp(k,1)*ch(4,1)-shp(k,2)*ch(4,2)-shp(k,3)*ch(4,3)
         s5 =shp(k,5)-shp(k,1)*ch(5,1)-shp(k,2)*ch(5,2)-shp(k,3)*ch(5,3)
         s6 =shp(k,6)-shp(k,1)*ch(6,1)-shp(k,2)*ch(6,2)-shp(k,3)*ch(6,3)
         s7 =shp(k,7)-shp(k,1)*ch(7,1)-shp(k,2)*ch(7,2)-shp(k,3)*ch(7,3)
         s8 =shp(k,8)-shp(k,1)*ch(8,1)-shp(k,2)*ch(8,2)-shp(k,3)*ch(8,3)
         s9 =shp(k,9)-shp(k,1)*ch(9,1)-shp(k,2)*ch(9,2)-shp(k,3)*ch(9,3)
         do i = 4,9
            shp(k,i)= gg(i,4)*s4 + gg(i,5)*s5 + gg(i,6)*s6
     $               +gg(i,7)*s7 + gg(i,8)*s8 + gg(i,9)*s9
         enddo         
      enddo
      end

      subroutine isopar3(shp,nel,xl,g,ch)
c        Input :
c            shp(nel, 1:19) - shape function gradient of 3rd order in the natural coordinate system
c                 xl(3,nel) - Nodal coordinates for element
c                       nel - Number of nodes attached to element
c                    g(3,3) - shifters ${g_x}^N$
c                   ch(6,3) - Christoffel symbols
c        Output:
c            shp(nel,10:19) - shape function gradient of 3rd in actual conf
c            shp(nel, 1:9)  - remains unchanged  
      implicit none
      integer i,j,k,nel 
      real*8  shp(nel,1:19),xl(3,nel),g(3,3),f(3,3),ch(6,3),a(6,3)
     $       ,chh(10:19,3),dch(10:19,3),s(10:19),gg(10:19,10:19)
     $       ,s1,s2,s3
c     inner product of Christoffel symbols self overmultiplication 
      do i = 1,3 ! upper index
         chh(10,i)=(ch(1,1)*ch(1,i)+ch(1,2)*ch(6,i)+ch(1,3)*ch(5,i))*3. ! xxx
         chh(11,i)=(ch(2,1)*ch(6,i)+ch(2,2)*ch(2,i)+ch(2,3)*ch(4,i))*3. ! yyy
         chh(12,i)=(ch(3,1)*ch(5,i)+ch(3,2)*ch(4,i)+ch(3,3)*ch(3,i))*3. ! zzz
         chh(13,i)= ch(1,1)*ch(5,i)+ch(1,2)*ch(4,i)+ch(1,3)*ch(3,i)   
     $            +(ch(5,1)*ch(1,i)+ch(5,2)*ch(6,i)+ch(5,3)*ch(5,i))*2. ! xxz
         chh(14,i)= ch(2,1)*ch(1,i)+ch(2,2)*ch(6,i)+ch(2,3)*ch(5,i)
     $            +(ch(6,1)*ch(6,i)+ch(6,2)*ch(2,i)+ch(6,3)*ch(4,i))*2. ! yyx
         chh(15,i)= ch(3,1)*ch(6,i)+ch(3,2)*ch(2,i)+ch(3,3)*ch(4,i)
     $            +(ch(4,1)*ch(5,i)+ch(4,2)*ch(4,i)+ch(4,3)*ch(3,i))*2. ! zzy
         chh(16,i)=(ch(4,1)*ch(6,i)+ch(4,2)*ch(2,i)+ch(4,3)*ch(4,i))*2.
     $            + ch(2,1)*ch(5,i)+ch(2,2)*ch(4,i)+ch(2,3)*ch(3,i)     ! zyy
         chh(17,i)=(ch(5,1)*ch(5,i)+ch(5,2)*ch(4,i)+ch(5,3)*ch(3,i))*2.
     $            + ch(3,1)*ch(1,i)+ch(3,2)*ch(6,i)+ch(3,3)*ch(5,i)     ! xzz
         chh(18,i)=(ch(6,1)*ch(1,i)+ch(6,2)*ch(6,i)+ch(6,3)*ch(5,i))*2.
     $            + ch(1,1)*ch(6,i)+ch(1,2)*ch(2,i)+ch(1,3)*ch(4,i)     ! yxx
         chh(19,i)= ch(6,1)*ch(5,i)+ch(6,2)*ch(4,i)+ch(6,3)*ch(3,i)
     $            + ch(5,1)*ch(6,i)+ch(5,2)*ch(2,i)+ch(5,3)*ch(4,i)
     $            + ch(4,1)*ch(1,i)+ch(4,2)*ch(6,i)+ch(4,3)*ch(5,i)     ! xyz
      enddo
      dch = 0. ! Partial derivative of Christoffel symbols dependent on 3rd partial d. 
      do j = 10,19
         s1 = 0.
         s2 = 0.
         s3 = 0.
         do k = 1,nel
            s1 = s1 + shp(k,j)*xl(k,1)
            s2 = s2 + shp(k,j)*xl(k,2)
            s3 = s3 + shp(k,j)*xl(k,3)
         enddo
         dch(j,1) =  g(1,1)*s1 + g(1,2)*s2 + g(1,3)*s3 
         dch(j,2) =  g(2,1)*s1 + g(2,2)*s2 + g(2,3)*s3 
         dch(j,3) =  g(3,1)*s1 + g(3,2)*s2 + g(3,3)*s3 
      enddo

      call sym33to1010(g,gg)

      do k = 1,nel        ! The part dependent on 2nd partial d. of shape function
         f(1,1) = shp(k,4)
         f(2,2) = shp(k,5)
         f(3,3) = shp(k,6)
         f(3,2) = shp(k,7)
         f(1,3) = shp(k,8)
         f(2,1) = shp(k,9)
         f(2,3) = f(3,2)
         f(3,1) = f(1,3)
         f(1,2) = f(2,1)
         do j = 1,6
            do i = 1,3
               a(j,i) = f(i,1)*ch(j,1)+f(i,2)*ch(j,2)+f(i,3)*ch(j,3)
            enddo
         enddo
         s(10) = a(1,1) + a(1,1) + a(1,1) ! xxx   
         s(11) = a(2,2) + a(2,2) + a(2,2) ! yyy
         s(12) = a(3,3) + a(3,3) + a(3,3) ! zzz   
         s(13) = a(1,3) + a(5,1) + a(5,1) ! xxz   
         s(14) = a(2,1) + a(6,2) + a(6,2) ! yyx 
         s(15) = a(3,2) + a(4,3) + a(4,3) ! zzy 
         s(16) = a(4,2) + a(4,2) + a(2,3) ! zyy   
         s(17) = a(5,3) + a(5,3) + a(3,1) ! xzz 
         s(18) = a(6,1) + a(6,1) + a(1,2) ! yxx   
         s(19) = a(6,3) + a(5,2) + a(4,1) ! xyz   
         do i = 10,19
            s(i) = shp(k,i) -s(i) + shp(k,1)*dch(i,1)    
     $                            + shp(k,2)*dch(i,2)
     $                            + shp(k,3)*dch(i,3)
         enddo
         do i = 10,19
         shp(k,i) = gg(i,10)*s(10) +gg(i,11)*s(11) +gg(i,12)*s(12)
     $             +gg(i,13)*s(13) +gg(i,14)*s(14) +gg(i,15)*s(15)
     $             +gg(i,16)*s(16) +gg(i,17)*s(17) +gg(i,18)*s(18)
     $             +gg(i,19)*s(19)            
         enddo                                      
      enddo
      end
