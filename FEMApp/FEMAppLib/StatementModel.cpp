#include "StatementModel.h"
#include "operators.h"
#include "../MathLib/SpaceShapeLagrange2N_1D_2.h"
#include "../MathLib/SpaceShapeLagrange3N_1D_3.h"
#include "../MathLib/SpaceShapeLagrange4N_1D_4.h"
#include "../MathLib/SpaceShapeLagrange2N_2D_4.h"
#include "../MathLib/SpaceShapeLagrange3N_2D_9.h"
#include "../MathLib/SpaceShapeLagrange4N_2D_16.h"
#include "../MathLib/SpaceShapeLagrange2N_3D_8.h"
#include "../MathLib/SpaceShapeLagrange3N_3D_27.h"
#include "../MathLib/SpaceShapeLagrange4N_3D_64.h"
#include "../MathLib/GeometricObjectMorph.h"
#include "../MathLib/SpaceTranslation.h"
#include "../MathLib/SpacePolar.h"
#include "../MathLib/SpaceRectangular.h"
#include "../MathLib/SpaceCylindrical.h"


CStatementModel::CStatementModel( CFEMProject &f ):FP( f ), model( 0 ), current_object( 0 ), params_par( string("params"),true, false )
{
}


CStatementModel::~CStatementModel( void )
{
}

void CStatementModel::Project( const DoubleVector &dv, mvector &x )
{
    x.SetDim( dv.GetList().size() );
    unsigned k;
    for (k=0; k<dv.GetList().size(); k++)
        x[ k ] = dv.GetList()[ k ];
}


void CStatementModel::Execute( istream &i )
{
    TokenParamDouble eps((string)"eps",true,false);
    i >> eps;
    model = &FP.GetModel();
    if ( eps.IsRead() ) model->SetEps( eps.GetParam() );
    ReadObject( model->GetObject(), i, 0 );
}

bool CStatementModel::ReadPoints( CModelObject &co, const PointVariantList &pvl, vector < CGeometricObjectPoint* > &pts, CSpace *sp )
{
    unsigned k;
    mvector x;
    for (k=0; k<pvl.GetList().size(); k++ )
    {
        switch ( pvl.GetList()[ k ].WhichRead( ) )
        {
            case -1:    return false;
            case  0:    x = FP.EvaluateExpressionVector( pvl.GetList()[k].GetFirst().GetList() );
                        pts.push_back( AddPoint( co, x, "", sp ) ); break;
            case  1:    pts.push_back( co.FindPoint(  pvl.GetList()[ k ].GetSecond().toString()  ) ); break;
        }
    }

   return true;
}


CGeometricObjectPoint* CStatementModel::AddPoint( CModelObject &co, const mvector &xp, const string name, CSpace *sp )
{
    mvector Xp=xp;
    if ( sp ) sp->Transform( xp, Xp );
    CGeometricObjectPoint *pt = model->FindPoint( Xp, false );
    if ( pt == 0 )
    {
        pt = new CGeometricObjectPoint( xp, sp );
        co.AddGeomObject( name, pt );
    }
    return pt;
}

void CStatementModel::AddShapeObject( CModelObject &co, const string  &name, CSpaceShape *sfn, const vector< CGeometricObjectPoint* > &points, CSpace *sp )
{
    model->StoreSpace( sfn );
    if ( points.size() != sfn->GetXdim() ) throw CFEException( "AddShapeObject - mismatch number of nodes");
    CSpaceShapeApproxPoint *ssa = new CSpaceShapeApproxPoint( *sfn, points , points[ 0 ]->Getx().GetDim(), sp );
    model->StoreSpace( ssa );
    CGeometricObject *obj = new CGeometricObjectShape< CGeometricObjectPoint >( model->GetDomain(), ssa );
    co.AddGeomObject( name, obj );
}

void CStatementModel::AddLine( CModelObject &co, const string &name, const string type, vector < CGeometricObjectPoint* > &points, CSpace *sp )
{
    CSpaceShape *ssp=0;
    if ( type == "" ) ssp = new CSpaceShapeLagrange2N_1D_2;
    else if ( type == "Lg2" ) ssp = new CSpaceShapeLagrange2N_1D_2;
    else if ( type == "Lg3" ) ssp = new CSpaceShapeLagrange3N_1D_3;
    else if ( type == "Lg4" ) ssp = new CSpaceShapeLagrange4N_1D_4;
    else throw CFEException( "Unknown line type '" + type + "'");
    AddShapeObject( co, name, ssp, points, sp );
}

void CStatementModel::AddSurface( CModelObject &co, const string &name, const string type, vector < CGeometricObjectPoint* > &points, CSpace *sp )
{
    CSpaceShape *ssp=0;
    if ( type == "" ) ssp = new CSpaceShapeLagrange2N_2D_4;
    else if ( type == "Lg4" ) ssp = new CSpaceShapeLagrange2N_2D_4;
    else if ( type == "Lg9" ) ssp = new CSpaceShapeLagrange3N_2D_9;
    else if ( type == "Lg16" ) ssp = new CSpaceShapeLagrange4N_2D_16;
    else throw CFEException( "Unknown surface type '" + type + "'");
    AddShapeObject( co, name, ssp, points, sp );
}

void CStatementModel::AddSolid( CModelObject &co, const string &name, const string type, vector < CGeometricObjectPoint* > &points, CSpace *sp )
{
    CSpaceShape *ssp=0;
    if ( type == "" ) ssp = new CSpaceShapeLagrange2N_3D_8;
    else if ( type == "Lg8" ) ssp = new CSpaceShapeLagrange2N_3D_8;
    else if ( type == "Lg27" ) ssp = new CSpaceShapeLagrange3N_3D_27;
    else if ( type == "Lg64" ) ssp = new CSpaceShapeLagrange4N_3D_64;
    else throw CFEException( "Unknown solid type '" + type + "'");
    AddShapeObject( co, name, ssp, points, sp );
}

void CStatementModel::AddRect( CModelObject &co, const string &name, vector < CGeometricObjectPoint* > &points, CSpace *sp )
{
    if ( points.size() == 2 )
    {
        vector < CGeometricObjectPoint* > pts( 4 );
        pts[0] = points[0];
        pts[3] = points[1];
        mvector x2(2), x3(2);
        const mvector x1 = points[0]->Getx();
        const mvector x4 = points[1]->Getx();
        x2[0] = x4[0]; x2[1] = x1[1];
        x3[0] = x1[0]; x3[1] = x4[1];
        pts[1] = AddPoint( co, x2, "", sp );
        pts[2] = AddPoint( co, x3, "", sp );
        AddSurface( co, name, "", pts, sp );
        return;
    }
    AddSurface( co, name, "", points, sp );
}

void CStatementModel::AddCube( CModelObject &co, const string &name, vector < CGeometricObjectPoint* > &points, CSpace *sp )
{
    if ( points.size() == 2 )
    {
        vector < CGeometricObjectPoint* > pts( 8 );
        const mvector xmin = points[0]->Getx();
        const mvector xmax = points[1]->Getx();

        pts[0] = points[0];
        pts[7] = points[2];

        vector< mvector > xp( 8, mvector( 3 ) );

        xp[1][0] = xmax[ 0 ]; xp[1][1] = xmin[ 1 ]; xp[1][2] = xmin[ 2 ];
        xp[2][0] = xmin[ 0 ]; xp[2][1] = xmax[ 1 ]; xp[2][2] = xmin[ 2 ];
        xp[3][0] = xmax[ 0 ]; xp[3][1] = xmax[ 1 ]; xp[3][2] = xmin[ 2 ];
        xp[4][0] = xmin[ 0 ]; xp[4][1] = xmin[ 1 ]; xp[4][2] = xmax[ 2 ];
        xp[5][0] = xmax[ 0 ]; xp[5][1] = xmin[ 1 ]; xp[5][2] = xmax[ 2 ];
        xp[6][0] = xmin[ 0 ]; xp[6][1] = xmax[ 1 ]; xp[6][2] = xmax[ 2 ];

        unsigned k;
        for(k=1; k<=6; k++)  pts[k] = AddPoint( co, xp[k], "", sp );
        AddSurface( co, name,"", pts, sp );
        return;
    }
    AddSurface( co, name, "", points,sp );
}

void CStatementModel::ReadObject( CModelObject &co, istream &i, CSpace *space, const string &basename )
{
    char c;
    i >> c;
    if ( c != '{' ) throw CFEException( "'{' is expected at the beginning of model or list" );

    while( true )
    {
        CLangToken< IsValidToken > token;
        i >> token;
        const string &strtoken = token.toString();

        if ( strtoken == "space" ) {

             CSpace *newspace = ReadSpace( i );
             newspace->SetSpace(space);
             ReadObject( co, i, newspace );
        }
        else if ( strtoken == "object" ) {
             TokenParam param_name( (string)"name", true, false ), param_space( (string)"space", true, false );
             i >> param_name >> param_space;
             CSpace *space = param_space.IsRead() ? FP.FindSpace( param_space.GetParam().toString() ) : 0;
             CModelObject *object = new CModelObject( param_name.GetParam().toString(), model, space );
             ReadObject( *object, i, space );
        }
        else if ( strtoken == "point" )
        {
            ExpressionVectorParameter x;
            i >> x;
            AddPoint( co, FP.EvaluateExpressionVector( x.GetParam().GetList() ), x.GetName().toString(), space );
        }
        else if ( strtoken == "points" )
        {
            ExpressionVectorParameterList pts;
            i >> pts;
            unsigned k;
            for (k=0; k<pts.GetList().size(); k++)
                AddPoint( co, FP.EvaluateExpressionVector( pts.GetList()[ k ].GetParam().GetList() ), pts.GetList()[k].GetName().toString(), space );

        }
        else if ( strtoken == "line" )
        {
            CLangToken< IsValidToken > type( string("type") );
            PointVariantListParameter p;
            vector < CGeometricObjectPoint* > pts;
            i >> type >> p;
            ReadPoints( co, p, pts, space );
            if ( type.IsRead() ) AddLine( co, p.GetName().toString(), type.toString(), pts, space );
            else AddLine( co, p.GetName().toString(), "", pts, space );
        }
        else if ( strtoken == "surface" )
        {
            CLangToken< IsValidToken > type( string("type") );
            PointVariantListParameter p;
            vector < CGeometricObjectPoint* > pts;
            i >> type >> p;
            ReadPoints( co, p, pts, space );
            if ( type.IsRead() ) AddSurface( co, p.GetName().toString(), type.toString(), pts, space );
            else AddSurface( co, p.GetName().toString(), "", pts, space );
        }
        else if ( strtoken == "solid" )
        {
            CLangToken< IsValidToken > type( string("type") );
            PointVariantListParameter p;
            vector < CGeometricObjectPoint* > pts;
            i >> type >> p;
            ReadPoints( co, p, pts, space );
            if ( type.IsRead() ) AddSolid( co, p.GetName().toString(), type.toString(), pts, space );
            else AddSolid( co, p.GetName().toString(), "", pts, space );
        }
        else if ( strtoken == "rect" )
        {
            PointVariantListParameter p;
            vector < CGeometricObjectPoint* > pts;
            i >> p;
            unsigned ip = p.GetParam().GetList().size();
            if ( ip!=2 && ip != 4) throw CFEException( "Wrong number of points to create 'rect' object named :'" + p.GetName().toString() + "2 or 4 points are expected" );
            ReadPoints( co, p, pts, space );
            AddRect( co, p.GetName().toString(), pts, space );
        }
        else if ( strtoken == "cube" )
        {
            PointVariantListParameter p;
            vector < CGeometricObjectPoint* > pts;
            i >> p;
            unsigned ip = p.GetParam().GetList().size();
            if ( ip!=2 && ip != 8) throw CFEException( "Wrong number of points to create 'cube' object named :'" + p.GetName().toString() + "2 or 8 points are expected" );
            ReadPoints( co, p, pts, space );
            AddCube( co, p.GetName().toString(), pts, space );
        }
        else if ( strtoken == "ex" )
        {
            CLangToken< IsValidToken > obj_param;
            ExpressionVector dx_param;
            i >> obj_param >> dx_param;
            const mvector dx = FP.EvaluateExpressionVector( dx_param.GetList() );
            CGeometricObject *ego = co.FindGeometricObject( obj_param.toString() );
            co.ExtrudeObject( *ego, dx, space );
        }
        else if ( strtoken == "" ) break;
             else throw CFEException( "Unknown object type :'" + strtoken + "'" );

    }
    i >> c;
    if ( c != '}' ) throw CFEException( "'}' is expected at the end of model or object list" );
}




CSpace* CStatementModel::ReadSpace( istream &i )
{
    CLangToken< IsValidToken > name_par;
    TokenParam  base_par( string("type") );

    i >> name_par;
    CSpace *spname = FP.FindSpace( name_par.toString() );
    if ( spname ) return spname;

    i >> base_par >> params_par;

    unsigned npar = params_par.GetParam().GetList().size();

    CSpace *space = 0;
    unsigned k;
    const string basetype = base_par.GetParam().toString();
    if ( basetype == "rect2D" )
    {
        mvector xs(2);
        if ( npar )
        {
            xs[0] = FindParam("xs");
            xs[1] = FindParam("ys");
        }
        space = new CSpaceRectangular( xs );
    }
    else if ( basetype == "rect3D" )
    {
        mvector xs(3);
        if ( npar )
        {
            xs[0] = FindParam("xs");
            xs[1] = FindParam("ys");
            xs[2] = FindParam("zs");
        }
        space = new CSpaceRectangular( xs  );
    }
    else if ( basetype == "polar" ) {
        mvector xs(2);
        double f = 0.0;
        if ( npar )
        {
            xs[0]   = FindParam("xs");
            xs[1]   = FindParam("ys");
            f       = FindParam("f");
        }
        space = new CSpacePolar(xs, f );
    }
    else if ( basetype == "cylindrical" ) {
        space = new CSpaceCylindrical;
    }
    else throw CFEException( "Unknown space type '" + basetype + "' not found" );

    FP.AddSpace( name_par.toString(), space );
    return space;
}

double CStatementModel::FindParam( const string &nm )
{
    unsigned k;
    for ( k=0; k<params_par.GetParam().GetList().size(); k++ )
      if ( params_par.GetParam().GetList()[k].GetName() == nm )
          return params_par.GetParam().GetList()[k].GetParam();

    throw CFEException( "Unknown parameter '" + nm + "'" );
    return sqrt(-1.0);
}
