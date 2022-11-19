#include "StatementElemDef.h"
#include "../LangLib/SubSetList.h"
#include "../LangLib/LangException.h"
#include "../FEMLib/FEFunctorVectorElemDOFs.h"
#include "../FEMLib/GaussPointsRectangular.h"
#include "../FEMLib/GaussPointsTriangular.h"

#include "../FEMLib/FiniteElement.h"
#include "../FEMLib/FiniteElementCoupledFields.h"
#include "../FEMLib/FiniteElementElasticPlane.h"
#include "../FEMLib/FiniteElementElasticSolid.h"
#include "../FEMLib/FiniteElementTruss3D.h"

#include "../FEMLib/FEFunctorMatrixMaterialPlaneStrainIzo.h"
#include "../FEMLib/FEFunctorMatrixMaterialPlaneStressIzo.h"
#include "../FEMLib/FEFunctorMatrixMaterialSolidAnizo.h"
#include "../FEMLib/FEFunctorMatrixMaterialSolidIzo.h"
#include "../FEMLib/FEFunctorMatrixMaterialSolidOrtho.h"

istream& operator >> ( istream &in, CStatementElemDef::CProblem &pr )
{
	char c;
	in >> c;	if ( c != '{' )	throw CLangException(&in," { expected" );
	in >> pr.type >> c >> pr.domain >> c >> pr.material;
	in >> c;	if ( c != '}' )	throw CLangException(&in," } expected" );
	return in;
}

CStatementElemDef::CStatementElemDef( CFEMProject &f ):FP( f )
{
	
//	FECreators[ "LE_PLANE" ] = 
//	FECreators[ "LE_FRAME2D" ] = 
//	FECreators[ "LE_FRAME3D" ] = 
//	FECreators[ "LE_TRUSS2D" ] = 
//	FECreators[ "LE_TRUSS3D" ] = 
//	FECreators[ "NLE_TRUSS2D" ] = 
//	FECreators[ "NLE_TRUSS3D" ] = 
//	FECreators[ "MT14" ] = 

//	FECreators[ "SHT_PLANE" ] = 
//	FECreators[ "THT_PLANE" ] = 
//	FECreators[ "SHT_SOLID" ] = 
//	FECreators[ "THT_SOLID" ] = 


}


CStatementElemDef::~CStatementElemDef(void)
{

}

void CStatementElemDef::Execute( istream &in )
{
    TokenParam  name(string("name")), type(string("type")), material(string("material")), equation(string("equation"));
	NamedParmeterTokenList2 domains(string("domains"));

    in >> name >> type >> domains >>  material >> equation;

    vector< FEDomain* > vdomains;
	unsigned k;
	for (k=0; k<domains.GetParam().GetList().size(); k++)
	{
        vector< CFiniteElementInstance* > *dme = FP.FindDomain( domains.GetParam().GetList()[ k ].toString() );
		if ( !dme ) throw CFEException( "Unknown domain :" + domains.GetParam().GetList()[ k ].toString() ); 
		vdomains.push_back( dme );
	}

	CMaterial *mat = FP.GetMaterial( material.GetParam().toString() );
	if ( !mat ) throw CFEException( "Material named :" + material.GetParam().toString()  + " not defined");

	CFE_Equation *eq = FP.GetEquation( equation.GetParam().toString() );

    CFiniteElement *fe = 0;

// *** LINEAR ELASTOSTATIC SOLID ELEMENT DEFINITION

    if ( type.GetParam().toString() == "LE_SOLID" ) // *** LINEAR ELASTOSTATIC SOLID ELEMENT DEFINITION
    {
            CFEFunctorMatrixMaterialSolidAnizo DAnizo;
            CFEFunctorMatrixMaterialSolidIzo DIzo;
            CFEFunctorMatrixMaterialSolidOrtho DOrtho;

            if ( mat->GetTypeName() == "Isotropic" ) fe = new CFiniteElementElasticSolid( vdomains, *mat, FP.GetIntegrationManager(), DIzo );
            else if ( mat->GetTypeName() == "Ortotropic" ) fe = new CFiniteElementElasticSolid( vdomains, *mat, FP.GetIntegrationManager(), DOrtho );
            else if ( mat->GetTypeName() == "Anisotropic" ) fe = new CFiniteElementElasticSolid( vdomains, *mat, FP.GetIntegrationManager(), DAnizo );
            else throw CFEException( "Unknown material type :" + mat->GetTypeName() );

    }
    else if ( type.GetParam().toString() == "LE_PLANE" ) // *** LINEAR ELASTOSTATIC PLANE ELEMENT DEFINITION
    {
            CFEFunctorMatrixMaterialPlaneStressIzo Dstress;
            CFEFunctorMatrixMaterialPlaneStrainIzo Dstrain;

            if ( mat->GetTypeName() == "PlaneStress" ) fe = new CFiniteElementElasticPlane( vdomains, *mat, FP.GetIntegrationManager(), Dstress );
            else if ( mat->GetTypeName() == "PlaneStrain" ) fe = new CFiniteElementElasticPlane( vdomains, *mat, FP.GetIntegrationManager(), Dstrain );
            else throw CFEException( "Unknown material type :" + mat->GetTypeName() );

    }
    else  if ( type.GetParam().toString() == "NE_ELEM14" )
    {
            fe = new CFiniteElementCoupledFields( vdomains, *mat, FP.GetIntegrationManager() );

    }
    else if ( type.GetParam().toString() == "NE_TRUSS3D" ) // *** NONLINEAR ELASTOSTATIC TRUSS 3D ELEMENT DEFINITION
    {
            fe = new CFiniteElementTruss3D( vdomains, *mat, FP.GetIntegrationManager() );
    }
    else throw CFEException( "Unknown finite element type :" + type.GetParam().toString() );


//	CFiniteElement *fe = new CFiniteElement( *creator, vdomains, *mat, FP.GetIntegrationManager() );

	FP.AddFiniteElement( name.GetParam().toString(), fe );
	fe->Create();
	eq->AssignFiniteElement( fe );

}

  




