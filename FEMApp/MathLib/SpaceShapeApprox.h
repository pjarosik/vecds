#ifndef SPACESHAPEAPPROX_H
#define SPACESHAPEAPPROX_H
#include "SpaceShape.h"
#include <vector>

template < class C >
class CSpaceShapeApprox : public CSpace
{
public:
    CSpaceShapeApprox( const CSpaceShape &sp, const C &nd, CSpace *s=0 );
    CSpaceShapeApprox( const CSpaceShapeApprox &s ):CSpace( s ),values( s.values ), ssp( s.ssp ) {  }
    //CSpaceShapeApprox( CCopyer &cpr, const CSpaceShapeApprox &s ):CSpace( cpr, s ),values( s.values ), ssp( s.ssp ) {  }
    CLONE_OBJECT(CSpaceShapeApprox<C>)
//    COPY_OBJECT(CSpaceShapeApprox<C>)

    //! CShapeFunctions destructor
    /*!

    */
    virtual ~CSpaceShapeApprox(void) {  }

    //! Shape function values
    /*!
      \param xi a local coordinates of a point
      \param sfn vector of shape function values at given point. (output)
    */
    virtual void transform( const mvector &x, mvector &X ) const;

    //! Shape function gradient
    /*!
      \param xi a local coordinates of a point
      \param gsfn matrix of shape functions gradients at given point. (output)
    */
    virtual void jacobian( const mvector &x, matrix &X ) const;

protected:

    const CSpaceShape &ssp;
    const C &values;

};


template < class C >
CSpaceShapeApprox<C>::CSpaceShapeApprox( const CSpaceShape &sp1, const C &nd,  CSpace *sp ):CSpace( sp1.Getxdim(), 1, sp ),ssp( sp1 ), values( nd )
{
    dg = ssp.GetDegree();
}

template < class C >
void CSpaceShapeApprox<C>::transform( const mvector &x,mvector &X ) const
{
    unsigned k;
    X.SetDim( Xdim );
    mvector N( ssp.GetXdim() );
    ssp.transform(x,N);
    for (k=0; k<N.GetDim(); k++)
        X[0] += N[ k ] * values[ k ] ;
}

template < class C >
void CSpaceShapeApprox<C>::jacobian( const mvector &x, matrix &J ) const
{
    matrix J1(Xdim,xdim);
    unsigned k,l;
    for (l=0; l<xdim; l++)
          J1(1,l) = values[l];
    ssp.jacobian(x,J);
    J = J * J1;
}


#endif // SPACESHAPEAPPROX_H
