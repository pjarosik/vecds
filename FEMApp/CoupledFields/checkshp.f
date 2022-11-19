      subroutine checkshp(ss,shp,xu,ndm,nel,ishp)
c
c     Program for testing the shape function derivatives of 1-3 order 3D FEs
c     Paweł Dłużewski 2005-2009 

      implicit none

      integer  i,j,k,l,ndm,nel,ishp,is,js,ks
      real*8 ss(3),shp(nel,0:19),xu(3,nel),g(3,3),s(-3:3,-3:3,-3:3,64),
     $     g1(3,3),g2(3,3),g3(3,3),s1(3),s2(3),s3(3),detf,shp0(64,0:19),
     $       det3d,d,dd,shp1(19)

             common/ttshp/ g

c     Save of current shape function
      do i = 0,ishp
         do k = 1,nel
            shp0(k,i) = shp(k,i)
         enddo
      enddo
      detf = 1/det3d(g(1,1))
      d = detf**0.3333 * 1.e-5 ! Set delta x,y,z=d in global conf.

c     Variation of Gauss position in natural conf. 
      g1 = g 
      do i  = -ndm,ndm
         is = 1
         do l = 1,ndm
            s1(l) = ss(l) + g1(abs(i),l) * sign(is,i)*d
         enddo
         call shape3d(s1,detf,shp(1,0),xu,nel)
         g2 = g
         do j = -ndm,ndm
            js = 1
            do l = 1,ndm
               s2(l) = s1(l) + g2(abs(j),l) * sign(js,j)*d
            enddo
            call shape3d(s2,detf,shp(1,0),xu,nel)
            g3 = g
            do k = -ndm,ndm
               ks = 1
               do l = 1,ndm
                  s3(l) = s2(l) + g3(abs(k),l) * sign(ks,k)*d
               enddo
               call shape3d(s3,detf,shp(1,0),xu,nel)
               do l = 1,nel
                  s(i,j,k,l) = shp(l,0)
               enddo
            enddo
         enddo
      enddo
      do k=1,nel
         if(ndm.eq.3) then 
      shp1(1)=( s(1,0,0,k)-s(-1,0,0,k))/2./d !x
      shp1(2)=( s(2,0,0,k)-s(-2,0,0,k))/2./d !y
      shp1(3)=( s(3,0,0,k)-s(-3,0,0,k))/2./d !z
      dd = 4.*d**2
      shp1(4)=(s(-1,-1,0,k)-s(-1,1,0,k)+s(1,1,0,k)-s(1,-1,0,k))/dd !xx
      shp1(5)=(s(-2,-2,0,k)-s(-2,2,0,k)+s(2,2,0,k)-s(2,-2,0,k))/dd !yy
      shp1(6)=(s(-3,-3,0,k)-s(-3,3,0,k)+s(3,3,0,k)-s(3,-3,0,k))/dd !zz
      shp1(7)=(s(-2,-3,0,k)-s(-2,3,0,k)+s(2,3,0,k)-s(2,-3,0,k))/dd !yz
      shp1(8)=(s(-3,-1,0,k)-s(-3,1,0,k)+s(3,1,0,k)-s(3,-1,0,k))/dd !zx
      shp1(9)=(s(-1,-2,0,k)-s(-1,2,0,k)+s(1,2,0,k)-s(1,-2,0,k))/dd !xy
      dd = 2.*d**3
      shp1(10)=(s(1,1,0,l)-2.*s(1,0,0,l)+2.*s(0,0,-1,l)-s(0,-1,-1,l))/dd!xxx
      shp1(11)=(s(2,2,0,l)-2.*s(2,0,0,l)+2.*s(0,0,-2,l)-s(0,-2,-2,l))/dd!yyy
      shp1(12)=(s(3,3,0,l)-2.*s(3,0,0,l)+2.*s(0,0,-3,l)-s(0,-3,-3,l))/dd!zzz
      dd = 8.*d**3 
      shp1(13)= (s( 1, 2,0,l)-s( 1,-2,0,l)-2.*s( 2,0,0,l)               ! MM&GJ=>PD
     $          -s(-1,-2,0,l)+s(-1, 2,0,l)+2.*s(-2,0,0,l))/dd           !xxy 14=>13
      shp1(14)= (s( 2, 3,0,l)-s( 2,-3,0,l)-2.*s( 3,0,0,l)
     $          -s(-2,-3,0,l)+s(-2, 3,0,l)+2.*s(-3,0,0,l))/dd           !yyz 16=>14
      shp1(15)= (s( 3, 1,0,l)-s( 3,-1,0,l)-2.*s( 1,0,0,l)
     $          -s(-3,-1,0,l)+s(-3, 1,0,l)+2.*s(-1,0,0,l))/dd           !zzx 17=>15
      shp1(16)= (s( 1, 3,0,l)-s( 1,-3,0,l)-2.*s( 3,0,0,l)
     $          -s(-1,-3,0,l)+s(-1, 3,0,l)+2.*s(-3,0,0,l))/dd           !zyy 13=>16
      shp1(17)= (s( 2, 1,0,l)-s( 2,-1,0,l)-2.*s( 1,0,0,l)
     $          -s(-2,-1,0,l)+s(-2, 1,0,l)+2.*s(-1,0,0,l))/dd           !yyx 15=>17
      shp1(18)= (s( 3, 2,0,l)-s( 3,-2,0,l)-2.*s( 2,0,0,l)
     $          -s(-3,-2,0,l)+s(-3, 2,0,l)+2.*s(-2,0,0,l))/dd           !zzy 
      dd = d*d*d
      shp1(19)=(s(3, 2, 1,l)-s(3, 2,-1,l)-s(3, 1,-2,l)+s( 3,-1,-2,l)
     $         -s(2, 1,-3,l)+s(2,-1,-3,l)+s(1,-2,-3,l)-s(-1,-2,-3,l))/dd!xyz
         else
            stop 'TESTSHP has not been programmed for ndm.ne.3 yet,
     $      you can complete the section in mentioned subroutine'
         endif
         do i = 1,ishp
            detf = dabs(shp0(k,i)-shp1(i))
     $         /(0.5*dabs(shp0(k,i))+0.5*dabs(shp1(i)))
c            if(i.le.ndm.and.detf.gt.1.e-3) then 
c               write(*,100)ss,i,k,shp0(k,i),shp1(i)
c               call sleep(3)
c            elseif(i.gt.ndm.and.detf.gt.1.e9 ) then 
c               call sleep(3)
c            endif
            write(*,100)ss,k,i,shp0(k,i),shp1(i)
            call sleep(2)
         enddo
      enddo
 100  format('* SHAPE FUNCTION ERROR: for Natural coords ss =',f6.4,2x,
     $     f6.4,2x,f6.4/
     $     'Analytical value obtained for shp(',i2,',',i2,')=',e10.4,2x,
     $     ' while FDM gives ',e10.4,'!!!')

c     Recover the shape function
      do i = 0,ishp
         do k = 1,nel
            shp(k,i) = shp0(k,i)
         enddo
      enddo

      return
      end
