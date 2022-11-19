#ifndef SPACETRANSLATION_H
#define SPACETRANSLATION_H
#include "Space.h"

class CSpaceTranslation : public CSpace
{
    public:
        CSpaceTranslation( const mvector &a, CSpace *sp );
        //CSpaceTranslation( CCopyer &cpr, const CSpaceTranslation &sp ):CSpace( sp.xdim, sp.Xdim ) {  }
        virtual ~CSpaceTranslation(void);

        CLONE_OBJECT(CSpaceTranslation)
        //COPY_OBJECT(CSpaceTranslation)

        virtual void transform( const mvector &x,  mvector &X )const;
        virtual void jacobian( const mvector &x,  matrix &j )const;
protected:
        mvector dx;
};

#endif // SPACETRANSLATION_H
