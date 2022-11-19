#ifndef SPACEMORPH1D_H
#define SPACEMORPH1D_H
#include "Space.h"

class CSpaceMorph1D : public CSpace
{
    public:

        CSpaceMorph1D( const CSpace *s1, const CSpace *s2, CSpace *s=0 );
        CSpaceMorph1D( const CSpaceMorph1D &sp ):CSpace( sp ), S1( sp.S1 ), S2( sp.S2 ) {   }
        //CSpaceMorph1D( CCopyer &cpr, const CSpaceMorph1D &sp ):CSpace( sp ), S1( cpr.Copy ( sp.S1 ) ), S2( cpr.Copy( sp.S2 ) ) {   }
        virtual ~CSpaceMorph1D( void );

        CLONE_OBJECT( CSpaceMorph1D )
        //COPY_OBJECT( CSpaceMorph1D )

        virtual void transform( const mvector &x,  mvector &X )const;
        virtual void jacobian( const mvector &x,  matrix &j )const;

    protected:

        const CSpace *S1, *S2;
};

#endif // SPACEMORPH1D_H
