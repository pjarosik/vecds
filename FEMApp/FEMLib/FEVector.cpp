#include <vector>
#include "FEFunctors.h"
#include "FEVector.h"
#include "FEAlgorithms.h"

CFEVector::CFEVector(void)
{
}

CFEVector::~CFEVector(void)
{
}

void CFEVector::AssignFiniteElements( CFEFunctorVector *vf, const vector< CFiniteElementInstance* > &fes )
{ 
	vector< CFiniteElementInstance* >::const_iterator i;
	vector< CFiniteElementInstance* > &v = VF[ (CFEFunctorVector*)vf->CloneTree() ];
	for ( i = fes.begin(); i != fes.end(); i++)	v.push_back( *i );
}

void CFEVector::Aggregate(bool from_dofs)
{
    mvector &V = *this;
    V=0;

	map< CFEFunctorVector*, vector< CFiniteElementInstance* > >::iterator i;	
	valarray< unsigned > al;
    unsigned k, tdim = GetDim();
    if ( GetDim() != DOFsVector.size() )
    {
        throw CFEException("CFEVector::Aggregate() - dimension of CFEVector and DOFs must be equal");
    }
	
// Aggregation from vector functors

	for ( i = VF.begin(); i!= VF.end(); i++ )
	{
		vector< CFiniteElementInstance* > &fes = i->second;
		CFEFunctorVector &vf = *i->first;
		vector< CFiniteElementInstance* >::iterator j;
		const mvector &mvf = vf;
		for ( j = fes.begin(); j != fes.end(); j++ ) 
		{
            vf.InvalidateStructure();
            vf( *j );
            vf.GetAllocationVector( al );
			for (k=0; k<mvf.GetDim(); k++)
				V[ al[ k ] ] += mvf[ k ];
		}
	}

    if ( !from_dofs )  return;

    // Aggregation from DOFs
	for (k=0; k<GetDim(); k++)
        if ( !DOFsVector[ k ]->IsFixed() ) V[  DOFsVector[ k ]->GetNumber()  ] += DOFsVector[ k ]->GetValue();
}


void CFEVector::BuildDOFsVector( vector< CFENode* > &all_nodes )
{

    SelectDOFsAndNodes(true);

    unsigned k,l, dofnumber=0;

    for (k=0; k<all_nodes.size(); k++)
    {
       CDOFVector &ndofs = all_nodes[k]->GetDofs();
       for (l=0; l<ndofs.size(); l++)
           if ( ndofs[ l ]->IsSelected() )
           {
               DOFsVector.push_back( ndofs[ l ] );
               ndofs[l]->SetSelection( false );
               ndofs[l]->SetNumber( dofnumber++ );
           }
       if ( all_nodes[k]->IsSelected() )
       {
           used_nodes.push_back( all_nodes[k] );
           all_nodes[k]->SetSelection( false );
       }
    }

    SetDim( DOFsVector.size() );
    SelectDOFsAndNodes( false );

}


void CFEVector::SelectDOFsAndNodes( bool state )
{
    map< CFEFunctorVector*, vector< CFiniteElementInstance* > >::iterator i;

    for ( i = VF.begin(); i!= VF.end(); i++ )
    {
        vector< CFiniteElementInstance* > &fes = i->second;
        vector< CFiniteElementInstance* >::iterator j;
        for ( j = fes.begin(); j != fes.end(); j++ )
        {
            (*j)->SelectDofsState( state );
            (*j)->SelectNodesState( state );
        }
    }
}

void CFEVector::ExtractDOFsSolution()
{
	mvector &v = *this;
	for ( unsigned k=0; k<GetDim(); k++)
		DOFsVector[ k ]->SetSolution( v[ DOFsVector[ k ]->GetNumber() ] );
}

void CFEVector::ApplyFixedBoundaryConditions( double mult )
{
	mvector &v = *this;
	unsigned k;
	for (k=0; k<GetDim(); k++)
            if ( DOFsVector[ k ]->IsFixed() ) v[ k ] = DOFsVector[ k ]->GetValue() * mult;
}


void CFEVector::Save( const string &fn, bool append)
{
    ofstream fout;
    if (append) fout.open(fn.c_str(),ios::app);
    else fout.open(fn.c_str());

    fout << GetDim() << endl;

    fout.setf( ios::scientific );
    fout.setf( ios::right );
    fout.precision( 15 );

    unsigned k;
    for(k=0; k<GetDim(); k++)
        fout << (*this)[k] << endl;

}

void CFEVector::Read( ifstream &fin )
{

    unsigned dm;
    fin >> dm;
    SetDim(dm);

    unsigned k;
    for(k=0; k<dm; k++)
        fin >> (*this)[k];

}
