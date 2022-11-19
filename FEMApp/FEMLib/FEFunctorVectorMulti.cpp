#include "FEFunctorVectorMulti.h"


//CFEFunctorVectorMulti::CFEFunctorVectorMulti(void)

CFEFunctorVectorMulti::CFEFunctorVectorMulti(  ):CFEFunctorVector("CFEFunctorVectorMulti")
{
   
     
}

CFEFunctorVectorMulti::CFEFunctorVectorMulti( const CFEFunctorVectorMulti &vf ):CFEFunctorVector( vf ),VF( vf.VF )
{
	unsigned k;
	for(k=0; k<VF.size(); k++) CLONE_MEMBER( vf, VF[ k ] )
}

void CFEFunctorVectorMulti::Add( CFEFunctorVector *vf ) 
{ 
		VF.push_back( vf ); 
		AddNode( vf ); 
		unsigned k;
		for (k=0; k<vf->GetNames().size(); k++)
			mnames.push_back( vf->GetNames()[ k ] );
}

void CFEFunctorVectorMulti::Compute( CFiniteElementInstance *fe1 )
{
	unsigned k,l,dim=0,ind=0;
	for (k=0; k<VF.size(); k++)
	{
		dim += VF[k]->GetDim();
	}
	
	fv.SetDim( dim );

	for (k=0; k<VF.size(); k++)
		for (l=0; l<VF[k]->GetDim(); l++)
			fv[ ind++ ] = (*VF[k])[l];
} 