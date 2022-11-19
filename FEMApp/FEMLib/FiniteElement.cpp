#include "FiniteElement.h"
#include <iomanip>

CFiniteElement::CFiniteElement(const string &en,
                    const vector< FEDomain* > &vdm,
					CMaterial &mat,
                    CIntegrationManager &im
                    ):elemtypename(en), vdomains(vdm), material(mat), IM(im)
{

}


CFiniteElement::~CFiniteElement( void )
{
    unsigned k;
    vector< CFEFunctorVector* >::iterator i;
    for (i = VectorFunctors.begin(); i!= VectorFunctors.end(); i++)
            DeleteTree( *i );

    vector< CFEFunctorMatrix* >::iterator j;
    for (j = MatrixFunctors.begin(); j!= MatrixFunctors.end(); j++)
            DeleteTree( *j );
}


void CFiniteElement::Create()
{
	unsigned k, l;

    CreateElement();

    map< string, CFEFunctorVector* >::const_iterator i;
	for ( i = V.begin(); i != V.end(); i++ )
	{
		for (k=0; k<vdomains.size(); k++)
			for (l=0; l<vdomains[k]->size(); l++)
			{
				(*vdomains[k])[l]->SetMatConstants( &material );
				i->second->CreateDOFs( (*vdomains[k])[l] );
			}

	}
}


void CFiniteElement::Print( ostream &fout ) const
{


}


void CFiniteElement::SetSelection( bool sel )
{
    vector< FEDomain* >::iterator i;
    for ( i = vdomains.begin(); i != vdomains.end(); i++ )
    {
        vector< CFiniteElementInstance* >::iterator j;
        for ( j = (*i)->begin(); j != (*i)->end(); j++ )
            (*j)->SetSelection( sel );
    }
}

void CFiniteElement::ClearResults( CFiniteElementInstance *fei )
{
    fei->ClearResults();
    const vector< CFENode* > &nodes = fei->GetNodes();
    unsigned k;
    for ( k = 0; k < nodes.size(); k++ )
    {
        map_named_ptr< CFEFunctorMatrix >::iterator mres;
        for ( mres = MR.begin(); mres != MR.end(); mres++ )
            nodes[ k ]->ResetResults( mres->second->GetColsNames() );

        map_named_ptr< CFEFunctorVector >::iterator vres;
        for ( vres = VR.begin(); vres != VR.end(); vres++ )
            nodes[ k ]->ResetResults( vres->second->GetNames() );
    }

}

void CFiniteElement::ComputeResults( bool selected_only )
{
    vector< FEDomain* >::iterator i;
    for (i=vdomains.begin(); i!=vdomains.end(); i++)
    {
        vector< CFiniteElementInstance* >::iterator fe;
        for( fe = (*i)->begin(); fe != (*i)->end(); fe++ )
        {
            if ( !selected_only || (*fe)->IsSelected() == true )
            {
                ClearResults( *fe );
                map_named_ptr< CFEFunctorMatrix >::iterator mres;
                for ( mres = MR.begin(); mres != MR.end(); mres++ )
                {
                    CFEFunctorMatrix &res = *(*mres).second;
                    res.InvalidateStructure();
                    res( *fe );
                    ( *fe )->SetMatrixResult( (*mres).first, res );
                }

                map_named_ptr< CFEFunctorVector >::iterator vres;
                for ( vres = VR.begin(); vres != VR.end(); vres++ )
                {
                    CFEFunctorVector &res = *(*vres).second;
                    res.InvalidateStructure();
                    res( *fe );
                    ( *fe )->SetVectorResult( (*vres).first, res );
                }
            }

        }
    }
}

bool CFiniteElement::HasResultNamed( const string &name )
{
    vector< FEDomain* >::const_iterator i;
    for (i=vdomains.begin(); i!= vdomains.end(); i++)
    {
        FEDomain::const_iterator j;
        for (j =(*i)->begin(); j!=(*i)->end(); j++ )
            if ( (*j)->HasResultNamed( name ) ) return true;
    }


    map_named_ptr< CFEFunctorMatrix >::iterator mres;
    for ( mres = MR.begin(); mres != MR.end(); mres++ )
    {
        CFEFunctorMatrix &res = *(*mres).second;
        unsigned k;
        for (k=0; k<res.GetColsNames().size(); k++)
            if ( res.GetColsNames()[k] == name ) return true;
    }

    map_named_ptr< CFEFunctorVector >::iterator vres;
    for ( vres = VR.begin(); vres != VR.end(); vres++ )
    {
        CFEFunctorVector &res = *(*vres).second;
        unsigned k;
        for (k=0; k<res.GetNames().size(); k++)
            if ( res.GetNames()[k] == name ) return true;
    }

    return false;

}
