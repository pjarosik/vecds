#ifndef STATEMENTMODEL_H
#define STATEMENTMODEL_H
#include "../LangLib/Statement.h"
#include "FEMProject.h"
#include "../LangLib/statdef.h"


class CStatementModel : public CStatement
{

public:
    CStatementModel( CFEMProject &fp  );
    virtual void Execute( istream &i );
    virtual ~CStatementModel( );

protected:

    CModelObject *current_object;
    CModel *model;
    CFEMProject &FP;
    NamedParameterListDoubleParameter2 params_par;

    CSpace* ReadSpace( istream &i );
    bool ReadPoints( CModelObject &co, const PointVariantList &pvl, vector < CGeometricObjectPoint* > &pts, CSpace *sp  );
    void ReadObject( CModelObject &mo, istream &i,CSpace *space, const string &basename="" );
    CGeometricObjectPoint* AddPoint(  CModelObject &co, const mvector &xp, const string name, CSpace *sp = 0 );
    void AddShapeObject( CModelObject &co, const string &name, CSpaceShape *sfn, const vector< CGeometricObjectPoint* > &points, CSpace *sp );
    void AddSurface( CModelObject &co, const string &name, const string type, vector < CGeometricObjectPoint* > &points, CSpace *sp );
    void AddSolid( CModelObject &co, const string &name, const string type, vector < CGeometricObjectPoint* > &points, CSpace *sp );
    void AddLine( CModelObject &co, const string &name, const string type, vector < CGeometricObjectPoint* > &points, CSpace *sp );
    void AddRect( CModelObject &co, const string &name, vector < CGeometricObjectPoint* > &pts, CSpace *sp );
    void AddCube( CModelObject &co, const string &name, vector < CGeometricObjectPoint* > &pts, CSpace *sp );
    void Project( const DoubleVector &dv, mvector &x );
    void Project( const CLangToken< IsValidToken > &pname, string &str );
    double FindParam( const string &nm );
};

#endif // STATEMENTMODEL_H

