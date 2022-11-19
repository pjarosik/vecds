/*
 * File:   CStatementMesh.cpp
 * Author: piotrek
 * 
 * Created on November 22, 2011, 9:39 AM
 */

#include "StatementMesh.h"
#include "../FEMLib/MeshGeneratorRectangular2D.h"
#include "../FEMLib/MeshGeneratorRectangular3D.h"
#include "../FEMLib/MeshGeneratorRectangularTrig.h"
#include "../MathLib/SpaceShapeLagrange2N_2D_4.h"
#include "../MathLib/SpaceShapeLagrange3N_2D_9.h"
#include "../MathLib/SpaceShapeLagrange4N_2D_16.h"
#include "../MathLib/SpaceShapeLagrange2N_3D_8.h"
#include "../MathLib/SpaceShapeLagrange3N_3D_27.h"
#include "../MathLib/SpaceShapeLagrange4N_3D_64.h"
#include "../MathLib/SpaceShapeTriangle3.h"
#include "../MathLib/SpaceShapeApprox.h"

//#include "../MathLib/SpaceShapeSerendip3D_20.h"
//#include "../MathLib/SpaceShapeSerendip3D_32.h"
#include "../MathLib/Timer.h"
#include "../LangLib/statdef.h"
#include "../FEMLib/Renumber.h"
#include "../XMLLib/xml_exceptions.h"
#include "../XMLLib/xml_analyser.h"
#include "../XMLLib/XML.h"

CStatementMesh::CStatementMesh( CFEMProject &f ):FP(f) {
}

CStatementMesh::~CStatementMesh() {
}

void CStatementMesh::Execute( istream &in )
{
    TokenParam type(string("type")), domain(string("domain")) , elemtype(string("elemtype")), space_par(string("space"),true,false);
    TokenParamFilename	numbering(string("numbering"));

    
    in >> type >> domain >> elemtype >> numbering >> space_par;

    CSpaceShape *sfn = 0;
    CSpace *space = 0;
    
    if ( space_par.IsRead() ) space = FP.FindSpace( space_par.GetParam().toString() );

    cout << "Generating mesh type:" << type.GetParam().toString() << "...";
    
    CTimer timer;
    timer.Start();
    unsigned ne, nn, k, l, ground = 0;
    
     
    if ( type.GetParam().toString() == "rectangular" )
    {
        ExpressionVectorParameter x1(string("x1")), x2(string("x2"));
        UnsignedList dim(string("dim"));
        
        in >> x1 >> x2 >> dim;

        char g;
        in >> g;
        if ( g == 'g' )
        {
            in >> g;
            if ( g == '=' ) in >> ground;
            else throw CFEException( "'='' expected after 'g' -  ground elem size parameter" );
        }
        else in.putback( g );
        
        unsigned nx = dim.GetParam().GetList()[0], ny = dim.GetParam().GetList()[1];

        if (  x1.GetParam().GetList().size()!= 2 ) throw CFEException( "Dimension of x1 vector should be 2"  );
        if (  x2.GetParam().GetList().size()!= 2 ) throw CFEException( "Dimension of x2 vector should be 2"  );
        if ( dim.GetParam().GetList().size()!= 2 ) throw CFEException( "Dimension of dim list should be 2"  );

        if ( ( ground > 0 ) && (nx % ground) > 0 )  throw CFEException( "Mesh dimension nx is not a miltiple of groud element dimension" );
        if ( ( ground > 0 ) && (ny % ground) > 0 )  throw CFEException( "Mesh dimension ny is not a miltiple of groud element dimension" );
       
        if ( elemtype.GetParam().toString() == "Lagrange4" ) sfn = new CSpaceShapeLagrange2N_2D_4;
        else if ( elemtype.GetParam().toString() == "Lagrange9" ) sfn = new CSpaceShapeLagrange3N_2D_9;
        else if ( elemtype.GetParam().toString() == "Lagrange16" ) sfn = new CSpaceShapeLagrange4N_2D_16;
        else throw CFEException( "Unknown (or unproper for 2D plane problems ) shape functions type:" + elemtype.GetParam().toString() );
        
        CRenumber renum( numbering.GetParam().toString(), sfn->GetXdim() );

        CMeshGeneratorRectangular2D rm( *sfn, nx, ny );
        mvector v1 = FP.EvaluateExpressionVector( x1.GetParam().GetList() ), v2 = FP.EvaluateExpressionVector( x2.GetParam().GetList() );
        
        rm.Generate();
        rm.GenerateRectangularArea( v1[ 0 ], v1[ 1 ], v2[ 0 ], v2[ 1 ] );
        
        nn = rm.GetNodesNumber(); 
        ne = rm.GetElemsNumber();
        
 //       rm.Save("mesh_generated.txt");

        vector < CFiniteElementInstance2D* > feset;

        FP.FindBoundaryObjects();
        
        vector< CFENode* > nodes( nn );
        for (k=0; k<nn; k++)
        {
            CFENode *node =  new CFENode( rm.GetNodes()[ k ], space );
            nodes[k] = node;
            FP.AddNode( domain.GetParam().toString(), node );
        }

        for (k=0; k<ne; k++)
        {
            vector< CFENode* > enodes( rm.GetElements()[ k ].size() );
            for (l=0; l<rm.GetElements()[ k ].size(); l++ )
                enodes[ l ] = nodes[ rm.GetElements()[ k ][ renum[ l ] ] ];

            CSpaceShapeApproxPtr< CFENode > *ssa = new CSpaceShapeApproxPtr< CFENode >( *sfn, enodes , 2 );
            FP.GetGeometricDomain().AddSpace( ssa );
            CFiniteElementInstance2D *fe = new CFiniteElementInstance2D( FP.GetGeometricDomain(), *ssa );

            FP.AddElementInstance( domain.GetParam().toString(), fe );
            if ( ground ) feset.push_back( fe );
        }

        if ( ground )
        {
            for (k=0; k<nx*ny/ground/ground; k++)
            {
                vector< CFiniteElementInstance* > gfe;
                unsigned igx = k % (nx / ground ), igy = k / ( nx / ground ), m;
                for (m=0; m<ground; m++)
                    for (l=0; l<ground; l++)
                    {
                        unsigned ix = ground * igx + m, iy = ground * igy + l;
                        gfe.push_back( feset[ ix + iy * nx ] );
                    }
                CGroundElement ge( gfe, ground );
                FP.AddGroundElement( ge );
            }
        }
        
    }
    else if ( type.GetParam().toString() == "cubic" )
    {
        ExpressionVectorParameter x1(string("x1")), x2(string("x2"));
        UnsignedList dim(string("dim"));
        
        in >> x1 >> x2 >> dim;
        
        if (  x1.GetParam().GetList().size()!= 3 ) throw CFEException( "Dimension of x1 vector should be 3"  );
        if (  x2.GetParam().GetList().size()!= 3 ) throw CFEException( "Dimension of x2 vector should be 3"  );
        if ( dim.GetParam().GetList().size()!= 3 ) throw CFEException( "Dimension of dim list should be 3"  );
        
       
        if ( elemtype.GetParam().toString() == "Lagrange8" ) sfn = new CSpaceShapeLagrange2N_3D_8;
        else if ( elemtype.GetParam().toString() == "Lagrange27" ) sfn = new CSpaceShapeLagrange3N_3D_27;
        else if ( elemtype.GetParam().toString() == "Lagrange64" ) sfn = new CSpaceShapeLagrange4N_3D_64;
        else throw CFEException( "Unknown (or unproper for 3D plane problems ) shape functions type:" + elemtype.GetParam().toString() );
        

        CRenumber renum( numbering.GetParam().toString(), sfn->GetXdim() );
        CMeshGeneratorRectangular3D rm( *sfn, dim.GetParam().GetList()[0], dim.GetParam().GetList()[1],dim.GetParam().GetList()[2] );
        mvector v1 = FP.EvaluateExpressionVector( x1.GetParam().GetList() ),v2 = FP.EvaluateExpressionVector( x2.GetParam().GetList() );
        
        rm.Generate();
        rm.GenerateRectangularArea( v1[ 0 ], v1[ 1 ], v1[ 2 ], v2[ 0 ], v2[ 1 ], v2[ 2 ] );
        
        nn = rm.GetNodesNumber(); 
        ne = rm.GetElemsNumber();
        
 //     rm.Save("mesh_generated.txt");
        
        vector< CFENode* > nodes( nn );
        mvector xp( 3 ), dx( 3 );

        FP.FindBoundaryObjects();

        for (k=0; k<nn; k++)
        {
            CFENode *node = 0;
            for (l=0; l<FP.GetNodesDomain().GetAll().size(); l++ )
            {
                if ( FP.GetNodesDomain().GetAll()[l]->IsBoundary() )
                {
                    FP.GetNodesDomain().GetAll()[l]->GetX(xp);
                    dx = xp - rm.GetNodes()[ k ];
                    if ( dx.GetNorm() < 1e-6 )
                        node = FP.GetNodesDomain().GetAll()[l];
                }
            }
            if ( node == 0 )
            {
                node = new CFENode( rm.GetNodes()[ k ], space );
                FP.AddNode( domain.GetParam().toString(), node );
            }
            nodes[ k ] = node;
        }
        for (k=0; k<ne; k++)
        {
            vector< CFENode* > enodes( rm.GetElements()[k].size() );
            for (l=0; l<rm.GetElements()[k].size(); l++ )
                enodes[l] = nodes[ rm.GetElements()[ k ][  renum[ l ]  ] ];

            CSpaceShapeApproxPtr< CFENode > *ssa = new CSpaceShapeApproxPtr< CFENode >( *sfn, enodes, 3 );
            FP.GetGeometricDomain().AddSpace( ssa );
            FP.AddElementInstance( domain.GetParam().toString(), new CFiniteElementInstance3D( FP.GetGeometricDomain(), *ssa  ) );
        }      
    }
    else if ( type.GetParam().toString() == "Lg8" )
    {
        ExpressionVectorParameter   x1(string("x1")), x2(string("x2")),x3(string("x3")), x4(string("x4")),
                                    x5(string("x5")), x6(string("x6")),x7(string("x7")), x8(string("x8"));
        UnsignedList dim(string("dim"));

        in >> x1 >> x2 >> x3 >> x4 >> x5 >> x6 >> x7 >> x8 >> dim;

        if (  x1.GetParam().GetList().size()!= 3 ) throw CFEException( "Dimension of x1 vector should be 3"  );
        if (  x2.GetParam().GetList().size()!= 3 ) throw CFEException( "Dimension of x2 vector should be 3"  );
        if ( dim.GetParam().GetList().size()!= 3 ) throw CFEException( "Dimension of dim list should be 3"  );


        if ( elemtype.GetParam().toString() == "Lagrange8" ) sfn = new CSpaceShapeLagrange2N_3D_8;
        else if ( elemtype.GetParam().toString() == "Lagrange27" ) sfn = new CSpaceShapeLagrange3N_3D_27;
        else if ( elemtype.GetParam().toString() == "Lagrange64" ) sfn = new CSpaceShapeLagrange4N_3D_64;
        else throw CFEException( "Unknown (or unproper for 3D plane problems ) shape functions type:" + elemtype.GetParam().toString() );


        CRenumber renum( numbering.GetParam().toString(), sfn->GetXdim() );
        CMeshGeneratorRectangular3D rm( *sfn, dim.GetParam().GetList()[0], dim.GetParam().GetList()[1],dim.GetParam().GetList()[2] );
        vector< mvector >  vs( 8 );

        vs[ 0 ] =  FP.EvaluateExpressionVector( x1.GetParam( ).GetList() );
        vs[ 1 ] =  FP.EvaluateExpressionVector( x2.GetParam( ).GetList() );
        vs[ 2 ] =  FP.EvaluateExpressionVector( x3.GetParam( ).GetList() );
        vs[ 3 ] =  FP.EvaluateExpressionVector( x4.GetParam( ).GetList() );
        vs[ 4 ] =  FP.EvaluateExpressionVector( x5.GetParam( ).GetList() );
        vs[ 5 ] =  FP.EvaluateExpressionVector( x6.GetParam( ).GetList() );
        vs[ 6 ] =  FP.EvaluateExpressionVector( x7.GetParam( ).GetList() );
        vs[ 7 ] =  FP.EvaluateExpressionVector( x8.GetParam( ).GetList() );

        CSpaceShapeLagrange2N_3D_8 ss8;

        rm.Generate();
        rm.GenerateShapeArea( &ss8, vs );

        nn = rm.GetNodesNumber();
        ne = rm.GetElemsNumber();

 //     rm.Save("mesh_generated.txt");

        vector< CFENode* > nodes( nn );
        mvector xp( 3 ), dx( 3 );

        for (k=0; k<nn; k++)
            nodes[ k ] = FP.AddUniqueNode( domain.GetParam().toString(), rm.GetNodes()[ k ] );

        for (k=0; k<ne; k++)
        {
            vector< CFENode* > enodes( rm.GetElements()[k].size() );
            for (l=0; l<rm.GetElements()[k].size(); l++ )
                enodes[l] = nodes[ rm.GetElements()[ k ][  renum[ l ]  ] ];

            CSpaceShapeApproxPtr< CFENode > *ssa = new CSpaceShapeApproxPtr< CFENode >( *sfn, enodes, 3 );
            FP.GetGeometricDomain().AddSpace( ssa );
            FP.AddElementInstance( domain.GetParam().toString(), new CFiniteElementInstance3D( FP.GetGeometricDomain(), *ssa  ) );
        }
    }
    else if ( type.GetParam().toString() == "RectDoubleTrig" )
    {
        ExpressionVectorParameter x1(string("x1")), x2(string("x2"));
        UnsignedList dim(string("dim"));
        TokenParam param_pattern(string("pattern"));
        TMPattern ptr;


        in >> param_pattern >> x1 >> x2 >> dim;

        if ( param_pattern.GetParam().toString()      == "Double" )   ptr = TM_DOUBLE;
        else if ( param_pattern.GetParam().toString() == "Double2" )  ptr = TM_DOUBLE2;
        else if ( param_pattern.GetParam().toString() == "Quad" )     ptr = TM_QUAD;
        else throw CFEException( "Unknown triangular pattern type :'" + param_pattern.GetParam().toString()  + "'" );

        unsigned nx = dim.GetParam().GetList()[0], ny = dim.GetParam().GetList()[1];

        if (  x1.GetParam().GetList().size()!= 2 ) throw CFEException( "Dimension of x1 vector should be 2" );
        if (  x2.GetParam().GetList().size()!= 2 ) throw CFEException( "Dimension of x2 vector should be 2" );
        if ( dim.GetParam().GetList().size()!= 2 ) throw CFEException( "Dimension of dim list should be 2"  );

        if ( elemtype.GetParam().toString() == "Triangle3" ) sfn = new CSpaceShapeTriangle3;
        else throw CFEException( "Unknown (or unproper for 2D plane problems 'RectDoubleTrig' generator ) shape functions type:" + elemtype.GetParam().toString() );

        CMeshGeneratorRectangularTrig rm( *sfn, nx, ny, ptr );

        mvector v1 = FP.EvaluateExpressionVector( x1.GetParam().GetList() ), v2 = FP.EvaluateExpressionVector( x2.GetParam().GetList() );

        rm.Generate();
        rm.GenerateRectangularArea( v1[ 0 ], v1[ 1 ], v2[ 0 ], v2[ 1 ] );

        nn = rm.GetNodesNumber();
        ne = rm.GetElemsNumber();

 //       rm.Save("mesh_generated.txt");

        CRenumber renum( numbering.GetParam().toString(), nn );
        FP.FindBoundaryObjects();

        vector< CFENode* > nodes( nn );
        for (k=0; k<nn; k++)
        {
            CFENode *node =  new CFENode( rm.GetNodes()[ k ], space );
            nodes[k] = node;
            FP.AddNode( domain.GetParam().toString(), node );
        }

        for (k=0; k<ne; k++)
        {
            vector< CFENode* > enodes( rm.GetElements()[ k ].size() );
            for (l=0; l<rm.GetElements()[ k ].size(); l++ )
                enodes[ l ] = nodes[ rm.GetElements()[ k ][ renum[ l ] ] ];

            CSpaceShapeApproxPtr< CFENode > *ssa = new CSpaceShapeApproxPtr< CFENode >( *sfn, enodes , 2 );
            FP.GetGeometricDomain().AddSpace( ssa );
            CFiniteElementInstance2D *fe = new CFiniteElementInstance2D( FP.GetGeometricDomain(), *ssa );

            FP.AddElementInstance( domain.GetParam().toString(), fe );
        }


    }
    else if ( type.GetParam().toString() == "Numpress" )
    {
            TokenParamFilename flt((string)"filename");
            in >> flt;
            string filename = flt.GetParam().toString();
            ifstream id( filename.c_str() );
            if ( !id.is_open() ) throw CXML_Exception("Can't open file :" + filename);
            CXML xml;
            CXML_Analyser< CXML > reader( id, xml );
            reader.ReadXMLFile( );
    }
    else throw CFEException( "Unknown mesh type :" + type.GetParam().toString() );
 
    timer.Stop();
    
    cout << " done! Generated nodes :" << nn << " elements :" << ne << " generation time :" << timer.GetFormattedTime() << endl;
    
    FP.AddSpaceShape(sfn);
}
