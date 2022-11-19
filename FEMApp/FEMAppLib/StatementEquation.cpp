#include "StatementEquation.h"
#include "../FEMLib/FE_EquationQuasistaticLinear.h"
#include "../FEMLib/FE_EquationQuasistaticNonlinear.h"
#include "../FEMLib/FE_EquationTopologyOptimization.h"

#include "../MathLib/LargeMatrixSkyLineCholesky.h"
#include "../MathLib/LargeMatrixSkyLineCholesky2.h"
#include "../MathLib/LargeMatrixSparseCRJacoby.h"
#include "../MathLib/LargeMatrixSparseCR_CG.h"
#include "../MathLib/LargeMatrixSparseCR_PCG.h"
#include "../MathLib/LargeMatrixSparseStream_PCG.h"
#include "../MathLib/LargeMatrixAsymmetricalCrout.h"
#include "../MathLib/LargeMatrixSparseCR_GaussSeidel.h"

#ifdef USE_PARDISO
     #include "../MathLib/LargeMatrixSparseCR_Pardiso.h"
#endif

#include "../LangLib/statdef.h"
#include "../FEMLib/PostProcess.h"

CStatementEquation::CStatementEquation( CFEMProject &f ):FP( f )
{
}

CStatementEquation::~CStatementEquation( void )
{
}

void CStatementEquation::Execute( istream &i )
{
    TokenParam name(string("name")),  type(string("type"));
    TokenParamUnsigned nthreads(string("nthreads"), true, false );
	NamedParameterListTokenParameter2  matrixtypes(string("matrixtypes"));
	NamedParmeterTokenList2 elements(string("elements"));

	CFE_Equation *equation = 0;

    unsigned nth=1;

// Reading equation unique name
// Checking if the name already exists
    if ( FP.GetEquation( name.GetParam().toString(), false ) )
		throw CFEException( "Equation named: " + name.GetParam().toString() + " already exists");

// Reading rest of parameters: type of equation, type of large matrixc, finite elements assigned to the equation
	i >> name;
	i >> type;
	i >> matrixtypes;
    i >> nthreads;

    if ( nthreads.IsRead() ) nth = nthreads.GetParam();
	unsigned k;



// Creating Finite element equation object
	if ( type.GetParam().toString() == "linear_equation" )
        equation = new CFE_EquationQuasistaticLinear( FP.GetNodesDomain() , FP.GetElements() );

        if ( type.GetParam().toString() == "nonlinear_equation" )
        {
            CNamedParameter< CLangToken< IsValidToken >, '=' ,double > lf(string("load_factor"));
            CNamedParameter< CLangToken< IsValidToken >, '=' ,unsigned > steps(string("steps"));
            i >> lf >> steps;
        equation = new CFE_EquationQuasistaticNonlinear( FP.GetNodesDomain(), lf.GetParam(), steps.GetParam() );
        }


        if ( type.GetParam().toString() == "topology_optimization" )
            equation = new CFE_EquationTopologyOptimization( FP.GetNodesDomain(), FP.GetGroundElements(), FP.GetFiniteElements() );



	if ( !equation ) throw CFEException( "Unknown equation type :" + type.GetParam().toString() );

// Creating post processing object
            CPostProcessing *pp = new CPostProcessing;
            equation->SetPostProcessingObject( pp );

// Creating Large matrix object
	for (k=0; k<matrixtypes.GetParam().GetList().size(); k++)
	{
		CFEMatrix *fm = equation->FindMatrix( matrixtypes.GetParam().GetList()[ k ].GetName().toString() );
		CLargeMatrix *LM = 0;
		if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SkylineCholesky" ) LM = new CLargeMatrixSkyLineCholesky( );
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SkylineCholesky2" ) LM = new CLargeMatrixSkyLineCholesky2( );
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SkylineCrout" ) LM = new CLargeMatrixAsymmetricalCrout( );
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SparseCRJacoby" ) LM = new CLargeMatrixSparseCRJacoby( );
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SparseCR_CG" ) LM = new CLargeMatrixSparseCR_CG( );
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SparseCR_PCG_I" ) LM = new CLargeMatrixSparseCR_PCG( true );
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SparseCR_PCG_J" ) LM = new CLargeMatrixSparseCR_PCG( false );
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SparseStream_PCG" ) LM = new CLargeMatrixSparseStream_PCG( );
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SparseCR_GaussSeidel" ) LM = new CLargeMatrixSparseCR_GaussSeidel( );
#ifdef USE_PARDISO
        if ( matrixtypes.GetParam().GetList()[ k ].GetParam().toString() == "SparseCR_Pardiso" ) LM = new CLargeMatrixSparseCR_Pardiso( nth );
#endif
        if ( !LM ) throw CFEException( "Unknown matrix type :" + matrixtypes.GetParam().GetList()[k].GetParam().toString() );
        fm->SetMatrix( LM );
        fm->SetNumberOfThreads( nth );

	}


// Adding the equation to the database
	FP.AddEquation( name.GetParam().toString(), equation );

}

  
