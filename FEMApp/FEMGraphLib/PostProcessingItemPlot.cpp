#include "PostProcessingItemPlot.h"
#include "../GraphLib/GraphDeviceSizer.h"
#include "../MathLib/SpacePerspective.h"
#include "../MathLib/SpaceAxonometry.h"
#include "../FEMGraphLib/PaintFrameFECanvas.h"
#include "../GraphLib/GraphDevicePS.h"
#include "../GraphLib/GraphDeviceSVG.h"
#include "../GraphLib/GraphDeviceXML.h"
#include "../GraphLib/GraphDevicePrm.h"
#include "../FEMGraphLib/Contour.h"
#include "../FEMLib/FE_Equation.h"
#include "../FEMLib/Renumber.h"

CPostProcessingItemPlot::CPostProcessingItemPlot(const vector< string > &st,
                                                        const string &nm,
                                                        CFEMProject &fp,
                                                        const CPaintSetup &set):CPostProcessingItem( st ),setup( set ), FP( fp ),filename( nm )
{

}

bool CPostProcessingItemPlot::userProcessing( CFE_Equation *eq, bool IsBegin )
{
    if ( IsBegin ) return false;

    CPaintFrameFECanvas paintFEM( FP, setup );
    CGraphDevice *GDV = 0;

    switch ( setup.type ) {
        case FL_PS :    GDV = new GraphDevicePS( (filename + ".ps").c_str(),   &paintFEM ); break;
        case FL_SVG :   GDV = new GraphDeviceSVG( (filename + ".svg").c_str(), &paintFEM ); break;
        case FL_HTML :  GDV = new GraphDeviceSVG( (filename + "_svg").c_str(), &paintFEM ); break;
        case FL_PRM :   GDV = new CGraphDeviceStorage( filename.c_str() ,      &paintFEM ); break;
        case FL_MEM :   GDV = new CGraphDeviceStorage( filename.c_str(),       &paintFEM ); break;
        case FL_VTK :   FlushVTK(); return true;
    }

    if ( GDV == 0 ) return false;

    GDV->MakeContext();

    paintFEM.FitToGraphDevice( *GDV, 0, 0 );
    paintFEM.SetupPointView();
    GDV->SetViewSpace( paintFEM.GetSpaceView( ) );

    GDV->SetNullPen();
    GDV->AssignFont("Arial",15);

    paintFEM.Paint( *GDV );

    GDV->DeleteContext();
    delete GDV;

    return true;

}

void CPostProcessingItemPlot::FlushVTK()
{
    ofstream fout( filename.c_str() );

    if ( !fout.is_open() ) return;

    CPaintFrameFECanvas paintFEM( FP, setup );
    paintFEM.ComputeDispMax();
    CGraphDeviceSizer sizer;

    paintFEM.Paint( sizer );

    double diagonal = sizer.GetDiagonal3D();

    const vector< CFENode* > &nodes = FP.GetNodes();
    fout << "# vtk DataFile Version 1.0" << endl;
    fout << "2D Unstructured Grid of Linear Triangles" << endl;
    fout << "ASCII" << endl << endl << "DATASET UNSTRUCTURED_GRID" << endl;
    fout << "POINTS " << nodes.size() << " float" << endl;
    unsigned k, l, dim = nodes[0]->GetDim();
    double scale = 0.1 * diagonal / paintFEM.GetMaxDisp();
    mvector X;
    for (k=0; k< nodes.size(); k++)
    {
        nodes[k]->GetX(X);
        if ( setup.deformed )
        {
            for (l=0; l<X.GetDim(); l++)
                fout << X[l] + scale * nodes[k]->GetDofs()[l]->GetSolution() << " ";
        }
        else
            for (l=0; l<X.GetDim(); l++)
                fout << X[l] << " ";
        if ( l == 2 ) fout << 0.0 << " ";
        fout << endl;
    }

    unsigned cells_list_size = 0;

    const vector< CFiniteElementInstance* > elements = FP.GetElements();

    for ( k = 0; k < elements.size(); k++ )
        cells_list_size += 1 + elements[k]->GetNodes().size();

    vector< CRenumber > renums2D, renums3D;

    renums2D.push_back( CRenumber("VTK_2D_4.txt",4) );
    renums2D.push_back( CRenumber("VTK_2D_8.txt",8) );
    renums2D.push_back( CRenumber("VTK_2D_9.txt",9) );

    renums3D.push_back( CRenumber("VTK_3D_8.txt",8) );
    renums3D.push_back( CRenumber("VTK_3D_20.txt",20) );
    renums3D.push_back( CRenumber("VTK_3D_27.txt",27) );

    fout << endl << "CELLS " << elements.size() << " " << cells_list_size << endl;
    for ( k = 0; k < elements.size(); k++ )
    {
        unsigned nnodes = elements[k]->GetNodes().size();
        fout << nnodes << " ";
        CRenumber *ren=0;
        if ( dim == 2 )
            switch( nnodes )
            {
                case 4 :ren = &renums2D[0]; break;
                case 8 :ren = &renums2D[1]; break;
                case 9 :ren = &renums2D[2]; break;
            }
        if ( dim == 3 )
            switch( nnodes )
            {
                case 8 :ren = &renums3D[0]; break;
                case 20 :ren = &renums3D[1]; break;
                case 27 :ren = &renums3D[2]; break;
            }

        for ( l=0; l<nnodes; l++)
                fout << elements[k]->GetNodes()[ ren->InverseIndex(l) ]->GetNumber() << " ";
        fout << endl;
    }

    fout << endl << "CELL_TYPES "  << elements.size() << endl;

    for ( k = 0; k < elements.size(); k++ )
    {
        unsigned nnodes = elements[k]->GetNodes().size();
        if ( dim == 2 )
            switch( nnodes )
            {
                case 4 :fout << 9  << endl; break;
                case 8 :fout << 23  << endl; break;
                case 9 :fout << 28  << endl; break;
            }
        if ( dim == 3 )
            switch( nnodes )
            {
                case 8 :fout << 12  << endl; break;
                case 20 :fout << 25  << endl; break;
                case 27 :fout << 29  << endl; break;
            }
    }

    fout << endl << "POINT_DATA "  << nodes.size() << endl;
    for ( k = 0;  k < nodes[ 0 ]->GetDofs().size(); k++ )
    {
        fout << "SCALARS " << nodes[0]->GetDofs()[k]->DofName() << " float" << endl;
        fout << "LOOKUP_TABLE " << nodes[0]->GetDofs()[k]->DofName() << endl;
        for ( l = 0; l < nodes.size(); l++ )
        {
            fout << nodes[l]->GetDofs()[k]->GetSolution() << endl;
        }
        fout << endl;
    }

}
