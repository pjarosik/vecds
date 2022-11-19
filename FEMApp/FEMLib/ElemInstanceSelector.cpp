#include "ElemInstanceSelector.h"


CElemInstanceSelector::CElemInstanceSelector( const CElemInstanceSelector &ns ):elements( ns.elements ), selected( ns.selected ), selections( ns.selections ), select_all( ns.select_all )
{
    unsigned k;
    for( k = 0; k < selections.size(); k++ )
        selections[ k ] = selections[ k ]->Clone();
}


CElemInstanceSelector::~CElemInstanceSelector()
{
    unsigned k;
    for( k = 0; k < selections.size(); k++ )
    delete selections[ k ];
}

CElemInstanceSelector& CElemInstanceSelector::operator=( const CElemInstanceSelector &es )
{
    selected    = es.selected;
    lastnn      = es.lastnn;
    select_all  = es.select_all;
    selections = es.selections;
    unsigned k;
    for( k = 0; k < es.selections.size(); k++ )
        selections[ k ] = selections[ k ]->Clone();
}

void CElemInstanceSelector::CSelectMethodByNumber::SetSelection( const DomainContainer< CFiniteElementInstance > &ec, vector< bool > &selection )
{
    unsigned k;
    for (k=0; k<list.size(); k++)
        selection[ list[ k ]  ] = true;
}


void CElemInstanceSelector::CSelectMethodByDomain::SetSelection( const DomainContainer< CFiniteElementInstance > &ec, vector< bool > &selection )
{
    const vector< CFiniteElementInstance* > *dc = ec.GetDomain( dn );
    if ( !dc ) throw CFEException("Node domain named :"+ dn + " not found");
    vector< CFiniteElementInstance* > ::const_iterator i;
    for( i = dc->begin(); i != dc->end(); i++)
        selection[ (*i)->GetNumber() ] = true;
}


void CElemInstanceSelector::Select( )
{
    unsigned k;
    if ( selections.size() == 0 ) return;
    if ( lastnn != elements.size() )
    {
        selection.resize( elements.size() );

        for (k=0; k<selection.size(); k++)
        {
            selection[ k ] = select_all;
            elements[ k ]->SetNumber( k );
            elements[ k ]->SetSelection( selection[ k ] );
        }
        lastnn = elements.size();
    }

    for( k = 0; k < selections.size(); k++ )
           selections[ k ]->SetSelection( elements, selection );

    for (k=0; k<elements.size(); k++)
    {
        if ( selection[ k ] )
        {
            elements[k]->SetSelection( true );
            elements[k]->SelectNodesState( true );
        }
    }

}
