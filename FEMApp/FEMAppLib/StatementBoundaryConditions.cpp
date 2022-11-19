#include "StatementBoundaryConditions.h"
#include "../LangLib/NumberList.h"
#include "../FEMLib/DOFVector.h"
#include "NodalSelectionReader.h"

CStatementBoundaryConditions::CStatementBoundaryConditions( CFEMProject &f ):FP( f )
{
}

CStatementBoundaryConditions::~CStatementBoundaryConditions(void)
{
}

void CStatementBoundaryConditions::Execute( istream &i )
{

    TokenParam par_space((string)"space",true,false);

    i >> par_space;
    CSpace *space = 0;

    if ( par_space.IsRead() ) space = FP.FindSpace( par_space.GetParam().toString() );

    NamedParameterListTokenParameterExpressions2 values( string("values") );
    const NamedParameterListTokenExpressions &vl = values.GetParam();
    CNodalSelectionReader nodeselector( FP, space );

    i >> nodeselector >> values;

    nodeselector.GetSelector().SetSelection();
    FixNodes( vl );

}


void CStatementBoundaryConditions::FixNodes( const NamedParameterListTokenExpressions &vl )
{
	unsigned k,l,m;
    vector< CFENode* > &snodes = FP.GetNodes();
    for ( k=0; k<snodes.size(); k++)
        if ( snodes[ k ]->IsSelected() )
        {
            CDOFVector &ndofs = snodes[  k ]->GetDofs();
            for( m=0; m<vl.GetList().size(); m++)
            {
                CDOF *dof = 0;
                for (l=0; l<ndofs.size(); l++)
                {
                    if ( vl.GetList()[m].GetName().toString() == ndofs[l]->DofName() )
                                    {
                                        dof = ndofs[l];
                                        ndofs[l]->SetFixed( true, FP.EvaluateExpression( vl.GetList()[ m ].GetParam().toString() ) );
                                    }
                    else if ( vl.GetList()[m].GetName().toString() == ndofs[l]->LoadName() )
                                {	
                                    dof = ndofs[l];  
                                    ndofs[l]->SetValue( FP.EvaluateExpression( vl.GetList()[m].GetParam().toString() ) );
                                }
                }
                if ( !dof ) throw CFEException( "Unknown DOF name :" +  vl.GetList()[m].GetName().toString() );
            }
        }
}
