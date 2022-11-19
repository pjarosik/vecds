/*
 * File:   CFE_EquationQuasistaticNonlinear.cpp
 * Author: piotrek
 *
 * Created on September 9, 2010, 9:12 PM
 */

#include "FE_EquationQuasistaticNonlinear.h"

CFE_EquationQuasistaticNonlinear::CFE_EquationQuasistaticNonlinear( DomainContainer< CFENode > &n, double l, unsigned st  ):CFE_Equation( n ), qs( st ), steps( st ), lf( l )
{
    Kt = CreateMatrix("Kt");
    P = CreateVector("P");
    R = CreateVector("R");
    q = CreateVector("q");
}

CFE_EquationQuasistaticNonlinear::~CFE_EquationQuasistaticNonlinear() {
}

void CFE_EquationQuasistaticNonlinear::CreateDOFs( CProfileReduction *pr )
{
	valarray<unsigned> numerator;
    q->BuildDOFsVector( all_nodes.GetAll() );
    //unsigned ndof = pr ? EnumerateDOFs( q->GetUsedNodes(),  pr->GetNumerator(), q->GetDOFTypesVector() ) : EnumerateDOFs( q->GetUsedNodes(),  numerator, q->GetDOFTypesVector() ),
      //  qsize = q->GetDOFVector().size();
    //if ( ndof != qsize ) throw CFEException("CFE_EquationQuasistaticNonlinear::Initialize() : differend DOFs number (Enumeration and vector q)");
	P->SetDOFVector( q->GetDOFVector( ) );
	R->SetDOFVector( q->GetDOFVector( ) );
}

void CFE_EquationQuasistaticNonlinear::Initialize()
{
	Kt->CreateMatrix( q->GetDOFVector().size() );

    postproces->RegisterStage("Analysis");
    postproces->RegisterStage("Step");
    postproces->RegisterStage("NRiter");
    postproces->RegisterStage("Aggregation");
    postproces->RegisterStage("Factorization");
    postproces->RegisterStage("Solve");
}

void CFE_EquationQuasistaticNonlinear::Solve( )
{
    BeginStage("Analysis");

     P->Aggregate();
     mvector dq( q->GetDim() );
	 CFEVector dP;
	 mvector &vdP=dP, &vR = (*R);
	 dP.SetDOFVector( q->GetDOFVector() );

    unsigned k;
	double norm=0.0;
    for (k=0; k<steps; k++)
    {
        BeginStage("Step");
        mvector Pn = lf * ( k + 1.0 ) / double(steps) * (*P);
        double cnorm = Pn.GetNorm();
        cout <<  "cnorm = " << cnorm << endl;
        vdP=( 1.0 / double(steps) * Pn );
        do {
                BeginStage("NRiter");
                    BeginStage("Aggregation");
                        Kt->Aggregate();
                          Kt->GetMatrixObject().FindFactorC();
                        ApplyBoundaryConditionsToMatrix( Kt->GetMatrixObject(), q->GetDOFVector() );

                        R->Aggregate(false);
                        vdP  =  Pn - vR;

                        dq = 0;

                        dP.ApplyFixedBoundaryConditions( Kt->GetMatrixObject().GetFactorC() );
                    EndStage();

                    // Kt->Print();
                    BeginStage("Factorization");
                        if ( !Kt->GetMatrixObject().Decompose( ) ) throw CFEException("Tangent matrix decomposition failed");
                    EndStage();

                    BeginStage("Solve");
                        Kt->GetMatrixObject().Solve( dP, dq );
                    EndStage();
                    (*q) += dq;
                    q->ExtractDOFsSolution();

                    norm = vdP.GetNorm();
                EndStage();
                cout <<  "norm=" << norm << " norm/cnorm=" << norm / cnorm << endl; cout.flush();
        } while( norm / cnorm > 1e-5 );

        qs[ k ] = (*q);
        EndStage();
    }
    EndStage();

}
