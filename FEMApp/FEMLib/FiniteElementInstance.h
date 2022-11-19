#pragma once

#include "FEException.h"
#include "FENode.h"
#include "Material.h"
#include "../MathLib/matrix.h"
#include "DOFVector.h"
#include "../MathLib/SpaceShape.h"
#include "IntegrationManager.h"
#include "map_named.h"
#include "../MathLib/GeometricObjectShape.h"
#include "../MathLib/ScalarShapeSpaceApprox.h"
#include <vector>

using namespace std;

typedef CGeometricObjectShape<CFENode> CGeometricObjectShapeNodal;

/**
 * Base abstract class for finite element area
 */

class  CFiniteElementInstance : public CGeometricObjectShape< CFENode >
{
public:

	//! CFiniteElementInstance constructor
    /*!
	 \param n CFENode pointers vector.
    */
    CFiniteElementInstance( CGeometricDomain &gd, const CSpaceShapeApproxPtr< CFENode > &s );

	//! CFiniteElementInstance destructor
    /*!
	 \param CFENode pointers vector.
    */
	virtual ~CFiniteElementInstance(void);

	//! Clone functions
    /*!
		This member allows to create a copy of the object. Useful in mesh generation.
    */
	virtual CFiniteElementInstance* Clone( void ) const=0;


	//! Creates nodal elastic DOFs for this element
    /*!
		Function called by CStiffnessMatrix::BuildMatrix. Element creates necessary elastic CDOF objects in each node it cooperates with.
    */
	virtual void CreateDOFs( const CDOFVector &doftype );

	//! returns nodal DOFs for this element and given type
    /*!
		Function called by CStiffnessMatrix::BuildMatrix. Element creates necessary elastic CDOF objects in each node it cooperates with.
    */
	virtual void GetDOFs( const CDOFVector &doftype, CDOFVector &found_dofs );


    //! returns CShapeSpace object transforming natural coordinates to shape functions space
    /*!
    */
    const CSpaceShape& GetShapeFunctions( ) const { return main_shape->GetShapeSpace(); }

	//! returns set of integration points together with waights ready to integrate
    /*!
		\param IM - Reference to to CIntegrationManager;
    */
    virtual const CIntegrationPoints* GetIntegrationPoints( const CIntegrationManager &IM, int reduction = 0 );

	//! Sets number of the element
    /*!
		\param n number of the element
    */
	void SetNumber( const unsigned n ) { nr = n; }

	//! Material assignment
    /*!
		\param m CMaterial ponter
		Function assigns a material to this element
    */
	void SetMatConstants( CMaterial *m ) { material = m; }

    //! Returns names of field that can be interpolated by this instance
    /*!
        \param m CMaterial ponter
        Function return a string vectror with naes of fields.
    */
    vector< string > GetResultFieldsNames(  ) const;

    //! Returns Scalar, shapespace based approximator
    /*!
        \param rname results name
        Function return an aproximator for given result named rname
    */
    CScalarShapeSpaceApprox CreateResultApproximator( const string &rname ) const;

    //! Returns Scalar, shapespace based approximator
    /*!
        \param go component of the element ( edge, face )
        \param rname results name
        Function return an aproximator for given result named rname
    */
    CScalarShapeSpaceApprox CreateResultApproximator( const CGeometricObjectShapeNodal *go,  const string &rname ) const;

	//! Material accessor
    /*!
		\param m CMaterial ponter
		Function return a material assigned to the element
    */
	CMaterial* GetMaterial(  )  { return material; }

	//! CElements DOFs accessor
    /*!
		\return Function returns const reference to elements DOFs vector. Id is used by CreateProfile and BuildMatrix functions.
    */
	const vector< CDOFVector >& GetElemDofs() const { return dofs; }

	//! CElements nodes accessor
    /*!
		\return Function returns const reference to elements nodes vector.
    */
    const vector< CFENode* >& GetNodes() const { return main_shape->GetPoints(); }

    //! Global coordinate finder
    /*!
        \param xi global coordinates (input)
        \param X  local coordinates (output)
    */
    virtual void GetXi( const mvector &X, mvector &xi ) const;


	 //! Transforms gradient from local to cartesioan coordinates.
    /*!
        \param xi local coordinates (input)
		\param dXi gradient in local coordinates (input)
		\param dX gradient in global coordinates (output)
    */
//    virtual void TransformGradient( const mvector &xi, const mvector &dXi,  mvector &dX ) const;

	//! Calculates CartesianGradient
    /*!
        \param xi local coordinates (input)
		\param dN shape functions gradient in global ( cartesian ) coordinates (output)
    */
    virtual void GetGlobalSFGradient( const mvector &xi, matrix &dN ) const;

//! Calculates Jacoby matrix
    /*!
        \param xi local coordinates (input)
		\param Jacoby matrix (output)
    */
	virtual void GetJacobyMatrix( const mvector &xi, matrix &J ) const;

    //! Accessor to shape function object
    /*!
       \return Function rteturns const reference to CSpaceShape object;
    */
//    const CSpaceShape& GetShapeFunctions() const { return sh.GetSpace(); }

	//! Checks if a point is in the bounding box
    /*!
		\param x global coordinates point to test if it is in the bounding box
       \return true if a point is in the bounding box
    */
	bool IsInBoundingBox( const mvector &x );

	//! Checks if a point is inside finite element
    /*!
		\param X point in Lagrange coordinates
       \return true if a point is inside lagrange configuration.
    */
	bool IsInside( const mvector &X );

	//! Access to the bounding box
    /*!
		\param x global coordinates point to test if it is in the bounding box
       \return x1, x2 - min and max corner of bounding box
    */
	void GetBoundingBox(  mvector &a1, mvector &a2 ) { a1=x1;  a2=x2; }

    //! Approximates all nodal results values if some are missing using linear shape functions and vertex values.
    /*!
    */
    virtual void ApproximateAllNodalResults( ) = 0;

	//! Calculates  volume of the element
    /*!
	*/
	virtual double GetVolume( ) const { throw CFEException(" GetVolume( ) - not defined for this finite element "); }

	//! Calculates  area of finite elements (if applicable)
    /*!
	*/
	virtual double GetArea( ) const { throw CFEException(" GetArea( ) - not defined for this finite element "); }

	//! Calculates  length of the element (if applicable)
    /*!
	*/
	virtual double GetLength( ) const { throw CFEException(" GetLength( ) - not defined for this finite element "); }

	//! element number accesor.
    /*!
		Returns number of element
	*/
    unsigned GetNumber( ) const { return nr; }
        
        //! Nodal graph update
        /*!
        Function updates nodal connection graph used for bandwidth reduction.
        */
	void UpdateNodalGraph(  ) const;

    //! Nodal graph reset
    /*!
    Function resets nodal connection graph used for bandwidth reduction.
    */
    void ResetNodalGraph(  ) const;

    //! Selects all DOFs
    /*!
    Function selects all DOFs of this instance
    */
    void SelectDofsState( bool st );

    //! Selects all nodes
    /*!
    Function selects all nodes of this instance
    */
    void SelectNodesState( bool st );


    //! Set matrix results
    /*!
        Function adds or sets new result matrix
        \param name Name of given result matrix
    */
    void SetMatrixResult( const string &name, const matrix &mr ) { MR[ name ] = mr; }

    //! Set vector results
    /*!
        Function adds or sets new result vector
        \param name Name of given result vector
    */
    void SetVectorResult( const string &name, const mvector &vr ) { VR[ name ] = vr; }

    //! Get matrix results
    /*!
        Function returns named result matrix
        \param name Name of given result matrix
    */
    matrix& GetMatrixResult( const string &name ) { return MR[ name ]; }

    //! Get vector results
    /*!
        Function returns named result vector
        \param name Name of given result vector
    */
    mvector& GetVectorResult( const string &name ) { return VR[ name ]; }

    //! Get matrix results
    /*!
        Function returns all matrix results
    */
    map< string, matrix >& GetMatrixResults(  ) { return MR; }

    //! Get vector results
    /*!
        Function returns all vector results

    */
    map< string, mvector >& GetVectorResults( ) { return VR; }

    //! Clears vector and matrix results
    /*!
        Function clears matrix as well as vector results.
    */

    void ClearResults(  ) { MR.clear(); VR.clear(); }

    //! Checks if all nodes has result or DOF named with given name.
    /*!
        \param name  Name of result
    */

    bool HasResultNamed( const string &name );

    void GetPoints( vector< CGeometricObjectShapeNodal* > &gs, bool boundary = true  ) const { GetShapeObjects(0,gs,boundary); }
    void GetEdges(  vector< CGeometricObjectShapeNodal* > &gs, bool boundary = true )  const { GetShapeObjects(1,gs,boundary); }
    void GetFaces(  vector< CGeometricObjectShapeNodal* > &gs, bool boundary = true )  const { GetShapeObjects(2,gs,boundary); }

protected:
	//! Finds bounding box
    /*!
       Finds smallest rectangle surrounding the finite element;
    */

    //! returns CGeometricObjectShape objects, components of the element instance.
    /*!
          \param dim -dimension of component ( 0 - point, 1 edge, 2, face )
          \param gs container of response
          \param boundary, if true the function collects boundary objects only.
    */
    void GetShapeObjects(unsigned dim, vector< CGeometricObjectShapeNodal *> &gs, bool boundary = true ) const;

	void FindBoundingBox();
	vector< CDOFVector > dofs; /*!< CDOF pointers vector. Set of element DOFs. It is set of nodal DOFs of this element in one vector*/
    CMaterial *material; /*!< CMaterial pointer. Describes material properities of the element */
	mvector x1, x2;
	unsigned nr;

    map< string, matrix >     MR;
    map< string, mvector >    VR;

};

typedef vector< CFiniteElementInstance* > FEDomain;

