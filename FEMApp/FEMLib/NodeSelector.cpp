#include "NodeSelector.h"


void CNodeSelector::CSelectMethodNearests::SetSelection(  DomainContainer< CFENode > &nc, vector< bool > &sel )
{
    unsigned k, nearest=0;
    for (k=0; k<points.size(); k++)
    {
        unsigned l;
        mvector Xp;
        if ( space ) space->Transform(points[ k ],Xp);
        else Xp = points[k];
        double distmin = nc[0]->Distance( Xp );
        for (l=1; l<nc.size(); l++)
        {
            double dist = nc[ l ]->Distance( Xp );
            if ( dist < distmin )
            {
                distmin = dist;
                nearest = l;
            }
        }
        sel[ nearest ] = true;
    }
}


void CNodeSelector::CSelectMethodByNumber::SetSelection(  DomainContainer< CFENode > &nc, vector< bool > &sel )
{
    unsigned k;
    for (k=0; k<list.size(); k++)
        sel[ list[ k ] ] = true;
}


void CNodeSelector::CSelectMethodByDomain::SetSelection(  DomainContainer< CFENode > &nc,vector< bool > &sel )
{
    vector< CFENode* > *dc = nc.GetDomain( dn );
    if ( !dc ) throw CFEException("Node domain named :"+ dn + " not found");
    vector< CFENode* >::iterator i;

    for( i = dc->begin(); i != dc->end(); i++)
        sel[ (*i)->GetNumber() ] = true;

}


void CNodeSelector::CSelectMethodRectangular::SetSelection(  DomainContainer< CFENode > &nc, vector< bool > &sel )
{
    unsigned k,l;
    mvector X;
    for (k=0; k<nc.size(); k++)
    {
        nc[ k ]->GetX( X );
        for ( l=0; l<X.GetDim(); l++ )
            if ( ( X[ l ] - p1[ l ] ) * ( p2[ l ] - X[ l ] ) < 0.0 ) continue;
        sel[ k ] = true;
    }
}

void CNodeSelector::CSelectMethodPolar::SetSelection(  DomainContainer< CFENode > &nc, vector< bool > &sel )
{
    unsigned k,l;
    mvector X;
    for (k=0; k<nc.size(); k++)
    {
        nc[ k ]->GetX( X );
        for ( l=0; l<X.GetDim(); l++ )
            if ( nc[ k ]->Distance( X ) <= d ) sel[ k ] =  true;
    }
}

void CNodeSelector::CSelectMethodInPlanes::SetSelection(  DomainContainer< CFENode > &nd, vector< bool > &sel )
{
    unsigned k,l;
    mvector X;
    for (k=0; k<nd.size(); k++)
        for (l=0; l<planes.size(); l++)
        {
            mvector X;
            nd[ k ]->GetX( X );
            if ( fabs( X[ coords[ l ] ] - planes[ l ] ) < eps )
            {
                sel[ k ] = true;
                break;
            }
        }
}


void CNodeSelector::CSelectMethodInLines::SetSelection( DomainContainer< CFENode > &nd, vector< bool > &sel )
{
    unsigned k,l;
    mvector X;
    for (k=0; k<nd.size(); k++)
        for (l=0; l<x1.size(); l++)
        {
            mvector X;
            nd[ k ]->GetX( X );
            if ( ( fabs( X[ c1[ l ] ] - x1[ l ] ) < eps )  &&
                 ( fabs( X[ c2[ l ] ] - x2[ l ] ) < eps ) )
            {
                sel[ k ] = true;
                break;
            }
        }
}


CNodeSelector::CNodeSelector( DomainContainer<CFENode> &n, bool bsel ):nodes( n ), lastnn( 0 ),selection( n.size() ), select_all( bsel  )
{

}

CNodeSelector::CNodeSelector( const CNodeSelector &ns ):nodes( ns.nodes ), selections( ns.selections ), lastnn( ns.lastnn ), selection( ns.selection )
{
    unsigned k;
    for( k = 0; k < selections.size(); k++ )
        selections[ k ] = selections[ k ]->Clone();
}

CNodeSelector::~CNodeSelector()
{
    SetSelection( false );
    unsigned k;
    for( k = 0; k < selections.size(); k++ )
        delete selections[ k ];
}

const CNodeSelector& CNodeSelector::operator=( const CNodeSelector &ns )
{
    lastnn      = ns.lastnn;
    selection   = ns.selection;
    select_all  = ns.select_all;

    unsigned k;
    for( k = 0; k < selections.size(); k++ )
        selections[ k ] = selections[ k ]->Clone();
}

void CNodeSelector::SetSelection( bool st  )
{
    unsigned k;
    if ( selections.size() == 0 ) return;
    if ( lastnn != nodes.size() )
    {
        selection.resize( nodes.size() );
        for (k=0; k<selection.size(); k++)
        {
            selection[ k ] = select_all;
            nodes[k]->SetNumber(k);
        }
        for( k = 0; k < selections.size(); k++ )
            selections[ k ]->SetSelection( nodes, selection );
        lastnn = nodes.size();
    }
    for (k=0; k<selection.size(); k++)
        nodes[ k ]->SetSelection( selection[ k ] );
}


CFENode* CNodeSelector::Next()
{
    while ( ++ci < nodes.GetAll().size() )
        if ( nodes[ ci ]->IsSelected() ) return nodes[ ci ];
    return 0;
}
