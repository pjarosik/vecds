// LargeMatrixSkyLineCholesky.cpp: implementation of the CLargeMatrixSkyLineCholesky class.
//
//////////////////////////////////////////////////////////////////////

#include "LargeMatrixSkyLineCholesky.h"
#include <iostream>
//#include "mathdef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CLargeMatrixSkyLineCholesky::CLargeMatrixSkyLineCholesky()
{

}

CLargeMatrixSkyLineCholesky::~CLargeMatrixSkyLineCholesky()
{
   
}

bool CLargeMatrixSkyLineCholesky::Decompose( CProgress *prg )
{
	  int     i, i1, j, k, pr;
	  double  r;

	  if ( prg ) prg->SetRange( dim );

	  dec_time.Start();

//    if ( !PositiveDiagonal() ) return false;
//      print();

      for (i=0; i<(int)dim; i++)
      {
		if (prg) prg->NewValue(i);
	  	i1 = 1;
	 	r  = 0.0;
		for ( k = 1; k < (int)prf[ i ]; k++ ) r += mat[ i ][ k ] * mat[ i ][ k ];
		if  ( mat[ i ][ 0 ] - r < 0.0 )
        {
            return false;
        }

		mat[ i ][ 0 ] = sqrt( mat[ i ][ 0 ] - r );

		for ( j = i + 1 ; j < (int)dim; j++)
           if ( ( j - i ) < (int)prf[ j ] )
           {
			  r  = 0.0;
			  pr = min(  prf[ j ] + i - j, prf[ i ]  );
			  for ( k = 1; k < pr; k++ )
				  r += mat[ j ][ k + j - i ] * mat[ i ][ k ];

			  if ( mat[i][0] == 0.0 )
              {
                  return false;
              }
			  mat[ j ][ j - i ] = ( mat[ j ][ j - i ] - r ) / mat[ i ][ 0 ];
           };
      };

	  dec_time.Stop();
        cout << "solution time =" << dec_time.GetFormattedTime() << endl;

//	   print();
	  return true;
}

bool CLargeMatrixSkyLineCholesky::PositiveDiagonal( )
{
    unsigned k;
    bool non = false;
    for ( k=0; k<dim; k++)
           if ( ( mat[ k ][ 0 ] < 0 ) || ( fabs( mat[ k ][ 0 ] ) < 1e-10 ) )
           {
//               KN.OM.MM.GetConsoleOutput() << "Nonpositive value on diagonal :k=" << k << "    diag(k)=" << mat[k][0] << "\n";
               non=true;
           }

//    KN.CheckConstrainedNodes( );
    if ( non )
    {
//        print();
//        assert( 0 );
    }
    return true;
}
