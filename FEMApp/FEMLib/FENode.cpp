
#include "FENode.h"
#include "FEException.h"
#include "FiniteElementInstance.h"

//unsigned long nan[2]={0xffffffff, 0x7fffffff};
//double NaN = *( double* )nan;


CFENode::CFENode( const mvector &coor, CSpace *sp ):CGeometricObjectPoint( coor, sp ),nr( 0 ), weight( 0.0 ), dscale( -1.0 )
{
}

CFENode::~CFENode( void )
{
	unsigned k;
	for(k=0; k<DOFs.size(); k++) delete DOFs[ k ];
}

CDOF* CFENode::FindDOF( unsigned type )
{
	unsigned k;
	for(k=0; k<DOFs.size(); k++)
		if ( DOFs[ k ]->GetType() == type ) return DOFs[ k ];

	return 0;
}

void CFENode::ResetResults( const vector< string > &names )
{
    vector< string >::const_iterator i;
    for (i=names.begin(); i!= names.end(); i++ )
    {
        map< string, double >::iterator j = avalues.find( *i );
        if ( j != avalues.end() ) avalues.erase( j );
    }
}

 void CFENode::AccumulateResults( const vector<string> &names, const mvector &values, double wg )
 {
	 if ( names.size() != values.GetDim() ) throw CFEException("CFENode::AccumulateResults -  names - values differend vectors size");
     unsigned k;
     for(k=0; k<names.size(); k++)
             avalues[ names[ k ] ] = weight * avalues[ names[ k ] ] / ( weight + 1.0 ) + values[ k ] / ( weight + 1.0 );
	 weight += wg;
 }

void CFENode::AccumulateResult( const string &name, double value, double wg )
{
    avalues[ name ] = weight * avalues[ name ] / ( weight + 1.0 ) + value / ( weight + 1.0 );
}

double CFENode::GetResult( const string &nm ) const
{
    map< string, double >::const_iterator ind = avalues.find( nm );
    if ( ind == avalues.end() ) return DBL_MAX;
	return ind->second;
}

 void CFENode::UpdateGraph( const vector< CFENode* > &nd )
 {
     unsigned k;
     vector< CFENode* >::const_iterator j;
     for( j=nd.begin(); j!=nd.end();  j++ )
     {
        vector< CFENode* >::iterator i = find( graph_nodes.begin(), graph_nodes.end(), *j );
        if ( i == graph_nodes.end() && *j != this ) graph_nodes.push_back( *j );
     }
 }


 void CFENode::GetX( mvector &X ) const
 {
     if ( sp ) sp->Transform(x,X); else X=x;
     if ( dscale > 0.0 )
     {
        unsigned k,l;
        for (k=0; k<X.GetDim(); k++ )
            for(l=0; l<DOFs.size(); l++)
                if ( DOFs[l]->GetType() == k ) X[k] += dscale * DOFs[l]->GetSolution();
     }
 }

 bool compare_level( const CFENode *n1, const CFENode *n2 ) { return n1->GetGraphLevel() < n2->GetGraphLevel(); }
 
 void CFENode::SortGraph()
 {
     sort( graph_nodes.begin(), graph_nodes.end(), compare_level );
 }


 template <>
 void CSpaceShapeApproxPtr<CFENode>::transform( const mvector &ksi, mvector &X ) const
 {
     unsigned k,l;
     X.SetDim( Xdim );
     mvector N( ssp.GetXdim() ),Xp;
     ssp.transform(ksi,N);
     for (k=0; k<values.size(); k++)
     {
             values[k]->GetX( Xp );
              X += N[ k ] * Xp;
     }

 }

 template <>
 void CSpaceShapeApproxPtr<CFENode>::jacobian( const mvector &ksi, matrix &J ) const
 {
     matrix J1(Xdim,xdim);
     mvector Xp;
     unsigned k,l;
     for (k=0; k<Xdim; k++)
     {
         values[k]->GetX( Xp );
         for (l=0; l<xdim; l++)
             J1(k,l) = Xp[l];
     }
     ssp.jacobian(ksi,J);
     J = J * J1;
 }


