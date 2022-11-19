#pragma once

#include "../MathLib/matrix.h"
#include "../MathLib/LargeMatrix.h"
#include "TreeNode.h"
#include "FiniteElementInstance.h"
#include <string>
using namespace std;


template < class P >
class CFEFunctor : public CTreeNode
{
public:

	CFEFunctor( const string &nm );
	virtual ~CFEFunctor( void );

	void operator()( P *p );
	virtual void ManageDOFs( P *p ) = 0;
	ostream& PrintFunctor( ostream &out ) const;
	virtual void DebugPrint( ) const = 0;

	
protected:

	bool ComputeDependent;
	virtual void Compute( P *param  ) = 0;

};

template <class P>
ostream& operator << ( ostream &out, const CFEFunctor<P> &mf );

template <class P>
CFEFunctor<P>::CFEFunctor(const string &nm):CTreeNode( nm ),ComputeDependent(true)
{  }

template <class P>
CFEFunctor<P>::~CFEFunctor(void)
{  }


template <class P>
inline void CFEFunctor<P>::operator() ( P *param )
{
		if ( notvalid )
		{
			if ( ComputeDependent )
			{
				vector< CTreeNode* >::iterator i;
				for (i=depends.begin(); i!=depends.end(); i++)
				{
                    CFEFunctor<P> *ff = 0;
                    ff = dynamic_cast< CFEFunctor<P>* >( *i );
					ff->operator()( param );
				}
			}
			ManageDOFs( param );
			Compute( param ); 
			#ifdef _DEBUG
				if ( printdiag ) DebugPrint();
			#endif
			ValidateThis( ); 
		}
}

 
