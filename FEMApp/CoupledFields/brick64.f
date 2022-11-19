      subroutine brick64(ss,shp)
      implicit none
c
c     History of this subroutine developed under direction of Pawel Dluzewski (=>2009): 
c     as far as PD remembers: 
c     Horacio Antunez + PD => 2D (~1995), next Macin Mazdziarz + PD => 3D (2009)
c.... calculate 1st & 2nd & 3rd derivatives of shape functions for 3D brick element in global coords

      integer  i,ir(64),is(64),it(64), iri,isi,iti 
      real*8   r,nr(4),dr(4),dr2(4),dr3(4),s,ns(4),ds(4),ds2(4),ds3(4),
     $         t,nt(4),dt(4),dt2(4),dt3(4),ss(3),shp(64,0:19)
      save     ir,is,it 

      data       ir /1,2,2,1, 1,2,2,1, 1,2,2,1, 1,2,2,1,
     &               3,4,2,2,4,3,1,1,3,4,4,3,
     &               3,4,2,2,4,3,1,1,3,4,4,3,
     &               3,4,2,2,4,3,1,1,3,4,4,3,
     &               3,4,2,2,4,3,1,1,3,4,4,3/,
     &           is /1,1,2,2, 1,1,2,2, 1,1,2,2, 1,1,2,2,
     &               1,1,3,4,2,2,4,3,3,3,4,4,
     &               1,1,3,4,2,2,4,3,3,3,4,4,
     &               1,1,3,4,2,2,4,3,3,3,4,4,
     &               1,1,3,4,2,2,4,3,3,3,4,4/,
     &           it /1,1,1,1, 2,2,2,2, 3,3,3,3, 4,4,4,4,
     &               1,1,1,1,1,1,1,1,1,1,1,1,
     &               2,2,2,2,2,2,2,2,2,2,2,2,
     &               3,3,3,3,3,3,3,3,3,3,3,3,
     &               4,4,4,4,4,4,4,4,4,4,4,4/

c     Set 1-d shape functions for each local direction

      r = ss(1)
      s = ss(2)
      t = ss(3)
      nr(3) = 9.0d0*(1.d0 - r*r)*(1.d0 - 3.d0*r)/16.d0
      nr(4) = 9.0d0*(1.d0 - r*r)*(1.d0 + 3.d0*r)/16.d0
      nr(1) = 0.5d0 * (1.d0 - r) - (2.d0*nr(3) + nr(4))/3.d0
      nr(2) = 0.5d0 * (1.d0 + r) - (2.d0*nr(4) + nr(3))/3.d0

      ns(3) = 9.0d0*(1.d0 - s*s)*(1.d0 - 3.d0*s)/16.d0
      ns(4) = 9.0d0*(1.d0 - s*s)*(1.d0 + 3.d0*s)/16.d0
      ns(1) = 0.5d0 * (1.d0 - s) - (2.d0*ns(3) + ns(4))/3.d0
      ns(2) = 0.5d0 * (1.d0 + s) - (2.d0*ns(4) + ns(3))/3.d0

      nt(3) = 9.0d0*(1.d0 - t*t)*(1.d0 - 3.d0*t)/16.d0
      nt(4) = 9.0d0*(1.d0 - t*t)*(1.d0 + 3.d0*t)/16.d0
      nt(1) = 0.5d0 * (1.d0 - t) - (2.d0*nt(3) + nt(4))/3.d0
      nt(2) = 0.5d0 * (1.d0 + t) - (2.d0*nt(4) + nt(3))/3.d0

      dr(3) = 9.0d0*( 9.d0*r*r - 2.d0*r - 3.d0)/16.d0
      dr(4) = 9.0d0*(-9.d0*r*r - 2.d0*r + 3.d0)/16.d0
      dr(1) =-0.5d0 - (2.d0*dr(3) + dr(4))/3.d0
      dr(2) = 0.5d0 - (2.d0*dr(4) + dr(3))/3.d0

      dr2(1) = 9.0d0*(1.0d0-3.0d0*r)/8.0d0
      dr2(2) = 9.0d0*(3.0d0*r+1.0d0)/8.0d0
      dr2(3) = 9.0d0*(9.0d0*r-1.0d0)/8.0d0
      dr2(4) =-9.0d0*(9.0d0*r+1.0d0)/8.0d0

      dr3(1) =-27.0d0/8.0d0
      dr3(2) = 27.0d0/8.0d0
      dr3(3) = 81.0d0/8.0d0
      dr3(4) =-81.0d0/8.0d0

      ds(3) = 9.0d0*( 9.d0*s*s - 2.d0*s - 3.d0)/16.d0
      ds(4) = 9.0d0*(-9.d0*s*s - 2.d0*s + 3.d0)/16.d0
      ds(1) =-0.5d0 - (2.d0*ds(3) + ds(4))/3.d0
      ds(2) = 0.5d0 - (2.d0*ds(4) + ds(3))/3.d0

      ds2(1) = 9.0d0*(1.0d0-3.0d0*s)/8.0d0
      ds2(2) = 9.0d0*(3.0d0*s+1.0d0)/8.0d0
      ds2(3) = 9.0d0*(9.0d0*s-1.0d0)/8.0d0
      ds2(4) =-9.0d0*(9.0d0*s+1.0d0)/8.0d0

      ds3(1) =-27.0d0/8.0d0
      ds3(2) = 27.0d0/8.0d0
      ds3(3) = 81.0d0/8.0d0
      ds3(4) =-81.0d0/8.0d0

      dt(3) = 9.0d0*( 9.d0*t*t - 2.d0*t - 3.d0)/16.d0
      dt(4) = 9.0d0*(-9.d0*t*t - 2.d0*t + 3.d0)/16.d0
      dt(1) =-0.5d0 - (2.d0*dt(3) + dt(4))/3.d0
      dt(2) = 0.5d0 - (2.d0*dt(4) + dt(3))/3.d0

      dt2(1) = 9.0d0*(1.0d0-3.0d0*t)/8.0d0
      dt2(2) = 9.0d0*(3.0d0*t+1.0d0)/8.0d0
      dt2(3) = 9.0d0*(9.0d0*t-1.0d0)/8.0d0
      dt2(4) =-9.0d0*(9.0d0*t+1.0d0)/8.0d0

      dt3(1) =-27.0d0/8.0d0
      dt3(2) = 27.0d0/8.0d0
      dt3(3) = 81.0d0/8.0d0
      dt3(4) =-81.0d0/8.0d0

c     Set local 3-d shape functions
c     shp(i, 0) =    N

c     shp(i, 1) =   dN/dr      shp(i, 2) =   dN/ds        shp(i, 3) =  dN/dt
c     shp(i, 4) = d^2N/drdr    shp(i, 5) = d^2N/dsds      shp(i, 6) = d^2N/dtdt
c     shp(i, 7) = d^2N/dtds    shp(i, 8) = d^2N/drdt      shp(i, 9) = d^2N/dsdr

c     shp(i,10) = d^3N/drdrdr  shp(i,11) = d^3N/dsdsds    shp(i,12) = d^3N/dtdtdt
c     shp(i,13) = d^3N/drdrdt  shp(i,14) = d^3N/dsdsdr    shp(i,15) = d^3N/dtdtds
c     shp(i,16) = d^3N/dtdsds  shp(i,17) = d^3N/drdtdt    shp(i,18) = d^3N/dsdrdr
c                                                         shp(i,19) = d^3N/drdsdt
      do i = 1,64
         iri = ir(i)
         isi = is(i)
         iti = it(i)
         shp(i,0) = nr(iri) * ns(isi) * nt(iti)
         
         shp(i,1) = dr(iri) * ns(isi) * nt(iti)
         shp(i,2) = nr(iri) * ds(isi) * nt(iti)
         shp(i,3) = nr(iri) * ns(isi) * dt(iti)    
               
         shp(i,4) =dr2(iri) * ns(isi) * nt(iti) !xx
         shp(i,5) = nr(iri) *ds2(isi) * nt(iti) !yy
         shp(i,6) = nr(iri) * ns(isi) *dt2(iti) !zz
         shp(i,7) = nr(iri) * ds(isi) * dt(iti) !zy
         shp(i,8) = dr(iri) * ns(isi) * dt(iti) !xz
         shp(i,9) = dr(iri) * ds(isi) * nt(iti) !yx

         shp(i,10) =dr3(iri) * ns(isi) * nt(iti)!x xx  ! xxx
         shp(i,11) = nr(iri) *ds3(isi) * nt(iti)!y yy  ! yyy
         shp(i,12) = nr(iri) * ns(isi) *dt3(iti)!z zz  ! zzz
         shp(i,13) = nr(iri) * ds(isi) *dt2(iti)!z zy  ! xx z 
         shp(i,14) =dr2(iri) * ns(isi) * dt(iti)!x xz  ! yy x
         shp(i,15) = dr(iri) *ds2(isi) * nt(iti)!y yx  ! zz y
         shp(i,16) = nr(iri) *ds2(isi) * dt(iti)!zy y  ! zy y
         shp(i,17) = dr(iri) * ns(isi) *dt2(iti)!xz z  ! xz z
         shp(i,18) =dr2(iri) * ds(isi) * nt(iti)!yx x  ! yx x
         shp(i,19) = dr(iri) * ds(isi) * dt(iti)!xyz   ! xyz
      enddo        

      end
