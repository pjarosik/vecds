      subroutine divxveps(sl,nl,sr,nr,x,ac,f,fe,s,ndf,nst)

      implicit none
      integer i,j,l,i1,jl,ndf,nst,nl,nr
      real*8 xn,yn,zn,sl(nl,3),sr(nr,9),s(nst,*),
     $       x,ac(6),f(3,3),fe(9,3),s1,s2,s3,s4,s5,s6,s7,s8,s9,
     $       v1(3),v2(3),v3(3),v4(3),v5(3),v6(3)

      do i = 1,3
         v4(i) = ac(1)*f(1,i) +ac(6)*f(2,i) +ac(5)*f(3,i)   
         v5(i) = ac(6)*f(1,i) +ac(2)*f(2,i) +ac(4)*f(3,i)   
         v6(i) = ac(5)*f(1,i) +ac(4)*f(2,i) +ac(3)*f(3,i)

         v1(i) =   fe(1,1)*v4(i) + fe(4,1)*v5(i) + fe(7,1)*v6(i)
     $           + fe(2,1)*v5(i)
     $           + fe(3,1)*v6(i)
         v2(i) =                   fe(4,2)*v4(i)
     $           + fe(2,2)*v4(i) + fe(5,2)*v5(i) + fe(8,2)*v6(i)
     $                           + fe(6,2)*v6(i)
         v3(i) =                                   fe(7,3)*v4(i)
     $                                           + fe(8,3)*v5(i)
     $           + fe(3,3)*v4(i) + fe(6,3)*v5(i) + fe(9,3)*v6(i)   
      enddo

      i1 = 1
      do i = 1,nl
         xn = -sl(i,1)*x*(1.-x)
         yn = -sl(i,2)*x*(1.-x)
         zn = -sl(i,3)*x*(1.-x)
         do l = 1,3
            s1 =  xn * v1(l)
            s2 =  yn * v2(l)
            s3 =  zn * v3(l)
            s4 =  xn * v4(l)
            s5 =  yn * v5(l)
            s6 =  zn * v6(l)
            s7 =  yn * v6(l) + zn * v5(l)
            s8 =  zn * v4(l) + xn * v6(l)
            s9 =  xn * v5(l) + yn * v4(l)

c     Tangent matrix chemical-displacement interaction  
            jl = l
            do j = 1,nr
               s(i1,jl)=s(i1,jl) +s1*sr(j,1) +s2*sr(j,2) +s3*sr(j,3)   
     $                           +s4*sr(j,4) +s5*sr(j,5) +s6*sr(j,6)   
     $                           +s7*sr(j,7) +s8*sr(j,8) +s9*sr(j,9)   
            jl = jl + ndf
            enddo
         enddo                 
         i1 = i1 + ndf
      enddo                    

      end
