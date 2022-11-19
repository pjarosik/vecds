      subroutine adj3d(a,adj)

      implicit none

c	Compute adjoint to the input matrix
c	  Input     a[3,3] - input matrix 
c	  Output  adj[3,3] - adjoint to a 
      
      real*8 a(3,3),adj(3,3)

c      adj(i,j) = -1**(i+j) * minor(j,i)

      adj(1,1) =   a(2,2)*a(3,3) - a(2,3)*a(3,2)
      adj(2,1) = - a(2,1)*a(3,3) + a(2,3)*a(3,1)
      adj(3,1) =   a(2,1)*a(3,2) - a(2,2)*a(3,1)

      adj(1,2) = - a(1,2)*a(3,3) + a(1,3)*a(3,2)
      adj(2,2) =   a(1,1)*a(3,3) - a(1,3)*a(3,1)
      adj(3,2) = - a(1,1)*a(3,2) + a(1,2)*a(3,1)

      adj(1,3) =   a(1,2)*a(2,3) - a(1,3)*a(2,2)
      adj(2,3) = - a(1,1)*a(2,3) + a(1,3)*a(2,1)
      adj(3,3) =   a(1,1)*a(2,2) - a(1,2)*a(2,1)

      return
      end
