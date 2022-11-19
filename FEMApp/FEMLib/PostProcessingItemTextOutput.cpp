#include "PostProcessingItemTextOutput.h"
#include <iomanip>
#include "../FEMLib/Renumber.h"
using namespace std;

CPostProcessingItemTextOutput::CPostProcessingItemTextOutput(
        const OutputDef &od
        ):CPostProcessingItemOutput( od ),frm("exp"), precision(8), size(20)
{
}

void CPostProcessingItemTextOutput::SetFormat( const string &t, unsigned s, unsigned prec )
{

    if ( t != "exp" && t != "float" && t != "fixed" )
        throw CFEException( "Unknow format type:"+t+ ". ext,float or fixed are supported");
    frm = t;
    size = s;
    precision = prec;

}


void CPostProcessingItemTextOutput::WriteSpacedText( ostream &out, const string &str )
{
    string::const_iterator i = str.begin();
    if ( i == str.end( ) ) return;
    out << *i;
    while( ++i != str.end() )
        out << ' ' << *i;

}

bool CPostProcessingItemTextOutput::FlushOutput( ostream &out, CFE_Equation *eq, bool IsBegin )
{

    unsigned k, l, m, esize = log10( OD.elements.size() ) + 1, nsize = log10( OD.nodes.size() ) + 1;
    if ( ((!IsBegin) && currentstage == "Analysis") || currentstage=="" )
    {
        out << "*************************************************************************************************" << endl;
        out << "*   F E M A P P                                                                                 *" << endl;
        out << "*   Finite Element Application                                                                  *" << endl;
        out << "*                                                                 (C) Piotr Tauzowski 2008-2014 *" << endl;
        out << "*************************************************************************************************" << endl << endl;

        out.precision(precision);
        out.setf( ios_base::scientific, ios_base::floatfield );


        for ( k = 0; k < OD.dataecho.size(); k++ )
        {
            if ( OD.dataecho[ k ] == "stat" )
            {

                out << "        " <<  setfill('.') << setiosflags( ios_base::left )
                    << setw(60)  << "NUMBER OF NODES   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                    << setw(20) << OD.nodes.size() << endl;

                out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                     << setw(60)  << "NUMBER OF ELEMENT TYPES   " <<   setfill(' ') << setiosflags( ios_base::right )
                     << setw(20) << OD.felements.size() << endl;

                out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                     << setw(60)  << "NUMBER OF ELEMENT INSTANCES   " <<   setfill(' ') << setiosflags( ios_base::right )
                     << setw(20) << OD.elements.size() << endl;

                out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                    << setw(60)  << "NUMBER OF MATERIALS   " <<   setfill(' ') << setiosflags( ios_base::right )
                    << setw(20) << OD.materials.size() << endl;

                out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                    << setw(60)  << "NUMBER OF DOMAINS   " <<   setfill(' ') << setiosflags( ios_base::right )
                    << setw(20) << OD.elements.DomainsNumber() << endl;

                out << endl << endl;
            }
            else if ( OD.dataecho[ k ] == "nodes" )
                    {
                        mvector X;
                        out << endl << endl;
                        WriteSpacedText(out,"*NODE COORDINATES*");
                        out << endl;

                        out << setw(nsize) << "No." << setw(25) << "X1" << setw(25) << "X2" << setw(25) << "X3" << endl;
                        for (l=0; l<OD.nodes.size(); l++ )
                        {
                            OD.nodes[l]->GetX(X);
                            out << "node " << setw(nsize) << l;
                            for (m=0; m<X.GetDim(); m++)
                                out << setw(25) << X[m];
                            out << endl;
                        }
                    }
                    else if ( OD.dataecho[ k ] == "FEAPmodel" )
                        {
                            double coords_scale = 1E-09;
                            ofstream ffout("FEAPmodel.txt");
                            ffout.precision(4);
                            ffout.setf( ios_base::scientific, ios_base::floatfield );
                            mvector X;
                            unsigned csize = 12;
                            ffout << endl << endl;
                            ffout << "coor" << endl;
                            for (l=0; l<OD.nodes.size(); l++ ) OD.nodes[l]->SetNumber( l + 1 );
                            for (l=0; l<OD.nodes.size(); l++ )
                            {
                                OD.nodes[l]->GetX(X);
                                ffout << setw(nsize) <<  OD.nodes[ l ]->GetNumber( ) << " 0 ";
                                for (m=0; m<X.GetDim(); m++)
                                    ffout << setw(csize) << coords_scale* X[m];
                                ffout << endl;
                            }

                            ffout << endl << endl;
                            ffout << "elem" << endl;

                            for (l=0; l<OD.elements.size(); l++)
                            {
                                ffout << resetiosflags( ios_base::right ) << setiosflags( ios_base::left ) << l + 1 << " 0 1";
                                ffout << resetiosflags( ios_base::left ) << setiosflags( ios_base::right );
                                string feapnum;
                                switch ( OD.elements[ l ]->GetNodes().size() ) {
                                    case 8:  feapnum = "FEAP_FEnum_3D_8.txt"; break;
                                    case 27: feapnum = "FEAP_FEnum_3D_27_2.txt"; break;
                                    case 64: feapnum = "FEAP_FEnum_3D_64.txt"; break;
                                    default: throw CFEException( "Unsupported node number :" ); break;
                                }
                                CRenumber ren( feapnum, OD.elements[ l ]->GetNodes().size() );
                                int nil=0;
                                for ( m=0; m<OD.elements[ l ]->GetNodes().size(); m++)
                                {
                                    if ( nil++ == 13 ) { nil = -1; ffout << endl; }
                                    ffout << " " << OD.elements[ l ]->GetNodes()[ ren.GetIndexWithOffset( m ) ]->GetNumber();
                                }
                                ffout << endl;
                            }

                            ffout << endl;
/*                            WriteSpacedText(ffout,"*NODAL DOF VALUES*");
                            ffout << endl << endl;
                            unsigned k, l;
                            for (k=0; k<OD.nodes.size(); k++)
                            {
                                    ffout << setw(nsize) <<  k+1;
                                    for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                                        if ( OD.nodes[ k ]->GetDofs()[l]->IsFixed() ) ffout  << setw(15) << OD.nodes[ k ]->GetDofs()[l]->GetValue();
                                        else ffout  << setw(15) << OD.nodes[ k ]->GetDofs()[l]->GetSolution();
                                    ffout << endl;
                             }

                            ffout << endl;
                            WriteSpacedText(ffout,"*NODAL DOF LOADS*");
                            ffout << endl << endl;
                            for (k=0; k<OD.nodes.size(); k++)
                                {
                                    bool fixed=false;
                                    for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                                        if ( !OD.nodes[ k ]->GetDofs()[ l ]->IsFixed() )
                                        {
                                            fixed = true;
                                            break;
                                        }
//                                        if ( !OD.nodes[ k ]->GetDofs()[l]->IsFixed() && OD.nodes[ k ]->GetDofs()[l]->GetValue() != 0.0 ) { fixed = true; break; }
                                    if ( fixed )
                                    {
                                        ffout << setw(nsize) <<  k+1;
                                        for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                                            if ( OD.nodes[ k ]->GetDofs()[l]->IsFixed() ) ffout  << setw(15) << 0.0;
                                            else ffout  << setw(15) << OD.nodes[ k ]->GetDofs()[l]->GetValue();
                                        ffout << endl;
                                    }
                                }

                            ffout << endl; */

                            ffout << "disp" << endl;
                            for (k=0; k<OD.nodes.size(); k++)
                            {
                                bool fixed=false;
                                for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                                    if ( OD.nodes[ k ]->GetDofs()[l]->IsFixed() ) { fixed = true; break; }
                                if ( fixed )
                                {
                                    ffout << setw(nsize) <<  OD.nodes[k]->GetNumber() << " 0 ";
                                    for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                                        ffout  << setw(csize) <<  OD.nodes[ k ]->GetDofs()[l]->GetValue();
                                    ffout << endl;
                                }
                            }

                            ffout << "boun" << endl;
                            for (k=0; k<OD.nodes.size(); k++)
                            {
                                bool fixed=false;
                                for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                                    if ( OD.nodes[ k ]->GetDofs()[l]->IsFixed() ) { fixed = true; break; }
                                if ( fixed )
                                {
                                    ffout << setw(nsize) <<  OD.nodes[k]->GetNumber() << " 0 ";
                                    for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                                        if ( OD.nodes[ k ]->GetDofs()[l]->IsFixed()) ffout << " 1 ";
                                        else ffout << " 0 ";
                                    ffout << endl;
                                }
                            }

                        }
                    else if ( OD.dataecho[ k ] == "felems" )
                            {
                                out << endl << endl;
                                WriteSpacedText(out,"*FINITE ELEMENT TYPES*");
                                out << endl << endl;
                                map_named_ptr< CFiniteElement >::const_iterator i;
                                for ( i = OD.felements.begin();  i != OD.felements.end(); i++ )
                                {
                                    CFiniteElement *fe = i->second;
                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "TYPE   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20) << fe->GetTypeName() << endl;

                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "NAME   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20) << i->first << endl;

                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "MATERIAL   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20) << fe->GetMaterial().GetTypeName() << endl;

                                        out << endl;
                                }
                            }
                            else if ( OD.dataecho[ k ] == "feinst" )
                                {
                                    out << endl << endl;
                                    WriteSpacedText(out,"*FINITE ELEMENT INSTANCES*");
                                    out << endl << endl;
                                    out << setw( esize ) << "    No." << setw( 40 ) << "node numbers" << endl;
                                    for (l=0; l<OD.nodes.size(); l++ ) OD.nodes[k]->SetNumber(l);
                                    for (l=0; l<OD.elements.size(); l++)
                                    {
                                        out << "elem:" << resetiosflags( ios_base::right ) << setiosflags( ios_base::left ) << setw( nsize + 1 ) << l;
                                        out << resetiosflags( ios_base::left ) << setiosflags( ios_base::right );
                                        for ( m=0; m<OD.elements[ l ]->GetNodes().size(); m++)
                                            out << setw( nsize + 1 ) << OD.elements[ l ]->GetNodes()[m]->GetNumber();
                                        out << endl;
                                    }
                                }
                                else if ( OD.dataecho[ k ] == "materials" )
                                {
                                    out << endl << endl;
                                    WriteSpacedText(out,"*MATERIALS*");
                                    out << endl << endl;
                                    // out << resetiosflags( ios_base::fixed ) << setiosflags( ios_base::scientific );
                                    map_named_ptr< CMaterial >::const_iterator i;
                                    for (i=OD.materials.begin(); i!=OD.materials.end(); i++)
                                    {
                                        CMaterial *mat = i->second;

                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "NAME   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20) << i->first << endl;

                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "TYPE   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20) << mat->GetTypeName() << endl;
                                        map< string, double >::const_iterator j;
                                        for ( j = mat->GetConstatnts().begin();  j != mat->GetConstatnts().end(); j++ )
                                            out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                            << setw(60)  << j->first + "     " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                            << setw(20) << j->second << endl;

                                        out << endl;

                                    }
                                 }
                                  else if ( OD.dataecho[ k ] == "output" )
                                    {
                                        out << endl << endl;
                                        WriteSpacedText(out,"*OUTPUT DEFINITION*");
                                        out << endl << endl;
                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "NAME   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20) << OD.name << endl;

                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "DATA ECHO   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20);
                                        for (l=0; l<OD.dataecho.size(); l++)
                                                out << OD.dataecho[l] << ", ";
                                        out << endl;

                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "EQUATIONS   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20);
                                        for (l=0; l<OD.eqs.size(); l++)
                                                out << "name" << ", ";
                                        out << endl;

                                        out << "        " <<  setfill('.') << resetiosflags( ios_base::right ) << setiosflags( ios_base::left )
                                        << setw(60)  << "STAGES   " <<  resetiosflags( ios_base::left ) << setfill(' ') << setiosflags( ios_base::right )
                                        << setw(20);
                                        for (l=0; l<OD.stages.size(); l++)
                                                out << OD.stages[l] << ", ";
                                        out << endl;

                                }

        }

    }


    out.setf( ios_base::scientific, ios_base::floatfield );
    out.precision(precision);


    out << "Stage :";
    WriteSpacedText(out,currentstage);

    if ( IsBegin ) out << " (begin) ";
    out<< endl << endl;


    if ( !IsBegin )
    {
        out << endl << endl;
        WriteSpacedText(out,"*NODAL DOF RESULTS*");
        out << endl << endl;

        OD.nsel.SetSelection( true );

        out << setw(10) << "No." << endl;
        unsigned k, l;
        for (k=0; k<OD.nodes.size(); k++)
            if ( OD.nodes[k]->IsSelected() )
            {
                out << "node:" << setw(nsize) <<  k;
                for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                    out << setw(4) << OD.nodes[ k ]->GetDofs()[l]->DofName() << "=" << setw(15) << OD.nodes[ k ]->GetDofs()[l]->GetSolution();
                out << endl;

                out << "     " << setw(nsize) <<  " ";
                for ( l = 0; l < OD.nodes[ k ]->GetDofs().size(); l++ )
                    out << setw(4) << OD.nodes[ k ]->GetDofs()[l]->LoadName() << "=" << setw(15) << OD.nodes[ k ]->GetDofs()[l]->GetValue();

                out << endl << endl;
            }

        OD.nsel.SetSelection( false );
        OD.esel.Select();
        if ( eq ) eq->RecomputeResults();

        out << endl << endl;
        WriteSpacedText(out,"*NODAL RESULTS*");
        out << endl << endl;

        for (k=0; k<OD.nodes.size(); k++)
            if ( OD.nodes[ k ]->IsSelected() )
            {
                map< string, double>::const_iterator i;
                const map< string, double> &results = OD.nodes[ k ]->GetResults();
                if ( results.size() )
                {
                    out << "node:" << setw(nsize) << OD.nodes[ k ]->GetNumber();
                    for (i=results.begin(); i!= results.end(); i++ )
                         out << setw( 6 ) << i->first << " = " << setw( 15 ) << i->second;
                    out << endl;
                }

            }

        OD.nsel.SetSelection( false );

        out << endl << endl;
        WriteSpacedText(out,"*EXTREMAL NODAL RESULTS*");
        out << endl << endl;

        map< string, double >   maxs, mins;
        map< string, unsigned > maxi, mini;
        unsigned maxind = 0;
        for (k=0; k<OD.nodes.size(); k++)
        {
              const map< string, double > &rs = OD.nodes[ k ]->GetResults();
              map< string, double >::const_iterator i;
              for ( i = rs.begin(); i != rs.end(); i++ )
              {
                if ( i->second > maxs[ i->first ] )  { maxs[ i->first ] = i->second; maxi[ i->first ] = k; if ( k > maxind ) maxind = k; }
                if ( i->second < mins[ i->first ] )  { mins[ i->first ] = i->second; mini[ i->first ] = k; if ( k > maxind ) maxind = k; }
              }

        }

        map< string, double >::const_iterator i, j=maxs.begin();
        out << setw(10) << "result" << setw( nsize + 2 ) << "node" << setw(20) << "min value" << setw( nsize + 6 ) << "node" << setw(20) << "max value" << endl;
        for (i=mins.begin(); i!=mins.end(); i++, j++)
            out << setw( 10 ) << i->first << setw( nsize + 2 ) << mini[ i->first ] << setw( 20 ) << i->second <<
                   setw( nsize + 6 )  << maxi[ j->first ] << setw( 20 ) << j->second << endl;

        out << endl << endl;
        WriteSpacedText(out,"*FINITE ELEMENT RESULTS*");
        out << endl << endl;
        map_named_ptr< CFiniteElement >::const_iterator ie;

        for ( ie = OD.felements.begin(); ie != OD.felements.end(); ie++ )
        {
            CFiniteElement *felem = ie->second;
            const map< string, CFEFunctorMatrix* > fres = felem->GetFEFunctorMatrixsResults();
            map< string, CFEFunctorMatrix* >::const_iterator i;
            const vector< FEDomain* >& domains = felem->GetDomains( );
            out << "Results for element :" << felem->GetTypeName() << endl;
            for (k=0; k<domains.size(); k++ )
            {
                for (l=0; l<domains[ k ]->size(); l++ )
                {
                    CFiniteElementInstance *ielem = (*domains[ k ])[ l ];
                    if ( ielem->IsSelected() )
                    {
                        out << "elem:" << ielem->GetNumber() << endl;
                        map< string, matrix > &ires = ielem->GetMatrixResults();
                        map< string, matrix >::const_iterator rfound;
                        for (i = fres.begin(); i != fres.end(); i++ )
                        {
                            rfound = ires.find( i->first );
                            if ( rfound != ires.end() )
                            {
                                const matrix &mr = rfound->second;
                                const CFEFunctorMatrix &rmf = *i->second;
                                unsigned row, col, dsize = 5, nsize = 20;
                                for ( row=0; row<rmf.GetRowsNames().size(); row++ ) if ( rmf.GetRowsNames()[row].size() > dsize ) dsize = rmf.GetRowsNames()[row].size();
                                out << setw( dsize ) << "Point in element";
                                for (col=0; col < mr.GetCols(); col++) out << setw(nsize) << rmf.GetColsNames()[col];
                                out << endl;
                                for ( row=0; row<mr.GetRows(); row++ )
                                {
                                    out << setw( dsize ) << (row < rmf.GetRowsNames().size() ? rmf.GetRowsNames()[row] : " ");
                                    for (col=0; col < mr.GetCols(); col++) out << setw( nsize ) << rmf(row,col);
                                    out << endl;
                                }
                            }
                        }
                    }
                }
            }
        }

        out << endl << endl;

    }

}
