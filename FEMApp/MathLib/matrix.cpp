
#include "matrix.h"
#include <iomanip>
#include <sstream>

//Section matrix::memsection = createSection();

matrix::~matrix( void )
{
	Reset();
}

void matrix::Allocate( unsigned r, unsigned c, const matrix &m )
{
// data[ c * rows  + r ]
	#ifdef _DEBUG
				assert( ( r < rows ) && (c < cols ) );
	#endif

	unsigned k,l;
	for (k=0; k<m.GetRows(); k++)
		for (l=0; l<m.GetCols(); l++)
		{
			#ifdef _DEBUG
				assert( ( ( r + k ) < rows ) && ( ( c + l ) < cols ) );
			#endif
			 data[ (c + l) * rows + (r + k) ] += m(k,l);
		}
}

double matrix::Det() const
{
	if ( cols != rows ) return 0.0;

	if ( cols == 3 ) return data[0*rows+0]*(data[1*rows+1]*data[2*rows+2]-data[2*rows+1]*data[1*rows+2])+
							data[1*rows+0]*(data[2*rows+1]*data[0*rows+2]-data[0*rows+1]*data[2*rows+2])+
							data[2*rows+0]*(data[0*rows+1]*data[1*rows+2]-data[1*rows+1]*data[0*rows+2]);

	if ( cols == 2 )  return data[0*rows+0]*data[1*rows+1] - data[0*rows+1]*data[1*rows+0];

	return 0.0;
}

void matrix::sym33to66( matrix &u ) const
{
  const matrix &v = *this;
/*

     integer i,j,k,inxvs(3,3)
      real*8 u(6,6),v(3,3)
      save inxvs
      data inxvs/1,6,5,6,2,4,5,4,3/

*/
  int i,j,k;

  unsigned inxvs[3][3] = {
                          {0, 5, 4},
                          {5, 1, 3},
                          {4, 3, 2}
                      };


/*
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

 */
  for (i=0; i<3; i++)
     for (j=0; j<=i; j++)
     {
        k = inxvs[i][j];
        u(k,0) = v(i,0) * v(j,0);
        u(k,1) = v(i,1) * v(j,1);
        u(k,2) = v(i,2) * v(j,2);
        u(k,3) = v(i,3) * v(j,2) + v(i,2) * v(j,1);
        u(k,4) = v(i,4) * v(j,0) + v(i,0) * v(j,2);
        u(k,5) = v(i,5) * v(j,1) + v(i,1) * v(j,0);
     }

}

void matrix::eig3d( mvector &d, int &rot )
{
    matrix &v = *this;
/*
implicit  none

      integer   rot, its, i,j,k
      real*8    g,h, aij, sm,thresh, t, c,s,tau
      real*8    v(3,3), d(3), a(3), b(3), z(3)
*/

    int its,i,j,k;
    double g,h, aij, sm,thresh, t, c,s,tau;
    mvector a(3), b(3), z(3);

/*
c     Move array into one-d arrays

      a(1) = v(1,2)
      a(2) = v(2,3)
      a(3) = v(1,3)

      do i = 1,3
        d(i) = v(i,i)
        b(i) = d(i)
        z(i) = 0.0d0
        do j = 1,3
          v(i,j) = 0.0d0
        end do ! j
        v(i,i) = 1.d0
      end do ! i
*/

    a[0] = v(0,1);
    a[1] = v(1,2);
    a[2] = v(0,2);

    for (i=0; i<3; i++)
    {
      d[i] = v(i,i);
      b[i] = d[i];
      z[i] = 0.0;
      for (j=0; j<3; j++)
        v(i,j) = 0.0;
      v(i,i) = 1.0;
    }

/*

c     Check for diagonal case

      sm = abs(a(1)) + abs(a(2)) + abs(a(3))
      g  = abs(d(1)) + abs(d(2)) + abs(d(3))
      if (sm .lt. 1.d-13*g) return

*/
    sm = fabs(a[0]) + fabs(a[1]) + fabs(a[2]);
    g  = fabs(d[0]) + fabs(d[1]) + fabs(d[2]);
    if (sm < 1.0E-13 * g) return;
/*

      rot = 0
      do its = 1,50

c       Set convergence test and threshold

        sm = abs(a(1)) + abs(a(2)) + abs(a(3))
        if (sm.eq.0.0d0) return

        if(its.lt.4) then
          thresh = 0.011d0*sm
        else
          thresh = 0.0d0
        end if
*/
      rot = 0;
      for (its=0; its<50; its++)
      {

          sm = fabs(a[0]) + fabs(a[1]) + fabs(a[2]);
          if ( s == 0.0 ) return;

          if( its > 4) thresh = 0.011 * sm;
          else thresh = 0.0;

/*

c       Perform sweeps for rotations

        do i = 1,3
          j = mod(i,3) + 1
          k = mod(j,3) + 1
*/
       for (i=0; i<3; i++)
       {
           j = (i+1) % 3;
           k = (j+1) % 3;
/*
          aij  = a(i)
          g    = 100.d0*abs(aij)
          if(abs(d(i))+g.ne.abs(d(i)) .or. abs(d(j))+g.ne.abs(d(j))) then

            if(abs(aij).gt.thresh) then
              a(i) = 0.0d0
              h    = d(j) - d(i)
              if(abs(h)+g.eq.abs(h)) then
                t = aij/h
              else
                t = sign(2.d0,h/aij)/(abs(h/aij)+sqrt(4.d0+(h/aij)**2))
              endif

*/

           aij  = a[ i ];
           g    = 100.0 * fabs( aij );
           if ( fabs( d[ i ] ) + g != fabs( d[ i ] ) || fabs( d[ j ] ) + g != fabs( d [ j ] ) )
           {
             if( fabs( aij ) > thresh )
             {
               a[i] = 0.0;
               h    = d[j] - d[i];
               if( fabs( h ) + g == fabs(h) ) t = aij/h;
               else t =  h/aij > 0.0 ? 2.0 : -2.0 / ( fabs( h/aij ) + sqrt( 4.0+pow(h/aij,2)));
/*

c             Set rotation parameters

              c    = 1.d0/sqrt(1.d0+t*t)
              s    = t*c
              tau  = s/(1.d0+c)
*/
               c    = 1.0/sqrt(1.0+t*t);
               s    = t*c;
               tau  = s/(1.0+c);


/*
c             Rotate diagonal terms

              h    = t*aij
              z(i) = z(i) - h
              z(j) = z(j) + h
              d(i) = d(i) - h
              d(j) = d(j) + h

*/
               h    = t*aij;
               z[ i ] = z[ i ] - h;
               z[ j ] = z[ j ] + h;
               d[ i ] = d[ i ] - h;
               d[ j ] = d[ j ] + h;

/*
c             Rotate off-diagonal terms

              h    = a(j)
              g    = a(k)
              a(j) = h + s*(g - h*tau)
              a(k) = g - s*(h + g*tau)
*/
               h    = a[j];
               g    = a[k];
               a[j] = h + s*(g - h*tau);
               a[k] = g - s*(h + g*tau);

/*
c             Rotate eigenvectors

              do k = 1,3
                g      = v(k,i)
                h      = v(k,j)
                v(k,i) = g - s*(h + g*tau)
                v(k,j) = h + s*(g - h*tau)
              end do ! k
              rot = rot + 1
*/
               for (k=0; k<3; k++)
               {
                 g      = v(k,i);
                 h      = v(k,j);
                 v(k,i) = g - s*(h + g*tau);
                 v(k,j) = h + s*(g - h*tau);
               }
               rot++;

//            end if
           }

//          else a(i) = 0.0d0
           else a[i] = 0.0;

//          end if
       }

//        end do ! i
      }
/*
c       Update diagonal terms

        do i = 1,3
          b(i) = b(i) + z(i)
          d(i) = b(i)
          z(i) = 0.0d0
        end do ! i
*/

       for (i=0; i<3; i++)
       {
         b[i] = b[i] + z[i];
         d[i] = b[i];
         z[i] = 0.0;
       }

//    end do ! its
      }
/*

      end
 */


}

void matrix::adj3d( matrix &adj ) const
{
        const matrix &a = *this;

/*
c	Compute adjoint to the input matrix
c	  Input     a[3,3] - input matrix
c	  Output  adj[3,3] - adjoint to a

      real*8 a(3,3),adj(3,3)

c      adj(i,j) = -1**(i+j) * minor(j,i)
 */

    adj(0,0) =   a(1,1)*a(2,2) - a(1,2)*a(2,1);
    adj(1,0) = - a(1,0)*a(2,2) + a(1,2)*a(2,0);
    adj(2,0) =   a(1,0)*a(2,1) - a(1,1)*a(2,0);

    adj(0,1) = - a(0,1)*a(2,2) + a(0,2)*a(2,1);
    adj(1,1) =   a(0,0)*a(2,2) - a(0,2)*a(2,0);
    adj(2,1) = - a(0,0)*a(2,1) + a(0,1)*a(2,0);

    adj(0,2) =   a(0,1)*a(1,2) - a(0,2)*a(1,1);
    adj(1,2) = - a(0,0)*a(1,2) + a(0,2)*a(1,0);
    adj(2,2) =   a(0,0)*a(1,1) - a(0,1)*a(1,0);

}

void matrix::inv3d( matrix &b ) const
{

/*
    implicit none

   c	Oblicza macierz odwrotna do macierzy a
   c	  Input   a[3,3] - macierz oryginalna
   c	  Output  b[3,3] - macierz odwrocona

         integer i,j
         real*8 deta
         real*8 a(3,3),b(3,3),adj(3,3)
*/
        int i,j;
        double deta;
        matrix a(3,3),adj(3,3);


/*


   c     Compute adjoint to a: adj(i,j) = -1**(i+j) *minor(j,i)

         call adj3d(a,adj)
*/
        a.adj3d( adj );
/*

   c     Compute determinant of a

         deta  = a(1,1)*adj(1,1) + a(1,2)*adj(2,1) + a(1,3)*adj(3,1)

*/

         deta  = a(0,0)*adj(0,0) + a(0,1)*adj(1,0) + a(0,2)*adj(2,0);

/*

   c     Compute inverse to a

         deta = 1.d0/deta
         do j = 1,3
           do i = 1,3
             b(i,j) = adj(i,j)*deta
           end do
         end do

         return
         end
*/

         deta = 1.0/deta;
         for (j=0; j<3; j++)
           for (i=0; i<3; i++)
             b(i,j) = adj(i,j)*deta;

}

void matrix::polard3d( matrix &r, matrix &u ) const
{

    //    c     Polar decomposition of 3d f into r * u
    //    c       r - ortogonal matrix
    //    c       u - symmetric matrix


          //integer i,j,k
          //real*8 rot,f(3,3),r(3,3),u(3,3),uinv(3,3),ulam(3),uu(3,3)

        int rot;
        unsigned i,j,k;
        matrix uinv(3,3), uu(3,3);
        const matrix &f = *this;
        mvector ulam(3);
    /*
          do i=1,3
             do j=1,3
                uu(i,j) = 0.d0
                do k=1,3
                   uu(i,j) = uu(i,j) + f(k,i)*f(k,j)
                enddo
             enddo
          enddo
    */

        for (i=0; i<3; i++)
            for (j=0; j<3; j++)
            {
                uu(i,j) = 0.0;
                for(k=0; k<3; k++)
                {
                    uu(i,j) = uu(i,j) + f(k,i)*f(k,j);
                }
            }

     uu.eig3d(ulam,rot);
     uu.eig3d( ulam, rot );

/*
          do i=1,3
             ulam(i) = dsqrt(ulam(i))
          enddo
*/
          for (i=0; i<3; i++)
             ulam[ i ] = sqrt( ulam[ i ] );

 /*

          do i = 1,3
             do j = 1,3
                u(i,j) = 0.d0
            do k = 1,3
                   u(i,j) = u(i,j) + ulam(k) * uu(i,k) * uu(j,k)
            enddo
             enddo
          enddo
*/

          for (i=0; i<3; i++)
             for (j=0; j<3; j++)
             {
                u(i,j) = 0.0;
                for (k=0; k<3; k++)
                    u(i,j) = u(i,j) + ulam[k] * uu(i,k) * uu(j,k);
             }


/*

          call inv3d(u,uinv)

          r = Matmul(f,uinv);

          return
          end

        */

       u.inv3d(uinv);
       r = f * uinv;

}

void matrix::Inverse( )
{
	if (cols!=rows) return;
	int n=cols,i,j,l,M[100],k=0;
	double maxA,d,e,eps=1e-20;
	for (i=0; i<n; i++)
	{
		maxA=0.0;
		for (j=0; j<n; j++)
		{
			d=data[i*rows+j];
			if ( fabs(maxA) < fabs(d) ) { maxA=d; k=j; }
		}
		if ( fabs(maxA) < eps ) return;
		M[i]=k;
		data[i*rows+k]=1.0;
		for (j=0; j<n; j++) { d=data[j*rows+k]/maxA; data[j*rows+k]=data[j*rows+i]; data[j*rows+i]=d; }
		for (j=0; j<n; j++)
		{
			if (j!=i) {
				d=data[i*rows+j]; data[i*rows+j]=0.0;
				for (l=0; l<n; l++)
				{
					e=d*data[l*rows+i];
					data[l*rows+j]-=e;
				}
			}
		}
	}
	for (i=n-1; i>=0; i--)
	{
		k=M[i];
		if (k!=i)
		{
			for (j=0; j<n; j++)
			{
				d=data[i*rows+j];
				data[i*rows+j]=data[k*rows+j];
				data[k*rows+j]=d;
			}
		}
	}
    return;
}

void matrix::print( const string &label ) const
{
	unsigned k, l;
	ofstream file("matrix_file_dump.txt", ios::app );
	file.setf( ios::scientific, ios::floatfield );
	file.setf( ios::right );
	file.precision( 7 );
	file << "name :" << label << endl;
	for (k=0; k<rows; k++)
	{
		for (l=0; l<cols; l++)
			file << setw(18) << data[l*rows+k];
		file << endl;
	}
}

mvector::~mvector( ) { Reset(); }

string mvector::toString() const
{
    stringstream ss;
    ss << "<";
    unsigned k;
    for ( k=0;  k<GetDim();  k++ )
    {
        ss << this->operator [](k);
        if (k) ss << ',';
    }
    ss << ">";
    return ss.str();
}
