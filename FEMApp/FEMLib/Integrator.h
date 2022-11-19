// Integrator.h: interface for the CIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTEGRATOR_H__9E4938E1_CF4E_4A71_A9B8_DF6B7D58E9BE__INCLUDED_)
#define AFX_INTEGRATOR_H__9E4938E1_CF4E_4A71_A9B8_DF6B7D58E9BE__INCLUDED_

//#include "Adv_math.h"	// Added by ClassView
#include "IntegrationPoints.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template< class T, class I >
class CIntegrator  
{
public:

	CIntegrator( const CIntegrationPoints &g ):gps(g) { SetBegin(); }
	virtual ~CIntegrator( );
	bool Integrate( T &res, I &integrand );
	
protected:
	
	int end, index;
	const CIntegrationPoints &gps;

        void SetBegin()                      { index = -1;   end = gps.N( );   }
	int  GetIndex()          const       { return   index;                  }
	operator int()           const       { return   index;                  }
	bool operator++()                    { return ++index < end;            }
        double  GetWeight()       const      { return  gps.GetWeight( index ); }
        const mvector& GetCoords() const     { return  gps.GetKsi(    index ); }

};

template< class T, class I >
CIntegrator< T, I >::~CIntegrator()
{  }

template< class T, class I >
bool CIntegrator< T, I >::Integrate( T &res, I &integrand )
{
	SetBegin( );
	while( this->operator ++() )
		res += GetWeight() * integrand( GetCoords( ) );
	return true;
}

#endif // !defined(AFX_INTEGRATOR_H__9E4938E1_CF4E_4A71_A9B8_DF6B7D58E9BE__INCLUDED_)
