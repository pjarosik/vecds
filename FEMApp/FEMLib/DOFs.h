
#pragma once

#include "DOF_types.h"

#include "DOF_Ax.h"
#include "DOF_Ay.h"
#include "DOF_Bx.h"
#include "DOF_By.h"
#include "DOF_Fix.h"
#include "DOF_Fiy.h"
#include "DOF_Fiz.h"
#include "DOF_Ux.h"
#include "DOF_Uy.h"
#include "DOF_Uz.h"
#include "DOF_T.h"
#include "DOF_D.h"
#include "DOF_C.h"

class CDOFs
{

public:

	static CDOF_Ax Ax;
	static CDOF_Ay Ay;
	static CDOF_Ux Ux;
	static CDOF_Uy Uy;
	static CDOF_Uz Uz;
	static CDOF_Fix Fix;
	static CDOF_Fiy Fiy;
	static CDOF_Fiz Fiz;
	static CDOF_T T;
    static CDOF_D D;
    static CDOF_C C;

	static CDOF_Bx Bx[12];
	static CDOF_By By[12];


};
