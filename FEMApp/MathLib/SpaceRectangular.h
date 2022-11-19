#ifndef CSPACERECTANGULAR_H
#define CSPACERECTANGULAR_H
#include "../MathLib/Space.h"
#include "../MathLib/matrix.h"

class CSpaceRectangular : public CSpace
{
public:
    CSpaceRectangular( const mvector &x, CSpace *sp = 0 );
    CSpaceRectangular( const CSpaceRectangular &r ):CSpace( r ),xs( r.xs ) {  }
    //CSpaceRectangular( CCopyer &cpr, const CSpaceRectangular &r ):CSpace( r ),xs( r.xs ) {  }
    virtual ~CSpaceRectangular( void ) { }

    CLONE_OBJECT( CSpaceRectangular )
    // COPY_OBJECT( CSpaceRectangular )

protected:

    mvector xs;
    virtual void transform( const mvector &x,  mvector &X ) const;
    virtual void jacobian( const mvector &x,  matrix &j ) const;

};

#endif // CSPACERECTANGULAR_H
