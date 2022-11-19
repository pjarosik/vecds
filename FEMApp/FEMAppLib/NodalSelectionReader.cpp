#include "NodalSelectionReader.h"
#include "../LangLib/statdef.h"
#include "../LangLib/LangException.h"

CNodalSelectionReader::CNodalSelectionReader( CFEMProject &fp, CSpace *sp ):selector(fp.GetNodesDomain()),space(sp),FP(fp)
{
}


void CNodalSelectionReader::read( istream &inp )
{
    unsigned k;
    EnumerationList2 param_numbers((string)"numbers",true,false);
    NamedParmeterTokenList2 param_domains((string)"domains",true,false);
    ExpressionVectorListParameter2 param_nearest((string)"nearest",true,false), param_incube((string)"incube",true,false);
    CLangToken< IsValidToken > param_insphere((string)"insphere");
    ExpressionVectorParameter x((string)"x");
    TokenParamDouble d((string)"radius");
    TokenParam param_space( (string)"space", true, false );
    NamedParameterListTokenParameterExpressions2 param_inplanes((string)"inplanes",true,false);
    NamedParameterOfNamedParmeterTokenListList param_inlines((string)"inlines",true,false);

    param_numbers.GetParam().SetMaxRange( FP.GetNodesDomain().size() );

    inp >> param_space >> param_numbers >> param_domains >> param_nearest >> param_incube >> param_insphere;
    if ( param_insphere.IsRead() ) inp >> x >> d;
    inp >> param_inplanes >> param_inlines;

    if ( param_numbers.IsRead() )
    {
        CNodeSelector::CSelectMethodByNumber sel( param_numbers.GetParam() );
        selector.AddSelector( sel );
    }


    if ( param_domains.IsRead() )
        for (k=0; k<param_domains.GetParam().GetList().size(); k++ )
        {
            string domain = param_domains.GetParam().GetList()[k].toString();
            CNodeSelector::CSelectMethodByDomain dom( domain );
            selector.AddSelector( dom );
        }
    if ( param_nearest.IsRead() )
    {
        vector< mvector > np;
        for (k=0; k<param_nearest.GetParam( ).GetList().size(); k++ )
            np.push_back( FP.EvaluateExpressionVector( param_nearest.GetParam( ).GetList()[ k ].GetList( ) ) );
        CNodeSelector::CSelectMethodNearests nn( np );
        selector.AddSelector( nn );
    }
    if ( param_incube.IsRead() )
    {
        const ExpressionVectorList &nodelist = param_incube.GetParam();
        if ( k<nodelist.GetList().size() != 2 ) throw CLangException( &inp, "Two points coordinates expected in rectangular selection list" );
        mvector p1( FP.EvaluateExpressionVector( nodelist.GetList()[0].GetList()) ), p2( FP.EvaluateExpressionVector( nodelist.GetList()[1].GetList() ) );
        CNodeSelector::CSelectMethodRectangular nn( p1, p2 );
        selector.AddSelector( nn );
    }
    if (param_insphere.IsRead())
    {
        mvector xs( FP.EvaluateExpressionVector( ( x.GetParam().GetList()  )));
        CNodeSelector::CSelectMethodPolar nn( xs, d.GetParam() );
        selector.AddSelector( nn );
    }
    if ( param_inplanes.IsRead() )
    {
        const NamedParameterListTokenExpressions &planes = param_inplanes.GetParam();
        vector< double > ax;
        vector< unsigned > cr;
        double eps=-1.0;
        bool haseps = false;
        unsigned k;
        for ( k=0; k<planes.GetList().size(); k++ )
            if ( planes.GetList()[k].GetName().toString() == "eps" ) {
                haseps = true;
                eps = FP.EvaluateExpression( planes.GetList()[ k ].GetParam().toString() );
            }
            else {
                unsigned c;
                if ( planes.GetList()[ k ].GetName() == "x" ) {
                   cr.push_back( 0 );
                   ax.push_back( FP.EvaluateExpression( planes.GetList()[ k ].GetParam().toString() )  );
                }
                else if ( planes.GetList()[ k ].GetName() == "y" ) {
                        cr.push_back(1);
                        ax.push_back( FP.EvaluateExpression( planes.GetList()[ k ].GetParam().toString() ) );
                     }
                     else if ( planes.GetList()[ k ].GetName() == "z" ) {
                            cr.push_back(2);
                            ax.push_back( FP.EvaluateExpression( planes.GetList()[ k ].GetParam().toString() ) );
                          }
                          else throw CLangException( &inp, "in_planes accept axis names { x, y or z } instead of :" + planes.GetList()[ k ].GetStringName( ) );

            }

        if ( haseps ) {
            CNodeSelector::CSelectMethodInPlanes np( ax, cr, eps  );
            selector.AddSelector( np );
        }
        else {
            CNodeSelector::CSelectMethodInPlanes np( ax, cr );
            selector.AddSelector( np );
        }
    }

    if ( param_inlines.IsRead() )
    {
        const NamedParmeterTokenListList &lines = param_inlines.GetParam();
        vector< double > ax1, ax2;
        vector< unsigned > cr1, cr2;
        double eps=-1.0;
        bool haseps = false;
        unsigned k;
        for ( k=0; k<lines.GetList().size(); k++ )
            if ( lines.GetList()[k].GetName().toString() == "eps" ) {
                haseps = true;
                eps = lines.GetList()[ k ].GetParam().GetList()[0];
            }
            else {
                unsigned c;
                string axis =  lines.GetList()[ k ].GetStringName();
                if ( axis=="xy" ) {
                cr1.push_back(0);
                cr2.push_back(1);
                ax1.push_back( lines.GetList()[ k ].GetParam().GetList()[0] );
                ax2.push_back( lines.GetList()[ k ].GetParam().GetList()[1] );
                }
                else if ( axis=="yz" ) {
                        cr1.push_back(1);
                        cr2.push_back(2);
                        ax1.push_back( lines.GetList()[ k ].GetParam().GetList()[0] );
                        ax2.push_back( lines.GetList()[ k ].GetParam().GetList()[1] );
                    }
                    else if ( lines.GetList()[ k ].GetName()=="xz" ) {
                            cr1.push_back(0);
                            cr2.push_back(2);
                            ax1.push_back( lines.GetList()[ k ].GetParam().GetList()[0] );
                            ax2.push_back( lines.GetList()[ k ].GetParam().GetList()[1] );
                        }
                        else throw CLangException( &inp, "in_lines accept axis names { xy, xz or yz } instead of :" + axis );

            }

        if ( haseps ) {
            CNodeSelector::CSelectMethodInLines np( ax1,ax2, cr1,cr2, eps  );
            selector.AddSelector( np );
        }
        else {
            CNodeSelector::CSelectMethodInLines np( ax1,ax2, cr1, cr2 );
            selector.AddSelector( np );
        }
    }

}
