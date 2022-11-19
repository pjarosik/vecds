#include "StatementOutput.h"
#include "../LangLib/statdef.h"
#include "NodalSelectionReader.h"
#include "ElemSelectionReader.h"
#include "../FEMLib/PostProcessingItemTextOutputOfstream.h"
#include "../FEMLib/PostProcessingItemTextOutputConsole.h"
#include "PostProcessingItemXMLOutput.h"

CStatementOutput::CStatementOutput( CFEMProject &fp ):FP( fp )
{
}


CStatementOutput::~CStatementOutput(void)
{
}


void CStatementOutput::Execute( istream &i )
{
    CLangToken< IsValidToken >      token_nodes("nodes"),
                                    token_elems("elems");
    TokenParam  par_name(   (string)"name"),
                pattern(    (string)"pattern");

    NamedParmeterTokenList2 par_types( (string) "types"),
                            par_stages(     (string) "stages",true,false),
                            par_dataecho(   (string) "dataecho",true,false),
                            nodal_values(   (string) "values",true,false),
                            elem_values(    (string) "values",true,false),
                            par_analysis(   (string) "analysis",true,false),
                            par_eqs;

    CNodalSelectionReader   ns( FP );
    CElemSelectionReader    es( FP );

    OutputDef od( FP.GetNodesDomain(), FP.GetElemsDomain(), FP.GetFiniteElements(), FP.GetMaterials() );

    i >> par_name >> par_types >> par_eqs >> par_stages >> par_dataecho;

    i >> token_nodes;
    if ( token_nodes.IsRead() )
        i >> ns; // >> nodal_values;

    i >> token_elems;

    if ( token_elems.IsRead() )
        i >> es; // pattern; // >> elem_values;

    i >> par_analysis;

    od.name = par_name.GetParam().toString();
    const ParamList &list = par_types.GetParam();

    unsigned k,m;
    for( k=0; k<par_eqs.GetParam().GetList().size(); k++ )
        od.eqs.push_back( FP.GetEquation( par_eqs.GetParam().GetList()[ k ].toString() ) );

    if ( par_dataecho.IsRead() ) toStringVector( par_dataecho.GetParam().GetList(), od.dataecho );

    od.esel = es.GetSelector();
    od.nsel = ns.GetSelector();

    if ( nodal_values.IsRead() ) toStringVector( nodal_values.GetParam().GetList(), od.nvals );
    if ( elem_values.IsRead() ) toStringVector( elem_values.GetParam().GetList(), od.evals );
    if ( par_stages.IsRead() ) toStringVector( par_stages.GetParam().GetList(), od.stages );
    if ( od.stages.size( ) == 0 ) od.stages.push_back("Analysis");

    for ( k=0; k<list.GetList().size(); k++ )
    {
        string type = list.GetList()[k].toString();
        CPostProcessingItem *it = 0;
        if ( type== "con") FP.AddPostProcessingItem( it = new CPostProcessingItemTextOutputConsole( od ) );
        else if ( type== "txt") FP.AddPostProcessingItem( it = new CPostProcessingItemTextOutputOfstream( od ) );
        else if ( type== "xml") FP.AddPostProcessingItem( it = new CPostProcessingItemXMLOutput( od, FP ) );
        else throw CFEException( "Unimplemented output type :'" + type + "'" );

        if (it)
            for (m=0; m<od.eqs.size(); m++)
                od.eqs[m]->AddPostProcessingItem( it );
        for ( m = 0; m < od.stages.size(); m++ )
            if ( od.stages[m] == "now" )
            {
                FP.FindBoundaryObjects(true);
                it->process( 0 , od.stages[m], false );
            }

    }

}
