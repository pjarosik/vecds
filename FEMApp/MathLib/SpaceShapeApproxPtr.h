#ifndef SPACESHAPEAPPROXPTR_H
#define SPACESHAPEAPPROXPTR_H

#include "SpaceShape.h"

template < class T >
class CSpaceShapeApproxPtr : public CSpace
{
public:
        CSpaceShapeApproxPtr( const CSpaceShape &ssp, const vector< T* > &nd, unsigned odim, CSpace *sp = 0 );
        CSpaceShapeApproxPtr( const CSpaceShapeApproxPtr<T> &s ):CSpace( s ),ssp( s.ssp ),values( s.values ) {  }
        // CSpaceShapeApproxPtr( CCopyer &cpr, const CSpaceShapeApproxPtr<T> &s );
        CLONE_OBJECT( CSpaceShapeApproxPtr<T> )
//        COPY_OBJECT( CSpaceShapeApproxPtr<T> )

            //! CShapeFunctions destructor
            /*!

            */
        virtual ~CSpaceShapeApproxPtr( void ) {  }

            //! Shape function values
            /*!
              \param xi a local coordinates of a point
              \param sfn vector of shape function values at given point. (output)
            */
        virtual void transform( const mvector &N, mvector &X ) const;

            //! Shape function gradient
            /*!
              \param xi a local coordinates of a point
              \param gsfn matrix of shape functions gradients at given point. (output)
            */
        virtual void jacobian( const mvector &ksi, matrix &X ) const;

        const vector<T*>& GetPoints() const { return values; }
        const CSpaceShape& GetShapeSpace() const { return ssp; }

protected:

        const CSpaceShape &ssp;
        const vector<T*> values;

};


template < class T >
CSpaceShapeApproxPtr<T>::CSpaceShapeApproxPtr( const CSpaceShape &sp1, const vector<T*> &nd,  unsigned odim, CSpace *sp ):CSpace( sp1.Getxdim(), odim, sp ), ssp( sp1 ), values( nd )
{
    dg = ssp.GetDegree();
}

/*
template < class T >
CSpaceShapeApproxPtr<T>::CSpaceShapeApproxPtr( CCopyer &cpr, const CSpaceShapeApproxPtr<T> &s ):CSpace( cpr, s ),ssp( s.ssp ),values( cpr.Copy( s.values ) )
{

}
*/
template < class T >
void CSpaceShapeApproxPtr<T>::transform( const mvector &ksi,mvector &X ) const
{
    unsigned k;
    X.SetDim( Xdim );
    mvector N( ssp.GetXdim() );
    ssp.transform(ksi,N);
    for (k=0; k<values.size(); k++)
            X += N[ k ] * (*values[ k ]);

}

template < class T >
void CSpaceShapeApproxPtr<T>::jacobian( const mvector &ksi, matrix &J ) const
{
    matrix J1(Xdim,xdim);
    unsigned k,l;
    for (k=0; k<Xdim; k++)
        for (l=0; l<xdim; l++)
            J1(k,l) = (*values[l])[k];
    ssp.jacobian(ksi,J);
    J = J * J1;
}

#endif // SPACESHAPEAPPROXPTR_H
