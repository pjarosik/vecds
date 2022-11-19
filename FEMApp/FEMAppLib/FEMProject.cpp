
#include "FEMProject.h"
#include "../FEMLib/FEException.h"
#include "../MathLib/SpaceShapeLagrange2N_3D_8.h"
#include "../MathLib/SpaceShapeLagrange3N_3D_27.h"
#include "../MathLib/SpaceShapeLagrange4N_3D_64.h"
#include "../MathLib/SpaceShapeLagrange2N_3D_8.h"
#include "../FEMLib/FE_EquationQuasistaticLinear.h"
#include "../FEMLib/ProfileReductionRCM.h"
#include "../FEMLib/Renumber.h"

#include "../MathLib/LargeMatrixSkyLineCholesky.h"
#include "../FEMLib/FEFunctorMatrixToNodalTransformation.h"
#include "../MathLib/Scaler.h"
#include "../MolecularLib/MException.h"
#include "../LangLib/FileLangStream.h"
#include "../LangLib/statdef.h"
#include "../MathLib/ProgressConsole.h"
#include "AtomAssignment.h"
#include "StatementAtom.h"
#include "StatementAtoms.h"
#include "StatementTitle.h"
#include "StatementEquation.h"
#include "StatementNodes.h"
#include "StatementElemDef.h"
#include "StatementElements.h"
#include "StatementMaterial.h"
#include "StatementBoundaryConditions.h"
#include "StatementResults.h"
#include "StatementOutput.h"
#include "StatementSolve.h"
#include "StatamentDOFs.h"
#include "StatementMesh.h"
#include "StatementPlot.h"
#include "StatementSpace.h"
#include "StatementModel.h"
#include "StatementFeap.h"
#include "StatementVariables.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

CFEMProject::CPaintSetup::CPaintSetup( bool st )
{
    paint_nodes = paint_edges = paint_faces = paint_bcs = paint_border_only = paint_node_numbers = paint_fe_numbers = st;
}

CFEMProject::CFEMProject(void):MC(),nodes(false),elements(false),disp_scale(-1.0)
{
	stat["title"]		= new CStatementTitle( *this );
	stat["atoms"]		= new CStatementAtoms( MC );
	stat["atom"]		= new CStatementAtom( MC );
	stat["equation"]	= new CStatementEquation( *this );
	stat["nodes"]		= new CStatementNodes( *this );
	stat["material"]	= new CStatementMaterial( *this );
	stat["elements"]	= new CStatementElements( *this );
	stat["elemdef"]		= new CStatementElemDef( *this );
    stat["bc"]          = new CStatementBoundaryConditions( *this );
    stat["solve"]       = new CStatementSolve( *this );
    stat["results"]     = new CStatementResults( *this );
	stat["output"]		= new CStatementOutput( *this );
    stat["dofs"]		= new CStatementDOFs( *this );
    stat["mesh"]		= new CStatementMesh( *this );
    stat["feap"]		= new CStatementFeap( *this );
    stat["plot"]		= new CStatementPlot( *this );
    stat["space"]		= new CStatementSpace( *this );
    stat["model"]		= new CStatementModel( *this );
    stat["variables"]	= new CStatementVariables( Variables );
}

CFEMProject::~CFEMProject( void )
{
	Reset();
}

void CFEMProject::Reset()
{
    GD.clear();
    nodes.clear();
    elements.clear();
	delete_vector( LargeMatrices );
    delete_vector( shapefunctions );
	delete_vector( AS );
	delete_vector( intpoints );
    delete_vector( postprocitems );
    delete_vector( spaces_rep );

	materials.Reset();
	equations.Reset();
}

template< class T >
void CFEMProject::delete_vector( vector< T* > &v )
{
	for (unsigned k=0; k<v.size(); k++)
		delete v[ k ];
}

void CFEMProject::CreateDOFs( const string &bran  )
{
    CProfileReduction *pr=0;
    if ( bran == "RCM" ) pr = new CProfileReductionRCM;
    if (!pr && bran!="none" ) throw CFEException( "Unknown bandwidth reduction algorithm name :" + bran );
    if ( pr ) pr->Optimize();
    map_named_ptr< CFE_Equation >::iterator i;
	for (i=equations.begin(); i!=equations.end(); i++)
		i->second->CreateDOFs(pr);
}

void CFEMProject::AddSpace( const string &nm, CSpace *space )
{
    if (!space) throw CFEException( "CFEMProject::AddSpace - null space pointer" );
    if (nm.size()==0) throw CFEException( "CFEMProject::AddSpace - name of new space cannot be an empty string");
    spaces[ nm ] = space;
    CSpace *sp = space;
    while(sp)
    {
        spaces_rep.push_back( sp );
        sp = sp->GetSpace();
    }
}

CSpace* CFEMProject::FindSpace( const string &nm ) const
{
    map<string,CSpace*>::const_iterator i = spaces.find(nm);
    if ( i == spaces.end() ) return 0;
    return i->second;
}

void CFEMProject::Solve()
{
    FindBoundaryObjects();
	map_named_ptr< CFE_Equation >::iterator i;
	for (i=equations.begin(); i!=equations.end(); i++)
	{
		i->second->Initialize();
		i->second->Solve();
	}
}


void CFEMProject::FindBoundaryObjects(bool set)
{
    unsigned k;
    for (k=0; k<elements.size(); k++) elements.GetAll()[k]->SetBoundary( !set );
    if ( !set ) return;
    for (k=0; k<elements.size(); k++)
         elements.GetAll()[k]->FindBoundaryObjects();

    //GD.ComputeBoundaryStat();
    //GD.PrintConnections();

}

CFE_Equation* CFEMProject::GetEquation( const string &nm, bool ex )
{
    CFE_Equation *eq = equations.Find( nm );
    if ( eq == 0 && ex ) throw CFEException( "Unknow FEEquation named \'" + nm + "\'" );
    return eq;
}

void CFEMProject::RectangularMesh( const CSpaceShape &rs, const vector< mvector > &crn, unsigned nx, unsigned ny, unsigned nz )
{
	ofstream fout("mesh_data.txt");

    CSpaceShapeLagrange2N_3D_8  afn;
	vector< mvector > nd;
	vector< vector< unsigned > > nel;
    mvector N( afn.GetXdim() );

    const vector< mvector > &LN = rs.GetLocalNodes();

	unsigned k,l,m, i, j;
	double dx = 2.0 / (double)nx, dy = 2.0 / (double)ny, dz = 2.0 / (double)nz;

	for (k=0; k<nx; k++)
		for (l=0; l<ny; l++)
			for (m=0; m<nz; m++)
			{
				vector< unsigned > nde;

                for(i=0; i<rs.GetXdim(); i++)
				{
					mvector xi( 3 ), X( 3 );

					CScaler sx( -1.0, 1.0, k * dx - 1 ,( k + 1 ) * dx -1);	xi[0] = sx.scale( LN[i][0] );
					CScaler sy( -1.0, 1.0, l * dy - 1 ,( l + 1 ) * dy -1);	xi[1] = sy.scale( LN[i][1] );
					CScaler sz( -1.0, 1.0, m * dz - 1 ,( m + 1 ) * dz -1);	xi[2] = sz.scale( LN[i][2] );

                    afn.transform( xi, N );

					for (j=0; j<N.GetDim(); j++)
					{
						X[0] += N[j] * crn[j][0];
						X[1] += N[j] * crn[j][1];
						X[2] += N[j] * crn[j][2];
					}

					bool found = false;
					for (j=0; j<nd.size(); j++)
					{
						if (	( fabs( X[0] - nd[j][0] ) < 1e-8 ) &&
								( fabs( X[1] - nd[j][1] ) < 1e-8 ) &&
								( fabs( X[2] - nd[j][2] ) < 1e-8 ) )
						{
							nde.push_back( j );
							found = true;
							break;
						}
					}

					if ( !found )
					{
						nde.push_back( (unsigned)nd.size() );
						nd.push_back( X );
					}

				}
				nel.push_back( nde );

			}

	fout << endl << nd.size() << " nodes" << endl;

	fout.precision(10);
	fout.setf( ios::scientific );
	for (k=0; k<nd.size(); k++)
		fout << nd[k][0] << "     " << nd[k][1] << "     " << nd[k][2] << "     " << endl;

	fout << endl << endl;

	fout << nel.size() << " elements" << endl;
	for (k=0; k<nel.size(); k++)
	{
		for ( l=0; l<nel[ k ].size(); l++) fout << nel[ k ][ l ] << "  ";
		fout << endl;
	}

}

template< class T >
void CFEMProject::CreateList( const string &slist, const vector< T* > &full, vector< T* > &list )
{
	if ( slist == "" ) throw CFEException( "empty list" );
	if ( slist == "all" ) { list = full; return; }
	stringstream ss( slist );
	int p=-1, k=-1, i;
	char c;
	while( !ss.eof() )
	{
		k = p = -1;
		ss >> p;
		if ( p == -1 ) return;
		ss >> c;
		if ( c == ',' ) list.push_back( full[p] );
		else if ( c == '-' )
			{
				ss >> k;
				if ( k == -1 ) throw CFEException( "malformed list :" + slist );
				for (i=p; i<=k; i++) list.push_back( full[i] );
				ss >> c;
				if ( c != ',' )
				{
					if ( ss.eof() ) return;
					else throw CFEException( "malformed list :" + slist );
				}
			}
			else throw CFEException( "malformed list :" + slist );
	}

}

void CFEMProject::ReadFile( const string &filename )
{
	CFileLangStream fi( filename );
	ExecuteCommands( fi );

}


void CFEMProject::CreateMolecularConnectionStructure( )
{
    unsigned k,l;
    const vector< CAtom* > &atoms = MC.GetAtoms();

	for(k=0; k<atoms.size(); k++)
    {
        vector< CFiniteElementInstance* > FEcan;
        for(l=0; l< elements.size(); l++ )
            if ( elements[k]->IsInBoundingBox( atoms[k]->GetX() ) )
                FEcan.push_back( elements[ l ] );

        if ( FEcan.empty() ) throw CMException("Atom not asigned to any element");

        for(l=0; l< FEcan.size(); l++ )
        {
            if ( FEcan[ l ]->IsInside( atoms[k]->GetX() ) )
            {
				mvector xi( atoms[k]->GetX().GetDim() );
				FEcan[ l ]->GetXi( atoms[k]->GetX(), xi );
                AS.push_back( new CAtomAssignment( atoms[ k ], FEcan[ l ], xi ) );
				ASM[ atoms[ k ] ] = AS.back();
                break;
            }

        }
  //      if ( FEcan.empty() ) throw CMException("Atom - element assignment not found");

    }
}


void CFEMProject::GenerateRandomAtoms( const string &at, unsigned nb )
{
    mvector x(3);
//    const mvector x1 = FEbb.GetMin( ), x2 = FEbb.GetMax( );


}

void CFEMProject::ExecuteCommands( CLangStream  &ls )
{

//	string cmd;
	istream &inp = ls.GetIls();

	while ( true )
	{
        CLangToken< IsValidToken > cmd;
		inp >> cmd;
        cout << cmd.toString() << endl;
        bool iseof = inp.eof();
        if ( !cmd.toString().size() && inp.eof() ) return;
        if ( cmd.toString() == "->HALT<-")  return;

        CStatement *st = stat[ cmd.toString() ];
        if ( !st && cmd.toString().size() ) throw CLangException( &inp, "unknown command :" + cmd.toString() );
        if ( st ) inp >> *st;

/*        char c;
        inp >> c;
        if ( c == '=' )
        {

        }
        else {
            inp.putback(c);

        } */
	}

}

void CFEMProject::VTK_Export( ostream &fout, bool is_deformed )
{



}

void CFEMProject::PrintResults( ostream &out )
{


	out << "  *****************************************" << endl;
	out << " *        FINITE ELEMENT APPLICATION       *" << endl;
	out << "*                                           *" << endl;
	out << " *          P. TAUZOWSKI (C) 2008-2011     *" << endl;
	out << "  *****************************************" << endl << endl;

	out << "Job title :" << jobtitle << endl<< endl;

	unsigned k, l, m, n, precicion = 5;

	for (k=0; k<elements.size(); k++) elements[k]->SetNumber(k);

	out.precision( precicion );
	out.setf( ios::scientific );

	out << " ***************************************" << endl;
	out << "*  D A T A                              *" << endl;
	out << " ***************************************" << endl << endl;


	out << "*  N O D A L   C O O R D I N A T E S  *" << endl << endl;

    mvector X;
	for (k=0; k<nodes.size(); k++)
	{
        nodes[k]->GetX(X);
		out << setw( 6 ) << k;
        for (l=0; l<X.GetDim(); l++)
            out << setw( 5 ) << 'x' << l+1 << " =" << setw( precicion + 9 ) << X[l];
		out << endl;
	}
//	setiosflags( ios::
	out << endl << endl;

	out << " ***************************************" << endl;
	out << "*  E L E M E N T S   D O M A I N S      *" << endl;
	out << " ***************************************" << endl << endl;

    vector< CFiniteElementInstance* >::iterator ci;
    vector< string > dnames;
    elements.GetDomainNames( dnames );

    for (k=0; k<dnames.size(); k++)
	{
        const vector< CFiniteElementInstance* > *domain = elements.GetDomain( dnames[ k ] );
        out << "ELEMENT DOMAIN :" << dnames[k];
        out << "DOMAIN SIZE    :" << domain->size() << '/' << elements.size() << " finite elements" << endl;
		out << setw( 15 ) << "global index" << setw( 15 ) << "domain index" << setw( 3 * 8  ) << " node numbers" << endl;
        for (l=0; l<domain->size(); l++)
		{
            const vector< CFENode* > &enodes = (*domain)[l]->GetNodes();
            out << setw(15) <<  (*domain)[l]->GetNumber() << setw(15) << l << "          ";
			for (k=0; k<enodes.size(); k++)
				out << setw( 8 ) << enodes[k]->GetNumber();
			out << endl;
		}
		out << endl;
	}

	out << " ***************************************" << endl;
	out << "*  R E S U L T S                        *" << endl;
	out << " ***************************************" << endl << endl;

//	out << " ***************************************" << endl;
	out << "*  D E G R E E S   O F   F R E E D O M  *" << endl;
//	out << " ***************************************" << endl << endl;

	out << endl;

	for (k=0; k<nodes.size(); k++)
	{
		CDOFVector &ndof = nodes[k]->GetDofs();
		out << setw( 6 ) << k;
		for (l=0; l<ndof.size(); l++)
			out << setw(5) << ndof[l]->DofName() << " =" << setw( precicion + 9 ) << ndof[l]->GetSolution();
		out << endl;
		nodes[k]->ResetResults();
	}


	out << endl << endl;

        out << " ***************************************" << endl;
        out << "*   E L E M E N T S    R E S U L T S    *" << endl;
        out << " ***************************************" << endl;

        map_named_ptr< CFiniteElement >::iterator ie;
        for (ie = felements.begin(); ie != felements.end(); ie++)
        {
            //ie->second->Print( out );
        }

/*	out << endl;
        map_named_ptr< CFE_Creator >::iterator cri;
        for (cri = creators.begin(); cri != creators.end(); ci++)
	{
                vector<CFiniteElementInstance*> *fe = GetDomain( cri->second );
                out << "ELEMENT DOMAIN :" << cri->second << endl;
		out << "DOMAIN SIZE    :" << fe->size() << '/' << elements.size() << " finite elements" << endl;
                out << "ELEMENT TYPE   :" << cri->first->GetTypeName() << endl;
		out << setw( 9 ) << "gl index" << setw( 9 ) << "dm index" << setw( 6 ) << "gp" << setw( 2 * ( precicion + 9 ) ) << "gauss points results" << endl;
                CFEFunctorMatrix *gpR = cri->first->GetGaussPointResultsFEFunctorVector();
		if (!gpR)
		{
		    out << "No gaus point results for this element type" << endl;
		    continue;
		}
                CFEFunctorMatrixToNodalTransformation nval( cri->first->GetIntegrationPoints(), gpR  );
		for (l=0; l<fe->size(); l++)
		{
			if (gpR)
			{
				matrix &gprm = *gpR;
				nval( (*fe)[ l ] );
				for (m=0; m<gprm.GetRows(); m++)
				{
					out << setw( 9 ) << (*fe)[ l ]->GetNumber() << setw( 9 ) << l << setw( 6 ) << m;
					for (n=0; n<gprm.GetCols(); n++)
						out << setw( 10 ) << gpR->GetColsNames()[ n ] << setw( 2 ) << " =" << setw( precicion + 9 ) << gprm(m,n);
					out << endl;
				}
			}
			out << endl;
		}
		out << endl;
        }
*/
	out << endl << endl;

	out << "*  N O D A L   R E S U L T S   R E S U L T S  *" << endl;

	out << endl;

	for (k=0; k<nodes.size(); k++)
	{
		out << setw( 6 ) << k;
		const map< string, double> &nres =nodes[k]->GetResults();
		map< string, double>::const_iterator ri = nres.begin();
		for (ri = nres.begin(); ri!= nres.end(); ri++ )
                        out << setw( 15 ) << ri->first << " =" << setw( precicion + 9 ) << ri->second;
		out << endl;
	}
}

void CFEMProject::LinkVariables( CExpression *expr ) const
{
    const map<string,CVariable*> &vars =  Variables.GetVariables();
    map<string,CVariable*>::const_iterator i;
    for (i=vars.begin(); i!=vars.end(); i++)
    {
        VariableDouble *vd = dynamic_cast< VariableDouble* >( i->second );
        expr->SetVariable( i->first, vd );
    }
}

mvector CFEMProject::EvaluateExpressionVector( const vector< CLangToken< IsValidExpression > > &vex) const
{
    unsigned k;
    mvector x( vex.size() );
    for ( k=0; k<vex.size(); k++)
        x[ k ] = EvaluateExpression( vex[ k ].toString() );
    return x;
}


CFENode* CFEMProject::AddUniqueNode( const string &dm, const mvector &xp )
{
    unsigned k;
    for( k=0; k<nodes.GetAll().size(); k++)
        if ( nodes.GetAll()[k]->Distance(xp) < 1.e-6 ) return nodes.GetAll()[k];
    CFENode *node = new CFENode( xp );
    GD.AddGeometricObject(node,"FENode");
    nodes.AddMember( node, dm );
    return node;
}
