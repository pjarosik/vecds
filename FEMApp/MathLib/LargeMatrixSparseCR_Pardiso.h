#ifndef LARGEMATRIXSPARSECR_PARDISO_H
#define LARGEMATRIXSPARSECR_PARDISO_H

#include <mkl_pardiso.h>
#include <mkl_types.h>
#include <mkl_spblas.h>
#include "LargeMatrixSparseCR.h"

enum PAR_MATRIX_TYPES
{
    PAR_REAL_SYM_PD = 2,
    PAR_REAL_SYM_ID = -2,
    PAR_REAL_UNSYM = 11
};

class CLargeMatrixSparseCR_Pardiso : public CLargeMatrixSparseCR
{
public:
    CLargeMatrixSparseCR_Pardiso( unsigned nth, PAR_MATRIX_TYPES tp = PAR_REAL_SYM_PD );
    CLargeMatrixSparseCR_Pardiso(const CLargeMatrixSparseCR_Pardiso& orig);
    virtual ~CLargeMatrixSparseCR_Pardiso();
    virtual CLargeMatrixSparseCR_Pardiso* Clone() const { return new CLargeMatrixSparseCR_Pardiso(*this); }

    virtual bool Solve( const mvector &b, mvector &x );
    virtual bool Decompose(CProgress *pr = 0);

protected:

    /* Matrix type */
    MKL_INT mtype;
    MKL_INT nrhs;		/* Number of right hand sides. */

    /* Internal solver memory pointer pt, */
    /* 32-bit: int pt[64]; 64-bit: long int pt[64] */
    /* or void *pt[64] should be OK on both architectures */
    void *pt[64];
    /* Pardiso control parameters. */
    MKL_INT iparm[64];
    MKL_INT maxfct, mnum, phase, error, msglvl;

};

#endif // LARGEMATRIXSPARSECR_PARDISO_H
