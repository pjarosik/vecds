#include "FE_EquationTopologyOptimization.h"
#include "FiniteElement.h"
#include "FEFunctorMatrixInt.h"
#include "../MathLib/ProgressConsole.h"
#include <iomanip>

CFE_EquationTopologyOptimization::CFE_EquationTopologyOptimization( DomainContainer< CFENode > &nd, vector< CGroundElement > &el, map_named_ptr< CFiniteElement > &fe, bool ic ):CFE_Equation( nd ),gelems( el ),IsConsequent( ic ), felements( fe ), qe(0),kf(0),ge_volume(0.0),csf(0)
{
    K = CreateMatrix("K");
    P = CreateVector("P");
    q = CreateVector("q");
}


CFE_EquationTopologyOptimization::~CFE_EquationTopologyOptimization( void )
{
    delete csf;
}

void CFE_EquationTopologyOptimization::Initialize()
{

    if ( felements.size() == 1 )
    {
        IsConsequent = false;
        CFiniteElement *fe = felements.begin()->second;
        kf = fe->GetFEFunctorMatrixs().find("K")->second;
        qe = fe->GetFEFunctorVectors().find("q")->second;
        CFiniteElementInstance *fei = (*fe->GetDomains()[0])[0];
        kf->InvalidateStructure();
        kf->ManageDOFs( fei );
        (*kf)( fei );
        Ke = (*kf);
        csf = new CFEFunctorScalarCompliance(Ke,0,qe);
        ge_volume = gelems[0].GetInstancesNumber() * (dynamic_cast<CFEFunctorMatrixInt*>(kf))->GetVolume();
    }
    else IsConsequent = true;

    if ( q->GetDOFVector().size() == 0 )  throw CFEException("CFE_EquationQuasistaticLinear::Initialize() : DOFs number is 0. Nothing to solve");
    K->CreateMatrix( q->GetDOFVector().size() );
    P->Aggregate();
    SelectFixedBCs( q->GetDOFVector() );

}

void CFE_EquationTopologyOptimization::Solve( )
{
    unsigned k, l, total_iter=0;
//    int framestep = 0;
    double tgmin = 1E-6, tgmax = 1.0, C = 0.0, Cmax, tm, tx, Vinit = 0.0, Volume = 0.0;
    cout << " * * * T O P O L O G Y    O P T I M I Z A T I O N * * *\n";
    cout << endl << endl;

// Initialization
    // finres.open("TO_solution.txt");
    // finres.close();

    for( l=0; l<gelems.size(); l++)
    {
        gelems[ l ].SetActive( true );
        gelems[ l ].SetThickness( tgmax );
    }

    cout << "Computing initial compliance...";

//    Initialize();

    SolveFEM();
    double norm = q->GetNorm();

    ofstream of("TOPOLOGY_OPTIMIZATION_RESULTS_FILE.txt");


    of.setf( ios::scientific );
    of.setf( ios::right );
    of.precision( 15 );

    of.close();

//    of << "q norm =" << setw(20) << an->q.GetLength() << "  qmax =" << setw(20) << an->q.AMaxAll() << endl;

// Maximum compliance calculation

    for( k=0; k<gelems.size(); k++)
    {
//		C += gelems[ k ].Calc_Rg( ) / gelems[ k ].GetT( );
        C += gelems[ k ].CalcCompliance(csf);
    }

    of << endl << endl;

    Cmax = 3.0 * C;
    cout << "C=" << C << " Cmax=" << Cmax << endl;
    vector<double> p;

    double d;

// Power parameter setting

//	for ( d = 1; d <= 3.1; d += 0.25 ) p.push_back( d );

    for ( d = 1; d <= 1.45; d += 0.1 ) p.push_back( d );
    for ( d = 1.5; d <= 3.0; d += 0.15 ) p.push_back( d );
    for ( d = 3.0; d <= 15.0; d += 0.25 ) p.push_back( d );


    vector<double> Agg( gelems.size() ), Rg( gelems.size() );

//    for( l=0; l<gelems.size(); l++)
  //      Vinit += Agg[ l ] = gelems[ l ].Calc_Vr( );

    for( l=0; l<gelems.size(); l++)
        Vinit += Agg[ l ] = ge_volume ; // gelems[ l ].GetT();
    cout << "Vinit = " << Vinit << endl;

// Optimization loop
    of.close();

    CTimer timer;

    timer.Start( gelems.size() );

    CProgressConsole prg_to("Topology optimization");

    prg_to.SetRange(gelems.size());

    for ( k=0; k<p.size(); k++)
    {
        // cout << "iterations for p=" << p[ k ] << endl;
        unsigned iter=0;
        unsigned ia=0, ip=0;
        bool changed;

        do {

            changed = false;
            double Sa = 0.0, Sp = 0.0, lm; tm = tgmax; tx = tgmin;
            C=0.0;

//  Lagrange multiplier calculations
            for( l=0; l<gelems.size(); l++ )
            {
                double Cge;
                Rg[ l ]  = gelems[ l ].Calc_Rg( csf, Cge );
                if ( gelems[ l ].IsActive( ) ) 	Sa += pow( Agg[ l ] / p[ k ], p[k] / ( p[ k ] + 1.0 ) ) * pow( Rg[ l ], 1.0 / ( p[ k ] + 1.0 ) );
                else  Sp += Rg[ l ] / gelems[ l ].GetT( );
                C += Cge;
            }

            lm = Sa / ( Cmax - Sp );

//			lm = pow( Sa, p[ k ]/( p[ k ] + 1.0 ) ) / ( Cmax - Sp );

            lm = pow( lm , ( p[ k ] + 1.0 ) /  p[ k ] );

            ia=0;
            ip=0;

// New thickness calculations

            Volume = 0.0;
            for( l=0; l<gelems.size(); l++)
            {
                double tgnew = pow( lm * p[ k ] * Rg[ l ] / Agg[ l ], p[ k ] / ( p[ k ] + 1.0 ) );

                if ( tgnew < tm ) tm = tgnew;
                if ( tgnew > tx ) tx = tgnew;

                if ( tgnew <= tgmin )
                {
                    if ( gelems[ l ].SetActive( false ) ) changed = true;
                    gelems[ l ].SetThickness( tgmin );
                    ip++;
                }

                if ( tgnew >= tgmax )
                {
                    if ( gelems[ l ].SetActive( false ) ) changed = true;
                    gelems[ l ].SetThickness( tgmax );
                    ip++;
                }

                if ( (tgnew > tgmin) && ( tgnew < tgmax ) )
                {
                    if ( gelems[ l ].SetActive( true ) ) changed = true;
                    gelems[ l ].SetThickness( tgnew );
                    ia++;
                }
                Volume += ge_volume * gelems[ l ].GetT(); //gelems[ l ].Calc_Vr() ;
            }

            //cout << "iteration=" << ++iter  << " active = " << ia << " pasive = " << ip << endl << " lm=" << lm << endl << endl << " tmax=" << tx << " tmin=" << tm << endl << endl <<" Sa=" << Sa << endl <<" Sp=" << Sp << endl <<" C=" << C << endl << " Cmax-Sp=" << Cmax-Sp << endl << endl;
            //cout << "V=" << Volume << " Vrel=" << Volume/Vinit << endl;

            double ptm;
            timer.EstimateTime(ip,ptm);
            // cout << "Estimated time :" << timer.FormatTime( 2.0 * ptm ) << endl;

// Making frame print

            // cout << "Saving state... :";

            of.open("TOPOLOGY_OPTIMIZATION_RESULTS_FILE.txt",ios::app);
             of << "iteration = " << ++total_iter << endl;
             of << "p = " << p[k] << endl;
             of << "active = " << ia << endl;
             of << "pasive = " << ip << endl;
             of << "C = " << C << endl;
             of << "V = " << Volume << endl;
             of << "Vrel = " << Volume/Vinit << endl;
             for( l=0; l<gelems.size(); l++)
                 of << gelems[l].GetT()  << " ";

             of << endl << endl;
             of.close();

             // cout << " OK" << endl; cout.flush();
             prg_to.NewValue(ip);

            SolveFEM();

        } while( changed && ia!=0 );
    }
    timer.Stop();
    Finalize();
    cout << "Optimization done" << endl<<endl;
    cout << "Vopt=" << Volume << " Vopt_rel=" << Volume/Vinit << endl<< endl;
    cout << "Solution time :" << timer.GetFormattedTime() << endl;
    ofstream fout( "Volres.txt", ios::app );
    fout << gelems.size() * 4 << "    " << Volume/Vinit << endl;;

}

void CFE_EquationTopologyOptimization::CreateDOFs( CProfileReduction *pr )
{
    q->BuildDOFsVector( all_nodes.GetAll() );
    P->SetDOFVector( q->GetDOFVector() );
}



void CFE_EquationTopologyOptimization::SolveFEM()
{
    //CProgressConsole prg_agr("Stifness matrix aggregation"), prg_dec("Decomposition");
    K->GetMatrixObject().SetZero();

    unsigned k;


    for (k=0; k<gelems.size(); k++)
        gelems[ k ].AggregateStiffnessMatrix( Ke, kf, K->GetMatrixObject() );

    K->GetMatrixObject().FindFactorC();

// Boundary Conditions

    P->Aggregate();
    const CDOFVector &DOFs = q->GetDOFVector();
    for (k=0; k<DOFs.size(); k++)
        if ( DOFs[ k ]->IsFixed() )
        {
            double dv = K->GetMatrixObject().GetFactorC() * DOFs[ k ]->GetValue( );
            (*P)[ DOFs[k]->GetNumber() ] = dv;
        }
//        else (*P)[ DOFs[k]->GetNumber() ] += DOFs[ k ]->GetValue();

    ApplyBoundaryConditionsToMatrix( K->GetMatrixObject(), dofs_fixed );
/*

    if (finres.is_open())
    {
        q->Read( finres );
    }
    else
    { */
        if ( !K->GetMatrixObject().Decompose( ) ) throw CFEException("Matrix decomposition failed");
        K->GetMatrixObject().Solve( *P, *q );
//        P->print("P test");
//        q->print("q test");
//    }
    q->ExtractDOFsSolution();
}

void CFE_EquationTopologyOptimization::Finalize()
{

}

void CFE_EquationTopologyOptimization::MakeFame( unsigned nfr )
{

}
