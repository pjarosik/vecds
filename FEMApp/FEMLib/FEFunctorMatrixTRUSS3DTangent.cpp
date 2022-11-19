/* 
 * File:   CFEFunctorMatrixTRUSS3DTangentMatrix.cpp
 * Author: piotrek
 * 
 * Created on September 8, 2010, 5:17 PM
 */

#include "FEFunctorMatrixTruss3DTangent.h"

CFEFunctorMatrixTruss3DTangent::CFEFunctorMatrixTruss3DTangent(CFEFunctorVector *q):CFEFunctorMatrix("CFEFunctorMatrixTRUSS3DTangentMatrix"),qe(q)
{
   AddNode(qe);
   alr = alc = qe->GetAllocator();
}

CFEFunctorMatrixTruss3DTangent::CFEFunctorMatrixTruss3DTangent(const CFEFunctorMatrixTruss3DTangent& mf) :CFEFunctorMatrix(mf)
{
    CLONE_MEMBER( mf, qe )
}

CFEFunctorMatrixTruss3DTangent::~CFEFunctorMatrixTruss3DTangent()
{

}

void CFEFunctorMatrixTruss3DTangent::Compute( CFiniteElementInstance *fe1)
{
    const mvector &q = (*qe);

    if ( fe1->GetNodes().size() != 2 ) throw CFEException( "FEFunctorMatrixTRUSS3DResidualVector::Compute() - TRUSS element should have two nodes" );

    unsigned i,j;
    mvector X;
    fe1->GetNodes()[1]->GetX(X);
    double E, A,  EA, du=q[3]-q[0], dv=q[4]-q[1], dw=q[5]-q[2], N, l = fe1->GetNodes()[0]->Distance( X );
    
	A = fe1->GetMaterial()->GetValue("A");
    E = fe1->GetMaterial()->GetValue("E");

    fv.SetDim(6,6);

    EA = E * A;
    matrix Ku1(6,6), Ku2(6,6), Ks(6,6), K(6,6);

    N  = EA * ( du*du + 2.0*du*l + dv*dv + dw*dw ) / 2.0 / l / l ;

	double cK[6][6] =  { { EA/l    , 0.0  , 0.0  , -EA/l   , 0.0 , 0.0  },
                          {0.0    , 0.0  , 0.0  , 0.0    , 0.0, 0.0  },
                          {0.0    , 0.0  , 0.0  , 0.0    , 0.0 , 0.0 },
                          {-EA/l   , 0.0  , 0.0  ,  EA/l   , 0.0 , 0.0  },
                          {0.0    , 0.0 , 0.0  , 0.0    , 0.0 , 0.0  },
                          {0.0    , 0.0  , 0.0 , 0.0    , 0.0 , 0.0  } };

    double cKs[6][6] =  { { N/l    , 0.0  , 0.0  , -N/l   , 0.0 , 0.0  },
                          {0.0    , N/l  , 0.0  , 0.0    , -N/l, 0.0  },
                          {0.0    , 0.0  , N/l  , 0.0    , 0.0 , -N/l },
                          {-N/l   , 0.0  , 0.0  ,  N/l   , 0.0 , 0.0  },
                          {0.0    , -N/l , 0.0  , 0.0    , N/l , 0.0  },
                          {0.0    , 0.0  , -N/l , 0.0    , 0.0 , N/l  } };
	
	double cKu1[6][6] = {   { 2.0*du , dv   , dw   , -2.0*du,-dv  ,-dw   },
                            {dv     , 0.0  , 0.0  , -dv    , 0.0 , 0.0  },
                            {dw     , 0.0  , 0.0  , -dw    , 0.0 , 0.0  },
                            {-2.0*du, -dv  , -dw  , 2.0*du , dv  , dw   },
                            {-dv    , 0.0  , 0.0  , dv     , 0.0 , 0.0  },
                            {-dw    , 0.0  , 0.0  , dw     , 0.0 , 0.0  } };


	double cKu2[6][6] = {  { du*du ,  du*dv ,  du*dw ,-du*du ,-du*dv , -du*dw },
        { dv*du ,  dv*dv ,  dv*dw ,-dv*du ,-dv*dv , -dv*dw },
        { dw*du ,  dw*dv ,  dw*dw ,-dw*du ,-dw*dv , -dw*dw },
        {-du*du , -du*dv , -du*dw , du*du , du*dv ,  du*dw },
        {-dv*du , -dv*dv , -dv*dw , dv*du , dv*dv ,  dv*dw },
        {-dw*du , -dw*dv , -dw*dw , dw*du , dw*dv ,  dw*dw } };

        for (i=0; i<6; i++)
            for (j=0; j<6; j++)  
				fv(i,j) =   cK[i][j] + cKs[i][j] + EA / l / l * cKu1[i][j]  +  EA / l / l / l * cKu2[i][j] ;


}
