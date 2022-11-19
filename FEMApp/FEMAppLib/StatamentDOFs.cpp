/* 
 * File:   CStatamentDOFs.cpp
 * Author: piotrek
 * 
 * Created on November 20, 2011, 4:58 PM
 */

#include "StatamentDOFs.h"
#include "../MathLib/Timer.h"
#include "../LangLib/statdef.h"

CStatementDOFs::CStatementDOFs( CFEMProject &f ):FP( f )  {
}


CStatementDOFs::~CStatementDOFs() {
}

void CStatementDOFs::Execute( istream &i )
{
    TokenParam name(string("bandwidth"),true,false);
    NamedParameterListTokenParameter2 create(string("create"),true,false);
    
    i >> name >> create;

    if (create.IsRead() )
    {
        unsigned k,l;
        CDOF *doftype = 0, *dof = 0;
        double sol=0.0;
        for (k=0; k<create.GetParam().GetList().size(); k++ )
        {
            string pname  = create.GetParam().GetList()[ k ].GetName().toString();
            string pvalue = create.GetParam().GetList()[ k ].GetParam().toString();

            if ( pname == "dof" )
            {
                if      ( pvalue == "ux" )  doftype = &CDOFs::Ux;
                else if ( pvalue == "uy" )  doftype = &CDOFs::Uy;
                else if ( pvalue == "uz" )  doftype = &CDOFs::Uz;
                else if ( pvalue == "t"  )  doftype = &CDOFs::T;
                else if ( pvalue == "c"  )  doftype = &CDOFs::C;
                else if ( pvalue == "d"  )  doftype = &CDOFs::D;
                else throw CFEException( "Unimplemented DOF named :'" + pvalue + "'" );
            }
            else if ( pname == "value" ) {
                sol = create.GetParam().GetList()[ k ].GetParam().toDouble() ;
            }
            else throw CFEException( "Unknown parameter :'" + pname + "'" );
        }
        for ( l = 0; l < FP.GetNodes().size(); l++ )
        {
            dof = FP.GetNodes( )[ l ]->FindDOF( doftype->GetType( ) );
            if ( dof == 0 )
            {
                dof = doftype->Clone();
                FP.GetNodes( )[ l ]->AddDOF( dof );
            }
            dof->SetValue( sol );
        }
    }

    if ( name.IsRead() )
    {
        CTimer timer;
        timer.Start();
        FP.CreateDOFs( name.GetParam().toString() );
        timer.Stop();
        cout << "DOF creation, and bandwidth optimization time= " << timer.GetFormattedTime() << endl;
    }
    else FP.CreateDOFs( "none" );
}
