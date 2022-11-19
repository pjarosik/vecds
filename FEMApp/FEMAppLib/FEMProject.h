#pragma once

#include "../FEMLib/FiniteElement.h"
#include "../FEMLib/FENode.h"
#include "../FEMLib/FiniteElementInstance1D.h"
#include "../FEMLib/FiniteElementInstance2D.h"
#include "../FEMLib/FiniteElementInstance3D.h"
#include "../FEMLib/Material.h"
#include "../FEMLib/FE_Equation.h"
#include "../MathLib/SpaceShape.h"
#include "../MathLib/LargeMatrix.h"
#include "../FEMLib/IntegrationManager.h"
#include "../FEMLib/IntegrationPoints.h"
#include "../MolecularLib/MolecularStructure.h"
#include "../MolecularLib/Atom.h"
#include "../LangLib/Statement.h"
#include "../LangLib/LangStream.h"
#include "AtomAssignment.h"
#include "Model.h"
#include "../FEMLib/map_named.h"
#include "../FEMLib/GroundElement.h"
#include "../MathLib/DomainContainer.h"
#include "../MathLib/GeometricDomain.h"
#include "../MathLib/Expression.h"
#include "../GraphLib/GraphDevice.h"
#include "../MathLib/DBNode.h"
#include "../FEMLib/PostProcessItem.h"

class CFEMProject
{

public:

    class CPaintSetup
    {
        public:
        CPaintSetup( bool );
        bool paint_nodes, paint_edges, paint_faces, paint_bcs, paint_border_only, paint_node_numbers, paint_fe_numbers;
    };

	CFEMProject( void );
	~CFEMProject( void );

	void Reset();

	void ReadFile( const string &filename );

	template< class T >
	void CreateList( const string &slist, const vector< T* > &full, vector< T* > &list );
	
    void AddNode( const string &dm, CFENode *n ) { GD.AddGeometricObject(n,"FENode"); nodes.AddMember( n, dm ); }
    CFENode* AddUniqueNode( const string &dm, const mvector &xp );
    void AddElementInstance( const string &dm, CFiniteElementInstance *e ) { GD.AddGeometricObject(e,"FEElementInstance"); elements.AddMember( e, dm );  }
    void AddFiniteElement( const string &name, CFiniteElement *e ) { felements[ name ] =  e; }
	void AddMaterial( const string &nm, CMaterial *m ) { materials[ nm ] = m; }
    void AddSpaceShape( CSpaceShape *sf ) { shapefunctions.push_back( sf ); }
	void AddEquation( const string &nm, CFE_Equation *equation ) { equations[ nm ] = equation; }
    void AddGroundElement( const CGroundElement &ge ) { gelems.push_back( ge ); }
    void AddPostProcessingItem( CPostProcessingItem *pi ) { postprocitems.push_back(pi); }
    void AddSpace( const string &nm, CSpace *space );
	void CreateDOFs( const string &bran );
	void Solve();
	
    CModel& GetModel() { return model; }
    CSpace* FindSpace( const string &nm ) const;
    const vector<CFiniteElementInstance*>* FindDomain( const string &nm ) const { return elements.GetDomain( nm ); }
    vector<CFiniteElementInstance*>* FindDomain( const string &nm ) { return elements.GetDomain( nm ); }
	CMaterial* GetMaterial( const string &nm )  { return  materials.Find( nm ); }
    map_named_ptr< CMaterial >& GetMaterials() { return materials; }
	const CFiniteElement* GetFiniteElement( const string &nm ) const { return felements.Find( nm ); }
    CFE_Equation* GetEquation( const string &nm, bool ex = true );
    map_named_ptr< CFiniteElement >& GetFiniteElements() { return felements; }
    const vector< CFENode* >& GetNodes() const { return nodes.GetAll(); }
    const vector< CFiniteElementInstance* >& GetElements() const { return elements.GetAll(); }
    vector< CFENode* >& GetNodes() { return nodes.GetAll(); }
    vector< CFiniteElementInstance* >& GetElements() { return elements.GetAll(); }
	CIntegrationManager& GetIntegrationManager() { return IM; }
    vector< CGroundElement >& GetGroundElements() { return gelems; }
    CGeometricDomain& GetGeometricDomain() { return GD; }
    const CGeometricDomain& GetGeometricDomain() const { return GD; }
    CGeometricDomain& GetModelDomain() { return model.GetDomain(); }
    vector< CPostProcessingItem* > GetPostProcessingItems(  ) const { return postprocitems; }
    void LinkVariables( CExpression *expr ) const ;
    double EvaluateExpression( CExpression *expr ) const { LinkVariables( expr ); return expr->Compute(); }
    double EvaluateExpression( const string &se ) const { CExpression expr(se); LinkVariables( &expr ); return expr.Compute(); }
    mvector EvaluateExpressionVector( const vector< CLangToken< IsValidExpression > >  &ev ) const;

    const DomainContainer< CFENode >& GetNodesDomain() const { return nodes; }
    DomainContainer< CFENode >& GetNodesDomain() { return nodes; }

    const DomainContainer< CFiniteElementInstance >& GetElemsDomain() const { return elements; }
    DomainContainer< CFiniteElementInstance >& GetElemsDomain() { return elements; }


	void SetJobTitle( const string &t) { jobtitle=t; }
    void FindBoundaryObjects(bool set=true);

    static void RectangularMesh( const CSpaceShape &rs, const vector< mvector > &crn, unsigned nx, unsigned ny, unsigned nz );
	void ExecuteCommands( CLangStream  &ls );

	void GenerateRandomAtoms( const string &at, unsigned nb );
    void CreateMolecularConnectionStructure();
	void PrintResults( ostream &out );
    void VTK_Export( ostream &out, bool is_deformed  );
    CDBNode& GetDB() { return DB; }

protected:
    string jobtitle;

	CIntegrationManager IM;
	CMolecularStructure MC;
    CGeometricDomain GD;
    CModel model;
    CDBNode Variables, DB;


	template< class T >
	void delete_vector( vector< T* > &v );

    double max_disp, disp_scale;
    vector< double > mins, maxs, dls;

    DomainContainer< CFENode > nodes;
    DomainContainer< CFiniteElementInstance > elements;

	vector< CIntegrationPoints* > intpoints;
	vector< CLargeMatrix* > LargeMatrices;
    vector< CGroundElement > gelems;
    vector< CSpaceShape* > shapefunctions;
    vector< CPostProcessingItem* >  postprocitems;
    vector< CSpace* > spaces_rep;

	map_named_ptr< CFiniteElement > felements;
	map_named_ptr< CMaterial > materials;
	map_named_ptr< CFE_Equation > equations;

	vector< CAtomAssignment* > AS;
	map< CAtom*, CAtomAssignment* > ASM;
	map< string, CStatement* > stat;
    map< string, CSpace* > spaces;


};

