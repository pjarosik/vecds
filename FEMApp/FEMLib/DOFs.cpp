
#include "DOFs.h"


CDOF_Ax CDOFs::Ax;
CDOF_Ay CDOFs::Ay;
CDOF_Ux CDOFs::Ux;
CDOF_Uy CDOFs::Uy;
CDOF_Uz CDOFs::Uz;
CDOF_Fix CDOFs::Fix;
CDOF_Fiy CDOFs::Fiy;
CDOF_Fiz CDOFs::Fiz;
CDOF_T CDOFs::T;
CDOF_D CDOFs::D;
CDOF_C CDOFs::C;

CDOF_Bx CDOFs::Bx[12] = { 
							CDOF_Bx( DOF_BX + 2 ),  
							CDOF_Bx( DOF_BX + 4 ),
							CDOF_Bx( DOF_BX + 6 ),
							CDOF_Bx( DOF_BX + 8 ),
							CDOF_Bx( DOF_BX + 10 ),
							CDOF_Bx( DOF_BX + 12 ),
							CDOF_Bx( DOF_BX + 14 ),
							CDOF_Bx( DOF_BX + 16 ),
							CDOF_Bx( DOF_BX + 18 ),
							CDOF_Bx( DOF_BX + 20 ),
							CDOF_Bx( DOF_BX + 22 ),
							CDOF_Bx( DOF_BX + 24 ),
};

CDOF_By CDOFs::By[12] = { 
							CDOF_By( DOF_BY + 2 ),  
							CDOF_By( DOF_BY + 4 ),
							CDOF_By( DOF_BY + 6 ),
							CDOF_By( DOF_BY + 8 ),
							CDOF_By( DOF_BY + 10 ),
							CDOF_By( DOF_BY + 12 ),
							CDOF_By( DOF_BY + 14 ),
							CDOF_By( DOF_BY + 16 ),
							CDOF_By( DOF_BY + 18 ),
							CDOF_By( DOF_BY + 20 ),
							CDOF_By( DOF_BY + 22 ),
							CDOF_By( DOF_BY + 24 ),
};

