#include "StatementSpace.h"
#include "../MathLib/SpacePolar.h"
#include "../MathLib/SpaceRectangular.h"
#include "../MathLib/SpaceCylindrical.h"

CStatementSpace::CStatementSpace( CFEMProject &fp ):FP( fp ),params_par( string("params"),true, false )
{
}

double CStatementSpace::FindParam( const string &nm )
{
    unsigned k;
    for ( k=0; k<params_par.GetParam().GetList().size(); k++ )
      if ( params_par.GetParam().GetList()[k].GetName() == nm )
          return params_par.GetParam().GetList()[k].GetParam();

    throw CFEException( "Missing parameter '" + nm + "'" );
    return sqrt(-1.0);
}

void CStatementSpace::CStatementSpace::Execute( istream &i )
{
    TokenParam  name_par( string("name") ),
                base_par( string("basetype") ),
                space_par( string("space"), true, false );


    i >> name_par >> base_par >> space_par >> params_par;

    CSpace *spname = FP.FindSpace( name_par.GetParam().toString() ), *sp = 0;
    if ( space_par.IsRead() )
    {
        sp = FP.FindSpace( space_par.GetParam().toString( ) );
        if ( !sp ) throw CLangException(&i, "Space named '" + space_par.GetParam().toString() + "' not found" );
    }
    if ( spname ) throw CLangException(&i, "Space named '" + name_par.GetParam().toString() + "' already exists" );


    CSpace *space = 0;
    unsigned k;
    const string basetype = base_par.GetParam().toString();
    if ( basetype == "rect2D" )
    {
        mvector xs(2);
        xs[0] = FindParam("xs");
        xs[1] = FindParam("ys");
        space = new CSpaceRectangular(xs, sp ? sp->DeepClone() : 0 );
    }
    else if ( basetype == "rect3D" )
    {
        mvector xs(3);
        xs[0] = FindParam("xs");
        xs[1] = FindParam("ys");
        xs[2] = FindParam("zs");
        space = new CSpaceRectangular(xs, sp ? sp->DeepClone() : 0 );
    }
    else if ( basetype == "polar" ) {
        mvector xs(2);
        space = new CSpacePolar(xs,FindParam("f"), sp ? sp->DeepClone() : 0 );
    }
    else if ( basetype == "cylindrical" ) {
        space = new CSpaceCylindrical( sp ? sp->DeepClone() : 0 );
    }
    else throw CFEException( "Unknown space type '" + basetype + "' not found" );

    FP.AddSpace( name_par.GetParam().toString(), space );
}


