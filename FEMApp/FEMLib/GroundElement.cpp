#include "GroundElement.h"
#include "FEFunctorMatrix.h"

void CGroundElement::SetThickness( double t )
{
	tg = t;
}

double  CGroundElement::Calc_Rg( CFEFunctorScalar *cmp, double &C ) const
{
    C = CalcCompliance( cmp );
    double rg =  tg * C;
	return rg;
}

/*
double  CGroundElement::Calc_Vr() const
{
	double V=0.0;

	for(unsigned k=0; k<felems.size(); k++) 
	{
        V += felems[ k ]->GetVolume( );
	}
	return V / tg;
}*/


double CGroundElement::CalcCompliance( CFEFunctorScalar *cmp ) const
{ 
		double C=0.0; 
		for(unsigned k=0; k<felems.size(); k++) 
        {
            cmp->InvalidateStructure();
            (*cmp)( felems[ k ] );
            C += (*cmp);
        }
		
        return tg * C;
}


void CGroundElement::AggregateStiffnessMatrix( const matrix &ke, CFEFunctorMatrix *mfk, CLargeMatrix &K )
{
    unsigned k,l;
    valarray<unsigned> alr, alc;

    for (k=0; k<felems.size(); k++)
    {
        mfk->InvalidateStructure();
        mfk->ManageDOFs( felems[ k ] );
        mfk->GetAllocationVectors( alr, alc );
        K.AllocateMatrix(alr,alc, tg * ke );
    }
}
