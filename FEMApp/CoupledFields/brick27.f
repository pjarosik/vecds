      subroutine brick27(ss,shp)
c     Calculate 1st & 2nd derivatives of shape functions for 3D brick element in global coords
c     History of this subroutine, Paweł Dłużewski (PD): 
c     1995 Horacio Antunez  + PD for 9-node 2D Lagrangian element, 
c     2005 Marcin Mazdziarz + PD extended to 3D, 
c     2012 Marcin Mazdziarz adopted the node ordering to that from FEAP81/83

      implicit none
      integer i,ir(27),is(27),it(27), iri,isi,iti 
      real*8  nr(3),dr(3),ns(3),ds(3),nt(3),dt(3),dr2(3),ds2(3),dt2(3),
     $        r,s,t,ss(3),shp(27,0:9)

      save     ir,is,it 

c      Ordering by vertex-edge-face-interior
c              1     4 5     8  9    12 13    16 17    20   21          27
cmm      data ir/1,2,2,1,1,2,2,1, 3,2,3,1, 3,2,3,1, 1,2,2,1, 1,2,3,3,3,3,3/
cmm      data is/1,1,2,2,1,1,2,2, 1,3,2,3, 1,3,2,3, 1,1,2,2, 3,3,2,1,3,3,3/
cmm      data it/1,1,1,1,2,2,2,2, 1,1,1,1, 2,2,2,2, 3,3,3,3, 3,3,3,3,1,2,3/
	  
c     Ordering from oryginal FEAP81/83 by vertex-edge-face-interior
c             1     4 5     8  9    12 13    16 17    20  21          27
      data ir/1,2,2,1,1,2,2,1, 3,2,3,1, 3,2,3,1, 1,2,2,1, 3,3,1,2,3,3,3/
      data is/1,1,2,2,1,1,2,2, 1,3,2,3, 1,3,2,3, 1,1,2,2, 3,3,3,3,1,2,3/
      data it/1,1,1,1,2,2,2,2, 1,1,1,1, 2,2,2,2, 3,3,3,3, 1,2,3,3,3,3,3/

c     Set 1-d shape functions for each local direction

      r = ss(1)
      s = ss(2)
      t = ss(3)
      nr(1) =  0.5d0 * r * (r - 1.d0)
      nr(2) =  0.5d0 * r * (r + 1.d0)
      nr(3) =  1.0d0 - r *  r

      ns(1) =  0.5d0 * s * (s - 1.d0)
      ns(2) =  0.5d0 * s * (s + 1.d0)
      ns(3) =  1.0d0 - s *  s

      nt(1) =  0.5d0 * t * (t - 1.d0)
      nt(2) =  0.5d0 * t * (t + 1.d0)
      nt(3) =  1.0d0 - t *  t

      dr(1) =  r - 0.5d0
      dr(2) =  r + 0.5d0
      dr(3) = -r - r
      dr2(1) =  1.0d0
      dr2(2) =  1.0d0
      dr2(3) = -2.0d0

      ds(1) =  s - 0.5d0
      ds(2) =  s + 0.5d0
      ds(3) = -s - s
      ds2(1) =  1.0d0
      ds2(2) =  1.0d0
      ds2(3) = -2.0d0

      dt(1) =  t - 0.5d0
      dt(2) =  t + 0.5d0
      dt(3) = -t - t
      dt2(1) =  1.0d0
      dt2(2) =  1.0d0
      dt2(3) = -2.0d0

c     Set local 3-d shape functions
c     shp(i,0) = N
c     shp1(i)=N/r,  shp2(i)=N/s,  shp3(i)=N/t
c     shp4(i)=N/rr, shp5(i)=N/ss, shp6(i)=N/tt, shp7(i)=N/st, shp8(i)=N/rt, shp9(i)=N/rs
      do i = 1,27
         iri = ir(i)
         isi = is(i)
         iti = it(i)
         shp(i,0) = nr(iri) * ns(isi) * nt(iti)
         
         shp(i,1) = dr(iri) * ns(isi) * nt(iti)
         shp(i,2) = nr(iri) * ds(isi) * nt(iti)
         shp(i,3) = nr(iri) * ns(isi) * dt(iti)    
               
         shp(i,4) =dr2(iri) * ns(isi) * nt(iti)
         shp(i,5) = nr(iri) *ds2(isi) * nt(iti)
         shp(i,6) = nr(iri) * ns(isi) *dt2(iti)
         shp(i,7) = nr(iri) * ds(isi) * dt(iti)
         shp(i,8) = dr(iri) * ns(isi) * dt(iti)
         shp(i,9) = dr(iri) * ds(isi) * nt(iti)
      enddo        

      end
