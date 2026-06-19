c     -----------------------------------------------------------------
c     displace
c
c     Example routine computing a displacement field 'u' for a set of
c     atoms given a set of dislocations. This is a placeholder example
c     that may be replaced by a full elasticity solution later.
c
c     Arguments (all double precision unless noted):
c       n_dis    (integer, in)  number of dislocations
c       n_atoms  (integer, in)  number of atoms
c       d_pos    (3, n_dis,  in)  dislocation positions (one per column)
c       d_b      (3, n_dis,  in)  Burger's vectors      (one per column)
c       d_plane  (3, n_dis,  in)  glide-plane normals   (one per column)
c       coords   (3, n_atoms,in)  atom coordinates      (one per column)
c       u        (3, n_atoms,out) computed displacements(one per column)
c     -----------------------------------------------------------------
      subroutine displace(n_dis, n_atoms, d_pos, d_b, d_plane,
     &                    coords, u)
      implicit none
      integer n_dis, n_atoms
      double precision d_pos(3, n_dis)
      double precision d_b(3, n_dis)
      double precision d_plane(3, n_dis)
      double precision coords(3, n_atoms)
      double precision u(3, n_atoms)

      integer i, k, half

c     Print all dislocations (position, Burger's vector, glide plane).
      write(*,*) 'displace: n_dis = ', n_dis, ', n_atoms = ', n_atoms
      do k = 1, n_dis
         write(*,*) 'dislocation ', k
         write(*,*) '  d_pos   = ', d_pos(1, k), d_pos(2, k),
     &                              d_pos(3, k)
         write(*,*) '  d_b     = ', d_b(1, k), d_b(2, k),
     &                              d_b(3, k)
         write(*,*) '  d_plane = ', d_plane(1, k), d_plane(2, k),
     &                              d_plane(3, k)
      end do

c     Simple example: move the first half of the atoms to the left by 3
c     (along x) and the remaining atoms to the right by 3.
      half = n_atoms / 2
      do i = 1, n_atoms
         if (i .le. half) then
            u(1, i) = -30.0d0
         else
            u(1, i) =  30.0d0
         end if
         u(2, i) = 0.0d0
         u(3, i) = 0.0d0
      end do

      return
      end
