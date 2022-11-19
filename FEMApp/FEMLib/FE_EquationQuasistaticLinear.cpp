
#include "FE_EquationQuasistaticLinear.h"
#include "../MathLib/ProgressConsole.h"
#include "ProfileReductionRCM.h"
#include <iostream>
#include <iomanip>

using namespace std;

CFE_EquationQuasistaticLinear::CFE_EquationQuasistaticLinear( DomainContainer< CFENode > &nd, vector< CFiniteElementInstance* > &e ):CFE_Equation( nd ),all_elements( e )
{
    K = CreateMatrix("K");
	P = CreateVector("P");
	q = CreateVector("q");
}

CFE_EquationQuasistaticLinear::~CFE_EquationQuasistaticLinear( void )
{

}

void CFE_EquationQuasistaticLinear::CreateDOFs( CProfileReduction *pr )
{
	valarray<unsigned> numerator;
    q->BuildDOFsVector( all_nodes.GetAll() );
//    unsigned ndof = pr ? EnumerateDOFs( q->GetUsedNodes(), pr->GetNumerator(), q->GetDOFTypesVector() ) : EnumerateDOFs(q->GetUsedNodes(),  numerator, q->GetDOFTypesVector() );
    // if ( ndof != q->GetDOFVector().size() ) throw CFEException("CFE_EquationQuasistaticLinear::CreateDOFsVector() : differend DOFs number (Enumeration and vector q)");
	P->SetDOFVector( q->GetDOFVector() );
}

void CFE_EquationQuasistaticLinear::Initialize()
{
    if ( q->GetDOFVector().size() == 0 )  throw CFEException("CFE_EquationQuasistaticLinear::Initialize() : DOFs number is 0. Nothing to solve");
	K->CreateMatrix( q->GetDOFVector().size() );

    postproces->RegisterStage("Analysis");
    postproces->RegisterStage("Aggregation");
    postproces->RegisterStage("Factorization");
    postproces->RegisterStage("Solve");

}

void CFE_EquationQuasistaticLinear::Solve()
{
    BeginStage( "Analysis" );

        timer.Start();
        CProgressConsole prg_agr("Stifness matrix aggregation"), prg_dec("Decomposition");

        BeginStage( "Aggregation" );
            P->Aggregate();
            K->Aggregate(&prg_agr); //K->Print();
            ApplyBoundaryConditions();
        EndStage();

        cout << endl;

        BeginStage( "Factorization" );
            if ( !K->GetMatrixObject().Decompose( &prg_dec ) ) throw CFEException("Matrix decomposition failed");
        EndStage();

        BeginStage( "Solve" );
            K->GetMatrixObject().Solve( *P, *q );
            q->ExtractDOFsSolution();
        EndStage();

    EndStage();

    timer.Stop();
    cout << endl << "Total analysis time:" << timer.GetFormattedTime() << endl;
}


void CFE_EquationQuasistaticLinear::ApplyBoundaryConditions()
{
	unsigned k;
	const CDOFVector &DOFs = q->GetDOFVector();
	for (k=0; k<DOFs.size(); k++)
        if ( DOFs[ k ]->IsFixed() )
        {
            double dv = K->GetMatrixObject().GetFactorC() * DOFs[ k ]->GetValue( );
            (*P)[ DOFs[k]->GetNumber() ] = dv;
        }
		else (*P)[ DOFs[k]->GetNumber() ] += DOFs[ k ]->GetValue();
    ApplyBoundaryConditionsToMatrix( K->GetMatrixObject(), q->GetDOFVector() );
}

