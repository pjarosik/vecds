#include "StatementFeap.h"
#include "../FEMLib/Renumber.h"

#include "../MathLib/SpaceShapeLagrange2N_3D_8.h"
#include "../MathLib/SpaceShapeLagrange2N_1D_2.h"
#include "../MathLib/SpaceShapeLagrange3N_3D_27.h"
#include "../MathLib/SpaceShapeLagrange4N_3D_64.h"


CStatementFeap::CStatementFeap( CFEMProject &f ):FP( f )
{

}

CStatementFeap::~CStatementFeap( void )
{

}

void CStatementFeap::SkipLines( istream &in, unsigned nl )
{
    string line;
    unsigned k;
    for( k=0; k < nl; k++ ) getline(in,line);
}

bool CStatementFeap::Find( istream &in, const string tofind )
{
    stringstream ss( tofind );
    vector<string> strings;
    while( !ss.eof() )
    {
        string token;
        ss >> token;
        strings.push_back( token );
    }


    unsigned k;
    for( k=0; k<strings.size(); k++)
    {
        while ( !in.eof() )
        {
            string token;
            in >> token;
            if ( token == strings[ k ] ) break;
        }
        if ( in.eof() ) throw CFEException( "Can't find in feap file sequence :" + tofind );
    }


    return true;
}

void CStatementFeap::Execute( istream &in )
{

    TokenParamFilename name(string("filename"));

    in >> name;

    Import( name.GetParam().toString() );
    FP.FindBoundaryObjects();
    int a=1;
/*
    TokenParam name(string("set"));
    NamedParameterDoubleListWhite2 coords(string("coords"));
    in >> name >> coords;

    if ( coords.GetParam().GetList().size() == 0 ) return;
    if ( coords.GetParam().GetList().size() % 3 ) throw CFEException( "Malformed nodes's coordinates list" );

    mvector x( 3 );
    unsigned i = 0,ni=0;
    while ( i < coords.GetParam().GetList().size() )
    {
        x[ 0 ] = coords.GetParam().GetList()[ i++ ];
        x[ 1 ] = coords.GetParam().GetList()[ i++ ];
        x[ 2 ] = coords.GetParam().GetList()[ i++ ];
        FP.AddNode(  name.GetParam().toString(), new CFENode( x ) );
    }

    cout << "readed nodes :" << FP.GetNodes().size() << endl;
*/



}


void CStatementFeap::Import( const string &filename )
{

    ifstream fin( filename.c_str() );

    if ( !fin.is_open() ) throw CFEException( "Can't open feap file :" + filename );

    Find( fin, "Number of Nodal Points  - - - - - - :");

    string snne;
    unsigned nn, ne, nne;
    fin >> nn;

    Find( fin, "Number of Elements  - - - - - - - - :");
    fin >> ne;

    Find( fin, "Spatial Dimension of Mesh - - - - - :");
    fin >> m_dim;

    Find( fin, "Degrees-of-Freedom/Node (Maximum) - :");
    fin >> m_ndisp;

    Find( fin, "Number Element Nodes    (Maximum) - :");
    fin >> snne;
    nne = atoi( snne.c_str() );

    unsigned k=1;
    mvector xp(3);
    double x1, x2, x3;

    while ( k < nn )
    {
        Find( fin, "node    1 Coord    2 Coord    3 Coord");
        while( true )
        {
           string number;
           fin >> number;
           if ( number == "feap" )
           {
                getline( fin, number );
                fin >> number;
                if ( number == "E" )
                {
                    k = nn;
                }
                break;
           }
           fin >> xp[0] >> xp[1] >> xp[2];
           FP.AddNode( "Feap", new CFENode( xp ) );
           numbers[ atoi( number.c_str() ) ] = k - 1;
           k++;
           if ( k == nn ) break;
        }
    }

    //FP.ComputeDiagonal();
    FP.GetGeometricDomain().EnumerateObjects();
/*    vector< unsigned > on;
    if ( FP.CheckOverlappedNodes(on) )
    {
        cout << endl << "Overlapped nodes found :";
        for(k=0; k<on.size(); k++) cout << on[ k ] + 1 << ",";
        cout << endl;
        throw CFEException( "Overlapped nodes" );
    }
    else cout << "No overlapped nodes found" << endl; */

    CSpaceShape *sfn = 0;
    switch (nne) {
        case 8:     sfn = new CSpaceShapeLagrange2N_3D_8; break;
        case 27:    sfn = new CSpaceShapeLagrange3N_3D_27; break;
        case 64:    sfn = new CSpaceShapeLagrange4N_3D_64; break;
        default: throw CFEException( "Wrong number od nodes in element:" );
    }

    CRenumber renum( "FEAP_FEnum_3D_27_2.txt", nne );
    k=1;
    vector< CFENode* > enodes( nne );
    while ( k < ne )
    {
        string number;
        Find( fin, "Node " + snne + " Node");
        unsigned l, mat,ge;
        while( true )
        {
            fin >> number;
            if ( number == "feap" )
            {
                 getline( fin, number );
                 break;
            }

            fin >> mat >> ge;
            unsigned m;
            for (m=0; m<nne; m++)
            {
                unsigned nnmb;
                fin >> nnmb;
                CFENode *node = FP.GetNodes()[ numbers[ nnmb  ] ];
                //enodes[ renum.GetIndexWithOffset( m ) ] = node;
                unsigned fi = renum.MyToExternal( m );
                enodes[ fi ] = node;
            }

            CFiniteElementInstance *elem = 0;
            CSpaceShapeApproxPtr< CFENode > *ssa = new CSpaceShapeApproxPtr< CFENode >( *sfn, enodes, enodes[0]->GetObjectDim() );
            FP.GetGeometricDomain().AddSpace( ssa );
            elem = new CFiniteElementInstance3D( FP.GetGeometricDomain(), *ssa );
            FP.AddElementInstance( "feap", elem );
            k++;
            if ( k > ne ) break;
        }
    }

    Find( fin, "N o d a l   D i s p l a c e m e n t s");
    k=1;
    Find( fin, "Node");
    vector< string > dnames;
    for (k=0; k<m_dim; k++ )
    {
        string nr, nm;
        fin >> nr >> nm;
        dnames.push_back( nr + " " + nm);
    }

    for (k=0; k<m_ndisp; k++ )
    {
        string nr, nm;
        fin >> nr >> nm;
        dnames.push_back( nr + " " + nm);
    }



    for ( k=0; k<nn; k++ )
    {
        unsigned number;
        mvector ndispl( m_dim + m_ndisp );
        unsigned m;
        fin >> number;
        for (m=0; m< m_dim + m_ndisp; m++)  fin >> ndispl[ m ];
        CFENode *node = FP.GetNodes()[ k ];
        node->AccumulateResults(dnames,ndispl);
    }

    Find( fin, "N o d a l   P r o j e c t i o n s");
    k=1;
    Find( fin, "10 Value");

    dnames.clear();

    dnames.push_back("1-Pr");
    dnames.push_back("2-Pr");
    dnames.push_back("3-Pr");
    dnames.push_back("1-Pr");
    dnames.push_back("I_1");
    dnames.push_back("J_2");
    dnames.push_back("J_3");
    dnames.push_back("sigma_11");
    dnames.push_back("sigma_22");
    dnames.push_back("sigma_33");
    dnames.push_back("sigma_23");
    dnames.push_back("sigma_31");
    dnames.push_back("sigma_12");
    dnames.push_back("alpha_13");
    dnames.push_back("alpha_23");
    dnames.push_back("alpha_33");
    dnames.push_back("detF");

    mvector ndispl(17);
    while ( true )
    {
        unsigned number;
        unsigned m;
        fin >> number;
        for (m=0; m<17; m++)  fin >> ndispl[ m ];
        CFENode *node = FP.GetNodes()[ number - 1 ];
        node->AccumulateResults(dnames,ndispl);
        unsigned pos = fin.tellg();
        char c;
        fin >> c;
        fin.seekg( pos );
        if ( c == '*' )
        {
            int a=1;
            break;
        }
        k++;
    }

    for (k=0; k<FP.GetElements().size(); k++)
        FP.GetElements()[k]->ApproximateAllNodalResults();
    FP.FindBoundaryObjects();
}
