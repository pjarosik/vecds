#pragma once
#include "FiniteElementInstance.h"
#include "IntegrationManager.h"
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include "map_named.h"
#include <iostream>
#include "DOF.h"
#include "DOFs.h"



class CFiniteElement
{

public:

    CFiniteElement(
                    const string &tn,
                    const vector< FEDomain* > &vdom,
					CMaterial &mat,
                    CIntegrationManager &IM
                );

    virtual ~CFiniteElement( void );

    void AddVector( const string &nm, CFEFunctorVector &vf, bool clear = true )         { V[  nm  ] = dynamic_cast< CFEFunctorVector* > ( vf.CloneTree( clear ) ); }
    void AddMatrix( const string &nm, CFEFunctorMatrix &mf, bool clear = true  )        { M[  nm  ] = dynamic_cast< CFEFunctorMatrix* > ( mf.CloneTree( clear ) ); }
    void AddRHVector( const string &nm, CFEFunctorVector &vf, bool clear = true  )      { P[  nm  ] = dynamic_cast< CFEFunctorVector* > ( vf.CloneTree( clear ) ); }
    void AddResultVector( const string &nm, CFEFunctorVector &vf, bool clear = true  )  { VR[ nm  ] = dynamic_cast< CFEFunctorVector* > ( vf.CloneTree( clear ) ); }
    void AddResultMatrix( const string &nm, CFEFunctorMatrix &mf, bool clear = true  )  { MR[ nm  ] = dynamic_cast< CFEFunctorMatrix* > ( mf.CloneTree( clear ) ); }

	const vector< FEDomain* >& GetDomains() const { return vdomains; }
    void Create();
    const map< string, CFEFunctorMatrix* >& GetFEFunctorMatrixs()          { return M; }
    const map< string, CFEFunctorVector* >& GetFEFunctorVectors()          { return V; }
    const map< string, CFEFunctorMatrix* >& GetFEFunctorMatrixsResults()   { return MR; }
    const map< string, CFEFunctorVector* >& GetFEFunctorVectorResults()    { return VR; }

    CMaterial& GetMaterial() { return material; }
    const string& GetTypeName() const { return elemtypename; }
    void Print( ostream &fout ) const;
    void SetSelection( bool sel = true );

    void ComputeResults( bool selected = true );
    bool HasResultNamed( const string &name );

protected:

    const string elemtypename;
    vector< FEDomain* > vdomains;
	CMaterial &material;

    CIntegrationManager &IM;
    map< string, CFEFunctorMatrix* > M, MR;
    map< string, CFEFunctorVector* > V, P, VR;

    vector< CFEFunctorMatrix* > MatrixFunctors;
    vector< CFEFunctorVector* > VectorFunctors;

    virtual void CreateElement()  = 0;
    void ClearResults( CFiniteElementInstance *fei );

};

