#include "StatementElements.h"
#include "../MathLib/SpaceShapeLagrange2N_3D_8.h"
#include "../MathLib/SpaceShapeLagrange2N_1D_2.h"
#include "../MathLib/SpaceShapeLagrange3N_3D_27.h"
#include "../MathLib/SpaceShapeLagrange4N_3D_64.h"
#include "../LangLib/statdef.h"
#include "../FEMLib/Renumber.h"

CStatementElements::CStatementElements( CFEMProject &f ):FP( f )
{
}

CStatementElements::~CStatementElements( void )
{
}

void CStatementElements::Execute( istream &in )
{

	TokenParam	domain(string("domain")),
				type(string("type")),
				sf(string("sf"));

	TokenParamFilename	numbering(string("numbering"));

	UnsignedListWhite2 nodes(string("nodes"));

	in >> domain >> type >> sf >> numbering >> nodes;
	if ( nodes.GetParam().GetList().size() == 0 ) return;
    CSpaceShape *sfn = 0;

	if ( type.GetParam().toString() == "Solid" )
	{
        if ( sf.GetParam().toString() == "Lagrange3D_8" ) sfn = new CSpaceShapeLagrange2N_3D_8;
            else if ( sf.GetParam().toString() == "Lagrange3D_27" ) sfn = new CSpaceShapeLagrange3N_3D_27;
                 else if ( sf.GetParam().toString() == "Lagrange3D_64" ) sfn = new CSpaceShapeLagrange4N_3D_64;
                    else throw CFEException( "Unknown shape functions type:" + sf.GetParam().toString() );
	}
	else  if ( type.GetParam().toString() == "Truss" )
                {
                    if ( sf.GetParam().toString() == "Truss3D" ) sfn = new CSpaceShapeLagrange2N_1D_2;
                    else throw CFEException( "Unknown shape functions type:" + sf.GetParam().toString() );
                }
                else throw CFEException( "Unknown element type:" + type.GetParam().toString() );

    FP.AddSpaceShape( sfn );
    unsigned edim = sfn->GetXdim(); //, nsize =  nodes.GetParam().GetList().size();

    if ( nodes.GetParam().GetList().size()  % edim ) throw CFEException( "Malformed nodes's number list" );

	unsigned k, i = 0, nodenum;
    const vector< CFiniteElementInstance* > *ed = FP.FindDomain( domain.GetParam().toString() );
	if ( ed ) CFEException( "Domain :" + domain.GetParam().toString() + " already exists" );
    vector< CFENode* > enodes( edim );
    vector<  CFENode* > &allnodes = FP.GetNodes();
    CRenumber renum( numbering.GetParam().toString(), edim );


    unsigned elem_number = FP.GetElements().size();
	while ( i < nodes.GetParam().GetList().size() )
	{
		for (k=0; k<edim; k++) enodes[k] = 0;
//		unsigned elem_number = nodes.GetParam().GetList()[ i++ ];
		for (k=0; k<edim; k++)
		{
            nodenum = nodes.GetParam().GetList()[ i++ ];
			if ( nodenum >= allnodes.size() )
			{
				stringstream ss;
				ss << "Node index :" << nodenum << " exceeds nodes vector dimension :" << allnodes.size() << " index list:" << i;
				throw CFEException( ss.str() );
			}
            enodes[ renum[ k ] ] = allnodes[ nodenum ];
		}
		for (k=0; k<edim; k++)
			if ( !enodes[ k ]  )
			{
				stringstream ss;
				ss << "Malformed element list, not all nodes in elemend was assigned int element :" << elem_number;
				throw CFEException( ss.str() );
			}

		CFiniteElementInstance *elem = 0;
        CSpaceShapeApproxPtr< CFENode > *ssa = new CSpaceShapeApproxPtr< CFENode >( *sfn, enodes, enodes[0]->GetObjectDim() );
        FP.GetGeometricDomain().AddSpace( ssa );
        if ( type.GetParam().toString() == "Solid" ) elem = new CFiniteElementInstance3D( FP.GetGeometricDomain(), *ssa );
                if ( type.GetParam().toString() == "Truss" ) elem = new CFiniteElementInstance1D( FP.GetGeometricDomain(), *ssa );

		FP.AddElementInstance( domain.GetParam().toString(), elem ); elem_number++;
	}

}

