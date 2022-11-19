/* 
 * File:   CFEFunctorVectorTRUSS3DResidualVector.cpp
 * Author: piotrek
 * 
 * Created on September 8, 2010, 5:54 PM
 */

#include "FEFunctorVectorTruss3DResidual.h"

CFEFunctorVectorTruss3DResidual::CFEFunctorVectorTruss3DResidual( CFEFunctorVector *q ):CFEFunctorVector("CFEFunctorVectorTRUSS3DResidualVector"),qe(q)
{
       AddNode(qe);
	   al = qe->GetAllocator();
}

CFEFunctorVectorTruss3DResidual::CFEFunctorVectorTruss3DResidual(const CFEFunctorVectorTruss3DResidual& vf):CFEFunctorVector( vf )
{
       CLONE_MEMBER( vf, qe );
}

CFEFunctorVectorTruss3DResidual::~CFEFunctorVectorTruss3DResidual() {
}

void CFEFunctorVectorTruss3DResidual::Compute(CFiniteElementInstance *fe1 )
{
    const mvector &q = (*qe);

    if (fe1->GetNodes().size()!=2) throw CFEException( "FEFunctorMatrixTRUSS3DResidualVector::Compute() - TRUSS element should have two nodes" );

    unsigned i;
    mvector X;
    fe1->GetNodes()[1]->GetX(X);
    double E, A,  EA, du=q[3]-q[0], dv=q[4]-q[1], dw=q[5]-q[2], N, l = fe1->GetNodes()[0]->Distance( X );
    A = fe1->GetMaterial()->GetValue("A");
    E = fe1->GetMaterial()->GetValue("E");


    EA = E * A;

    fv.SetDim(6);

   N = E * A * ( du * du + 2.0 * du * l + dv * dv + dw * dw ) / 2.0 / l / l;
   double Fe[6] = { N * ( -1.0 - du / l )  , -N * dv / l , -N * dw / l , N * ( 1.0 + du / l ), N * dv / l , N * dw / l  };

   for (i=0; i<6; i++)
       fv[i] = Fe[i];

}

/*

Fe.Dim( 6 );
   double du = q(3) - q(0), dv = q(4) - q(1), dw = q(5) - q(2), N, E, A;

   mat->GetConstantValue("E",E);
   mat->GetConstantValue("A",A);

   N = E * A * ( du * du + 2.0 * du * l + dv * dv + dw * dw ) / 2.0 / l / l;
   Fe.in( N * ( -1.0 - du / l )  , -N * dv / l , -N * dw / l , N * ( 1.0 + du / l ), N * dv / l , N * dw / l );

*/
