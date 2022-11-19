#include "StatementPlot.h"
#include "../LangLib/statdef.h"
#include "../FEMGraphLib/PaintSetup.h"
#include "../GraphLib/GraphDeviceSizer.h"
#include "../GraphLib/GraphDevicePS.h"
#include "../GraphLib/color.h"
#include "../GraphLib/PlotXY.h"
#include "../FEMGraphLib/PaintFrameFECanvas.h"
#include "../FEMGraphLib/PostProcessingItemPlot.h"
#include "../MathLib/SpacePerspective.h"
#include "../MathLib/SpaceAxonometry.h"
#include "TopOptFrame.h"
#include <list>

CStatementPlot::CStatementPlot( CFEMProject &fp  ):FP( fp )
{
}


void CStatementPlot::Execute( istream &i )
{
    unsigned k;
    TokenParamFilename par_filename(string("name")),par_projection(string("projection"),true,false);
    NamedParmeterTokenList2     par_types(string("types")),
                                par_stages(string("stages"),true,false),
                                par_content(string("content"),true,false),
                                par_settings(string("settings"),true,false),
                                par_eqs(string("eqs"),true,false);

    NamedParameterListTokenParameter2 par_parameters(string("parameters"),true,false), par_map(string("map"),true,false);

    i >> par_filename >> par_types >> par_eqs >> par_stages >> par_content >> par_settings >> par_projection >> par_parameters >> par_map;

    CPaintSetup paintsetup;
    if ( par_content.GetParam( ).GetList().size() )
    {
        paintsetup.paint_edges = false;
        paintsetup.paint_faces = false;
        paintsetup.paint_nodes = false;
        paintsetup.paint_node_numbers = false;
        paintsetup.paint_fe_numbers = false;
        paintsetup.paint_bcs = false;
    }


    for ( k=0; k<par_content.GetParam( ).GetList().size(); k++ )
    {
        string param = par_content.GetParam( ).GetList()[ k ].toString( );
        if ( param == "nodes" ) paintsetup.paint_nodes = true;
        else if ( param == "edges" ) paintsetup.paint_edges = true;
        else if ( param == "faces" ) paintsetup.paint_faces = true;
        else if ( param == "model" ) paintsetup.paint_model = true;
        else throw CFEException( "Unimplemented content type :'" + param + "'" );
    }

    for ( k=0; k<par_settings.GetParam( ).GetList().size(); k++ )
    {
        string param = par_settings.GetParam( ).GetList()[ k ].toString( );
        if ( param == "deformed" ) paintsetup.deformed = true;
        else if ( param == "contour" ) { paintsetup.contour = true; }
        else if ( param == "hollow" ) { paintsetup.hollow = true; }
        else if ( param == "interior" ) { paintsetup.boundary_only = false; }
        else if ( param == "deformation" ) { paintsetup.deformation = true; }
        else if ( param == "node_numbers" ) { paintsetup.paint_node_numbers = true; }
        else if ( param == "elem_numbers" ) { paintsetup.paint_fe_numbers = true; }
        else throw CFEException( "Unimplemented setting parameter :'" + param + "'" );
    }


    vector< string > st, cnt;

    if ( par_stages.IsRead() ) toStringVector( par_stages.GetParam().GetList(), st );
    if ( par_content.IsRead() ) toStringVector( par_content.GetParam().GetList(), cnt );
    if ( st.size( ) == 0 ) st.push_back("Analysis");

// Map settings parameter
    if ( par_map.IsRead() )
    {
        const NamedParameterListToken &maplist = par_map.GetParam();
        for (k=0; k< maplist.GetList().size(); k++)
        {
            const string &parname  = maplist.GetList()[k].GetName().toString();
            const string &parvalue = maplist.GetList()[k].GetParam().toString();
            if ( parname == "value") paintsetup.map = parvalue;
            else if ( parname == "n") paintsetup.contour_number = maplist.GetList()[k].GetParam().toUnsigned();
            else if ( parname == "degree") paintsetup.contour_degree = maplist.GetList()[k].GetParam().toUnsigned();
            else if ( parname == "paint_map")
            {
                if ( parvalue == "yes" ) paintsetup.paint_map=true;
                else if ( parvalue == "no" ) paintsetup.paint_map=false;
                else throw CFEException( "Parameter :'" + parname + "' should be 'yes' or 'no' only");
            }
            else if ( parname == "paint_contour")
            {
                if ( parvalue == "yes" ) paintsetup.paint_contours = true;
                else if ( parvalue == "no" ) paintsetup.paint_contours = false;
                else throw CFEException( "Parameter :'" + parname + "' should be 'yes' or 'no' only");
            }
            else if ( parname == "colors")
            {
                if      ( parvalue == "RED_YELLOW"  ) paintsetup.colormode = CM_RED_YELLOW;
                else if ( parvalue == "SPECTRUM"    ) paintsetup.colormode = CM_SPECTRUM;
                else if ( parvalue == "GREEN_RED"   ) paintsetup.colormode = CM_GREEN_RED;
                else if ( parvalue == "RED_GREEN"   ) paintsetup.colormode = CM_RED_GREEN;
                else if ( parvalue == "RED_BLUE"    ) paintsetup.colormode = CM_RED_BLUE;
                else if ( parvalue == "BLUE_RED"    ) paintsetup.colormode = CM_BLUE_RED;
                else if ( parvalue == "GRAY"        ) paintsetup.colormode = CM_GRAY;
                else throw CFEException( "Contour color paterrn :'" + parvalue + "' is not implemented");
            }
            else throw CFEException( "Unknown map settings parameter :'" + parname + "'");
        }
    }

// Equations parameter
    vector< CFE_Equation* > eqs;
    bool hasresult = false;
    string eqnames;
    for( k=0; k<par_eqs.GetParam().GetList().size(); k++ )
    {
        const string eqname = par_eqs.GetParam().GetList()[ k ].toString();
        eqs.push_back( FP.GetEquation( eqname ) );
        if ( paintsetup.map.size() && eqs.back()->HasResultNamed( paintsetup.map ) ) hasresult = true;
        eqnames += eqname;
        if ( k ) eqnames+=",";
    }
//    if ( paintsetup.map.size() && !hasresult  ) throw CFEException( "Result named :'" + paintsetup.map + "' is not used in equations :" + eqnames );


// Image types parameter
    const ParamList &list = par_types.GetParam();
    for ( k=0; k<list.GetList().size(); k++ )
    {
        string type = list.GetList()[ k ].toString();
        CPostProcessingItem *it = 0;
        if ( type == "ps") paintsetup.type = FL_PS;
        else if ( type == "svg") paintsetup.type = FL_SVG;
        else if ( type == "html") paintsetup.type = FL_HTML;
        else if ( type == "xml") paintsetup.type = FL_XML;
        else if ( type == "prm") paintsetup.type = FL_PRM;
        else if ( type == "mem") paintsetup.type = FL_MEM;
        else throw CFEException( "Unimplemented graphic file type :'" + type + "'" );

        FP.AddPostProcessingItem( it = new CPostProcessingItemPlot( st,
                                                                                    par_filename.GetParam().toString(),
                                                                                    FP,
                                                                                    paintsetup                                                                                    )
                );
        unsigned l, m;

        if ( eqs.size() )
        {
            if ( it )
                for (l=0; l<eqs.size(); l++) {
                    eqs[ l ]->AddPostProcessingItem( it );
                    for ( m = 0; m < st.size(); m++ )
                        if ( st[m] == "now" ) it->process( eqs[ l ] , st[m], false );
                }
        }
        else
            for ( m = 0; m < st.size(); m++ )
                if ( st[m] == "now" ) it->process( 0 , st[m], false );


    }


// --------------------------------------------------------------



    if (!FP.GetGroundElements().size()) return;


    unsigned gs = FP.GetGroundElements()[0].GetSize();
/*

    stringstream ss;
    unsigned gs = FP.GetGroundElements()[0].GetSize();
    ss << "TO_solution_";
    if ( gs < 10 ) ss << "0";
    ss << gs << ".txt";
    string filename = ss.str();

*/
    string filename = "TOPOLOGY_OPTIMIZATION_RESULTS_FILE.txt";

    vector< CTopOptFrame* > frames;
    CPlotLine lineC, lineV, lineVrel;

    ifstream fin( filename.c_str() );
    while( true )
    {
        CTopOptFrame *newframe=new CTopOptFrame(gs);
        if ( newframe->Read( fin ) ) frames.push_back( newframe );
        else { delete newframe; break; }
        lineC.AddY( newframe->GetC() );
        lineV.AddY( newframe->GetV() );
        lineVrel.AddY( newframe->GetVrel() );
    }

    if ( frames.size()==0 ) return;

    lineC.SetXRange(0,frames.size() );
    lineV.SetXRange(0,frames.size() );
    lineVrel.SetXRange(0,frames.size() );


    CPaintFrameFECanvas paintframe2( FP, paintsetup, 0, 0, 10, 10 );
    GraphDevicePS PSdevice2( (filename+".ps").c_str(),&paintframe2 );
    paintframe2.FitToGraphDevice( PSdevice2, 10, 10 );

    PSdevice2.MakeContext();
    PSdevice2.SetNullPen();

    for (k=0;k<FP.GetGroundElements().size(); k++)
    {
        double val = 1.0 - frames.back()->GetValue(k);
        DColor col(val,val,val);
        DBrush nbr( col, 0 );
        PSdevice2.SetBrush(nbr);
        paintframe2.PaintFiniteElements( PSdevice2, FP.GetGroundElements()[ k ].GetElements() );
    }

    DColor nodecolor(1,0,0);

//    FEpainter.PaintNodes( PSdevice2, FP.GetNodes(), nodecolor );
    PSdevice2.DeleteContext();


    GraphDevicePS plotC( (filename+"_C.ps").c_str() );
    plotC.SetAspect(16,9);
    plotC.MakeContext();
    CPlotXY pf_C( 0, 0, 200, 100, 0.67 );
    pf_C.SetAxialDescriptions( "Iterations",  "Compliance" );
    pf_C.GetSetup().has_description = false;

        lineC.SetDescription( "Compliance" );
        lineC.GetSetup( ).line_pen = DPen( DColor( 1.0, 0.0, 0.0 ), 0, 1.0  );
        lineC.GetSetup( ).point_freq = 1;
        lineC.GetSetup( ).has_line = true;
        lineC.GetSetup( ).point_type = PT_NOTHING;

    pf_C.AddPlotLine( lineC );
    pf_C.FitToGraphDevice( plotC );
    pf_C.Paint( plotC );
    plotC.DeleteContext( );



    GraphDevicePS plotV( (filename+"_V.ps").c_str() );
    plotV.SetAspect(16,9);
    plotV.MakeContext();
    CPlotXY pf_V( 0, 0, 200, 100, 0.67 );
    pf_V.SetAxialDescriptions( "iterations",  "volume" );
    pf_V.GetSetup().has_description = false;

        lineV.SetDescription( "Volume" );
        lineV.GetSetup( ).line_pen = DPen( DColor( 0.0, 1.0, 0.0 ), 0, 1.0  );
        lineV.GetSetup( ).point_freq = 1;
        lineV.GetSetup( ).has_line = true;
        lineV.GetSetup( ).point_type = PT_NOTHING;

    pf_V.AddPlotLine( lineV );
    pf_V.FitToGraphDevice( plotV );
    pf_V.Paint( plotV );
    plotV.DeleteContext( );



    GraphDevicePS plotVrel( (filename+"_Vrel.ps").c_str() );
    plotVrel.SetAspect(16,9);
    plotVrel.MakeContext();
    CPlotXY pf_Vrel( 0, 0, 200, 100, 0.67 );
    pf_Vrel.SetAxialDescriptions( "iterations",  "volume" );
    pf_Vrel.GetSetup().has_description = false;

        lineVrel.SetDescription( "Relative olume" );
        lineVrel.GetSetup( ).line_pen = DPen( DColor( 0.0, 0.0, 1.0 ), 0, 1.0  );
        lineVrel.GetSetup( ).point_freq = 1;
        lineVrel.GetSetup( ).has_line = true;
        lineVrel.GetSetup( ).point_type = PT_NOTHING;

    pf_Vrel.AddPlotLine( lineVrel );
    pf_Vrel.FitToGraphDevice( plotVrel );
    pf_Vrel.Paint( plotVrel );
    plotVrel.DeleteContext( );



}
