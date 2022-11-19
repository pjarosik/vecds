#ifndef RESULTAPPROX_H
#define RESULTAPPROX_H
#include "FENode.h"
#include "../MathLib/SpaceShapeApproxPtr.h"
#include "../MathLib/SpaceShapeApprox.h"

class CResultApprox : public CSpaceShapeApproxPtr<CFENode>
{
public:

    CResultApprox( const CSpaceShapeApproxPtr< CFENode > &sa, const string &rn );
    CResultApprox( const CResultApprox &r ):CSpaceShapeApproxPtr<CFENode>( r ), sa( r.sa ), ra( r.ra->Clone() ), rname( r.rname ), nv( r.nv ) {  }
    // CResultApprox( CCopyer &cpr, const CResultApprox &r ):CSpaceShapeApproxPtr<CFENode>( cpr, r ), sa( r.sa ), ra( cpr.Copy( r.ra->Clone() ) ), rname( r.rname ), nv( r.nv ) {  }
    virtual ~CResultApprox() { delete ra; }
    CLONE_OBJECT( CResultApprox )
//    COPY_OBJECT( CResultApprox )

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

    //! Chechs if initialization finished correctly. If all nodes has given results name.
    /*!

    */
    bool IsValid() const { return ra != 0; }


protected:

    const CSpaceShapeApproxPtr< CFENode > &sa;
    CSpaceShapeApprox< vector<double> > *ra;
    const string rname;
    vector< double > nv;

};

#endif // RESULTAPPROX_H
