#pragma once

#include "../MathLib/matrix.h"
#include "../MathLib/GeometricObjectPoint.h"
#include "DOFVector.h"
#include <vector>
#include "EnrichmentFunctions.h"

using namespace std;

class CCrack;
/**
 * Class represents a node.
 */
class CFENode : public CGeometricObjectPoint
{
public:
	//! CFENode constructor
    /*!
	 \param coords vector of nodal coordinates
    */
    CFENode( const mvector &coords, CSpace *sp=0 );

	//! CFENode copy constructor
    /*!
	 \param n CFENode object being copied.
    */
    CFENode( const CFENode &n ):CGeometricObjectPoint(n),nr(n.nr),DOFs( n.DOFs ),avalues(n.avalues),weight(weight),dscale(n.dscale) {  }

	//! Assign operator
    /*!
	 \param n CFENode object being copied.
    */
//	const CFENode& operator =( const CFENode& n ) { x = n.x; nr = n.nr; DOFs = n.DOFs; return n; }
	~CFENode(void);

	//! Coordinate accessor
    /*!
	 \param c c-th coordinate is returned. ( 0-x, 1-y, 2-z )
	 \return value of coordinate.
    */
//	double GetCoord( unsigned c ) const { return x[ c ]; }

	//! Coordinate accessor (operator, const version)
    /*!
	 \param c c-th coordinate is returned. ( 0-x, 1-y, 2-z )
	 \return value of coordinate.
    */
//	double operator[](unsigned c ) const { return x[ c ]; }

	//! Coordinate accessor (operator, reference version)
    /*!
	 \param c reference of c-th coordinate is returned. ( 0-x, 1-y, 2-z )
	 \return reference to coordinate.
    */
//	double& operator[](unsigned c ) { return x[ c ]; }

	//! Coordinate accessor const vector reference version
    /*!
	 \return const reference to vector of coordinates is returned.
    */
//    const mvector& GetX() const { return x; }

	//! Distance to other node calculation
    /*!
	 \param n CFENode object.
	 \return distance from this object to n.
    */
	double Distance( const mvector &n ) const;

	//! Node number accessor
    /*!
	 \return node number (index, beginning form 0)
    */
    unsigned GetNumber() const { return nr; }

	//! Node number accessor
    /*!
	 \param n node number (index, beginning form 0)
	 Function sets the node number ( begins from 0 )
    */
	void SetNumber( unsigned n) { nr = n; }

	/*! DOF search.
	 \param type of DOFs constant. \sa DOF_TYPES
	 \return CDOF ponter in case the node already contains this type of DOF. Otherwise return 0.
    */
	CDOF* FindDOF( unsigned type );

	/*! DOF search.
	 \param type of DOFs constant. \sa DOF_TYPES
	 \return CDOF ponter in case the node already contains this type of DOF. Otherwise return 0.
    */
	void AddDOF( CDOF* dof ) { DOFs.push_back( dof ); }

	/*! Nodal DOFs vector accessor.
	 \return function returns nodal DOFs vector const reference.
    */
	const CDOFVector& GetDofs() const { return DOFs; }
	CDOFVector& GetDofs() { return DOFs; }

	CEnrichmentFunctions* GetEF() const { return tfn; }
	CCrack* GetCrack() const { return crack; }

    void SetEF( CEnrichmentFunctions* ef ) { tfn = ef; }
    void SetCrack( CCrack *c ) { crack = c; }

    void AccumulateResults( const vector< string > &names, const mvector &values, double wg = 1.0 );
    void AccumulateResult( const string &names, double v, double wg = 1.0 );
    void ResetResults( const vector< string > &names );
    void ResetResults( )
    {
        weight = 1.0; avalues.clear();
    }
    double GetResult( const string &nm ) const;
    const map< string, double>& GetResults() const {  return avalues; }
    void SetDefScale( double ds = -1.0 ) { dscale = ds; }
    virtual void GetX( mvector &X ) const;
    unsigned GetGraphLevel() const { return graph_nodes.size(); }
    void UpdateGraph( const vector< CFENode* > &nd );
    void ResetGraph(  ) {  graph_nodes.clear(); }
    void SortGraph();
    const vector< CFENode* >& GetGraphNodes() const { return graph_nodes; }
         
protected:

    unsigned nr;        /*!<    Node number (begins from 0).    */
    CDOFVector DOFs;    /*!<    Nodal vector of CDOF pointer    */
    map< string, double > avalues;
    double weight, dscale;
	CEnrichmentFunctions *tfn;
	CCrack *crack;
    vector< CFENode* > graph_nodes;

};

inline double CFENode::Distance( const mvector &n ) const
{
	double res = 0.0;
    mvector X;
    GetX( X );
    for ( unsigned k=0; k<X.GetDim(); k++ ) res += ( X[ k ] - n[ k ] ) * ( X[ k ] - n[ k ] );
	return sqrt( res );
}

typedef CSpaceShapeApproxPtr<CFENode> CSpaceShapeNodal;

