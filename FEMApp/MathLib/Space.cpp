
#include "Space.h"

CSpace::CSpace(unsigned a, unsigned b, CSpace *sp,bool ro):space(sp), xdim(a), Xdim(b), dg(-1)
{
    #ifdef _DEBUG
        if ( space ) assert( Xdim == space->Getxdim() );
    #endif
}

CSpace::CSpace( const CSpace &s ):xdim( s.xdim ), Xdim( s.Xdim ),space( s.space ),dg( s.dg )
{ 

}

/* CSpace::CSpace( CCopyer &cpr, const CSpace &s ):xdim( s.xdim ), Xdim( s.Xdim ),space( s.space ),dg( s.dg )
{

}*/

CSpace::~CSpace( void )
{

}

CSpace* CSpace::DeepClone( )
{
    if ( space == 0 ) return Clone();
    CSpace *ps = space;
    space = space->DeepClone();
    CSpace *ts = Clone();
    space = ps;
    return ts;
}

void CSpace::transformFrom( const mvector &x,  mvector &X, CSpace *sp ) const
{
	#ifdef _DEBUG
		assert( x.GetDim() == xdim ); 
		assert( Xdim ); 
		if ( space == 0 ) 
			assert( sp == 0 ); 
	#endif

	transform( x, X );
	if ( space && space != sp ) 
	{
		mvector temp( X );
		space->transformFrom( temp, X, sp );
	}

}

void CSpace::Jacobian( const mvector &x, matrix &J ) const
{

	#ifdef _DEBUG
				assert( x.GetDim() != xdim ); 
	#endif

	matrix j( Xdim, Xdim );
	J.SetDim( xdim, Xdim );
	jacobian( x, j );
	if ( space ) 
	{
		space->Jacobian( x, J );
		J = J * j;
	}
	else J = j;

}

void CSpace::Transform( const mvector &x,  mvector &X )const 
{
	#ifdef _DEBUG
		assert( x.GetDim() == xdim ); 
	#endif
	transform(x,X);
	mvector xs(X);
	if ( space ) space->Transform(xs,X);
}


void CSpace::InverseTransform( const mvector &X, mvector &x, double eps )const
{
    #ifdef _DEBUG
        assert( X.GetDim() == Xdim );
        assert( x.GetDim() == xdim );
    #endif

        mvector Xp( Xdim ), dX( Xdim ), dxi( xdim );
        matrix J1( xdim, Xdim );
        double norm = 1.0;

        while( norm > eps )
        {
            Xp = 0;
            Transform( x, Xp );
            dX = X - Xp;
            Jacobian( x, J1 );
            J1.Inverse( );
            dxi = J1 * dX;
            x += dxi;
            norm = dxi.GetNorm();
        }


}
