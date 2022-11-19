#include "PostProcessingItemXMLOutput.h"
#include <sstream>
#include <fstream>
#include "../XMLLib/XML.h"
#include "../MathLib/matrix.h"

CPostProcessingItemXMLOutput::CPostProcessingItemXMLOutput(const OutputDef &od, CFEMProject &fp ):CPostProcessingItemOutput( od ), precision( 8 ), size( 20 ), FP( fp )
{
}

 bool CPostProcessingItemXMLOutput::userProcessing( CFE_Equation *eq, bool IsBegin  )
 {
     unsigned k, l, m;
     //CXML xml;
     //CXML_OutputDevice od( OD.name + "_vvect.txt" );
     //CXML_Creator creator( od );
     ofstream fout( (OD.name + "_vvect.txt").c_str() );

     CGeometricDomain &GDC = FP.GetGeometricDomain();
     const vector< CGeometricObject* > &ago = GDC.GetAllGeometricObjects();

     fout << "FEM_DATA" << endl;
     fout << OD.name << endl;
     for (k=0; k<OD.dataecho.size(); k++ )
     {
         if ( OD.dataecho[ k ] == "nodes" )
         {
             fout << OD.nodes.size() << endl;
                 for ( l = 0; l < OD.nodes.size(); l++)
                 {
                     OD.nodes[l]->SetNumber(l);
                     mvector X;
                     OD.nodes[ l ]->GetX( X );
                     for( m = 0; m < X.GetDim(); m++ ) fout << X[ m ] << " ";
                     if ( OD.nodes[ l ]->IsBoundary() ) fout << "yes ";
                     else fout << "no ";

                     fout << OD.nodes[ l ]->GetDofs().size() << " ";
                     for (m=0; m<OD.nodes[ l ]->GetDofs().size(); m++)
                         fout <<  OD.nodes[ l ]->GetDofs()[m]->DofName() << " " << OD.nodes[ l ]->GetDofs()[m]->GetSolution() << "    ";

                     fout << endl;
                 }
         } else if ( OD.dataecho[ k ] == "felems" ) {
             fout << endl << endl << endl << OD.elements.size() << endl;
             for ( l = 0; l < OD.elements.size(); l++)
             {

                         fout << (int)OD.elements[l]->GetNodes().size() << " ";
                         for( m = 0; m < OD.elements[l]->GetNodes().size(); m++ )
                             fout << " " << OD.elements[l]->GetNodes()[ m ]->GetNumber();
                         fout << endl;
                 }
         }
     }
     fout << endl << endl;
/*     for ( l = 0; l < OD.nodes.size(); l++)
     {
             fout << OD.nodes[ l ]->GetDofs().size() << " ";
             for (m=0; m<OD.nodes[ l ]->GetDofs().size(); m++)
                 fout <<  OD.nodes[ l ]->GetDofs()[m]->DofName() << " " << OD.nodes[ l ]->GetDofs()[m]->GetSolution() << "    ";
             fout << endl;
     } */
 /*            const map<string,double> &results = OD.nodes[ l ]->GetResults();
             map<string,double>::const_iterator i;
             for (i=results.begin(); i!=results.end(); i++)
             {
                 creator.OpenNode( "Result" );
                     creator.AddParameter("name", i->first );
                     creator.AddParameter("value",i->second );
                 creator.CloseNode();
             } */

     /*
     creator.AddHeader("version=\"1.0\"");
     creator.OpenNode("FEMAppModel");
        creator.AddParameter( "name", OD.name );
        for (k=0; k<OD.dataecho.size(); k++ )
        {
            if ( OD.dataecho[ k ] == "nodes" )
            {
                creator.OpenNode("nodes");
                    creator.AddParameter("number",(int)OD.nodes.size());
                    for ( l = 0; l < OD.nodes.size(); l++)
                    {
                        OD.nodes[l]->SetNumber(l);
                        creator.OpenNode("Node");
                            if ( OD.nodes[ l ]->IsBoundary() ) creator.AddParameter("boundary","yes");
                            else creator.AddParameter("boundary","no");
                            mvector X;
                            OD.nodes[ l ]->GetX( X );
                            for( m = 0; m < X.GetDim(); m++ )
                            {
                                stringstream ss;
                                ss << "x" << m+1;
                                creator.AddParameter(ss.str(),X[m]);
                            }
                        creator.CloseNode();
                    }
                creator.CloseNode();
            } else if ( OD.dataecho[ k ] == "felems" ) {
                creator.OpenNode("elements");
                    creator.AddParameter("number",(int)OD.elements.size());
                    for ( l = 0; l < OD.elements.size(); l++)
                    {
                        creator.OpenNode("elem");
                            creator.AddParameter("n",(int)OD.elements[l]->GetNodes().size());
                            stringstream ss;
                            for( m = 0; m < OD.elements[l]->GetNodes().size(); m++ )
                                ss << " " << OD.elements[l]->GetNodes()[ m ]->GetNumber();
                            creator.AddText( ss.str() );
                        creator.CloseNode();
                    }
                creator.CloseNode();
            }
        }
        creator.OpenNode("NodalResults");
        for ( l = 0; l < OD.nodes.size(); l++)
        {
            creator.OpenNode( "node" );
                creator.AddParameter( "number", (int) l );
                for (m=0; m<OD.nodes[ l ]->GetDofs().size(); m++)
                {
                    creator.OpenNode( "Result" );
                        creator.AddParameter("name", OD.nodes[ l ]->GetDofs()[m]->DofName() );
                        creator.AddParameter("value", OD.nodes[ l ]->GetDofs()[m]->GetSolution() );
                    creator.CloseNode();
                }
    /*            const map<string,double> &results = OD.nodes[ l ]->GetResults();
                map<string,double>::const_iterator i;
                for (i=results.begin(); i!=results.end(); i++)
                {
                    creator.OpenNode( "Result" );
                        creator.AddParameter("name", i->first );
                        creator.AddParameter("value",i->second );
                    creator.CloseNode();
                } */

            //creator.CloseNode();
        //}
//        creator.CloseNode();

  //   creator.CloseNode();

 }
