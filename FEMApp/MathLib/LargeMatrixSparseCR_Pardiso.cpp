
#ifdef USE_PARDISO

#include "LargeMatrixSparseCR_Pardiso.h"
#include <iostream>

CLargeMatrixSparseCR_Pardiso::CLargeMatrixSparseCR_Pardiso( unsigned nth, PAR_MATRIX_TYPES mtp ):CLargeMatrixSparseCR(true), mtype( mtp ),nrhs(1)
{
    unsigned i;
    for (i = 0; i < 64; i++)
        {
          iparm[i] = 0;
        }
      iparm[0] = 1;			/* No solver default */
      iparm[1] = 2;			/* Fill-in reordering from METIS */
      iparm[2] = nth;         /* Numbers of processors, value of OMP_NUM_THREADS */
      iparm[3] = 0;			/* No iterative-direct algorithm */
      iparm[4] = 0;			/* No user fill-in reducing permutation */
      iparm[5] = 0;			/* Write solution into x */
      iparm[6] = 0;			/* Not in use */
      iparm[7] = 2;			/* Max numbers of iterative refinement steps */
      iparm[8] = 0;			/* Not in use */
      iparm[9] = 13;		/* Perturb the pivot elements with 1E-13 */
      iparm[10] = 1;		/* Use nonsymmetric permutation and scaling MPS */
      iparm[11] = 0;		/* Conjugate transposed/transpose solve */
      iparm[12] = 1;		/* Maximum weighted matching algorithm is switched-on (default for non-symmetric) */
      iparm[13] = 0;		/* Output: Number of perturbed pivots */
      iparm[14] = 0;		/* Not in use */
      iparm[15] = 0;		/* Not in use */
      iparm[16] = 0;		/* Not in use */
      iparm[17] = 0;		/* Output: Number of nonzeros in the factor LU */
      iparm[18] = 0;		/* Output: Mflops for LU factorization */
      iparm[19] = 0;		/* Output: Numbers of CG Iterations */
      iparm[59] = 0;		/* Input:  In/Out of core 0/2  */
      maxfct = 1;			/* Maximum number of numerical factorizations. */
      mnum = 1;			/* Which factorization to use. */
      msglvl = 0;			/* Print statistical information in file */
      error = 0;			/* Initialize error flag */

    /* -------------------------------------------------------------------- */
    /* .. Initialize the internal solver memory pointer. This is only */
    /* necessary for the FIRST call of the PARDISO solver. */
    /* -------------------------------------------------------------------- */
      for (i = 0; i < 64; i++)
        {
          pt[i] = 0;
        }

}


CLargeMatrixSparseCR_Pardiso::CLargeMatrixSparseCR_Pardiso( const CLargeMatrixSparseCR_Pardiso &K  ):CLargeMatrixSparseCR(true),mtype( K.mtype ),nrhs(1)
{
    unsigned i;
    for (i = 0; i < 64; i++)
        {
          iparm[i] = 0;
        }
      iparm[0] = 1;			/* No solver default */
      iparm[1] = 2;			/* Fill-in reordering from METIS */
      iparm[2] = K.iparm[2];         /* Numbers of processors, value of OMP_NUM_THREADS */
      iparm[3] = 0;			/* No iterative-direct algorithm */
      iparm[4] = 0;			/* No user fill-in reducing permutation */
      iparm[5] = 0;			/* Write solution into x */
      iparm[6] = 0;			/* Not in use */
      iparm[7] = 2;			/* Max numbers of iterative refinement steps */
      iparm[8] = 0;			/* Not in use */
      iparm[9] = 13;		/* Perturb the pivot elements with 1E-13 */
      iparm[10] = 1;		/* Use nonsymmetric permutation and scaling MPS */
      iparm[11] = 0;		/* Conjugate transposed/transpose solve */
      iparm[12] = 1;		/* Maximum weighted matching algorithm is switched-on (default for non-symmetric) */
      iparm[13] = 0;		/* Output: Number of perturbed pivots */
      iparm[14] = 0;		/* Not in use */
      iparm[15] = 0;		/* Not in use */
      iparm[16] = 0;		/* Not in use */
      iparm[17] = 0;		/* Output: Number of nonzeros in the factor LU */
      iparm[18] = 0;		/* Output: Mflops for LU factorization */
      iparm[19] = 0;		/* Output: Numbers of CG Iterations */
      iparm[59] = 0;		/* Input:  In/Out of core 0/2  */
      maxfct = 1;			/* Maximum number of numerical factorizations. */
      mnum = 1;			/* Which factorization to use. */
      msglvl = 0;			/* Print statistical information in file */
      error = 0;			/* Initialize error flag */

    /* -------------------------------------------------------------------- */
    /* .. Initialize the internal solver memory pointer. This is only */
    /* necessary for the FIRST call of the PARDISO solver. */
    /* -------------------------------------------------------------------- */
      for (i = 0; i < 64; i++)
        {
          pt[i] = 0;
        }
}

CLargeMatrixSparseCR_Pardiso::~CLargeMatrixSparseCR_Pardiso()
{

}





bool CLargeMatrixSparseCR_Pardiso::Decompose(CProgress *pr)
{
    /* -------------------------------------------------------------------- */
    /* .. Reordering and Symbolic Factorization. This step also allocates */
    /* all memory that is necessary for the factorization. */
    /* -------------------------------------------------------------------- */

    double ddum;			/* Double dummy */
    MKL_INT idum;			/* Integer dummy. */
    MKL_INT n = (int)dim;

    CreateMatrixCRData2( false );
      phase = 11;
      cout << "PARDISO reordering... ";
      PARDISO (pt, &maxfct, &mnum, &mtype, &phase, &n, data, rdata, cdata, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
      if (error != 0)
        {
          cout << "PARDISO ERROR: during symbolic factorization: ";
          switch(error)
          {
            case -1     : cout << "input inconsistent"<<endl; break;
            case -2 	: cout << "not enough memory"<<endl; break;
            case -3 	: cout << "reordering problem"<<endl; break;
            case -4 	: cout << "zero pivot, numerical factorization or iterative refinement problem"<<endl; break;
            case -5 	: cout << "unclassified (internal) error"<<endl; break;
            case -6 	: cout << "pre-ordering failed (matrix types 11, 13 only)"<<endl; break;
            case -7 	: cout << "diagonal matrix is singular"<<endl; break;
            case -8 	: cout << "32-bit integer overflow problem"<<endl; break;
            case -9 	: cout << "not enough memory for OOC"<<endl; break;
            case -10 	: cout << "problems with opening OOC temporary files"<<endl; break;
            case -11 	: cout << "read/write problems with the OOC data file"<<endl; break;
            default     : cout << "unknown error"<<endl;  break;
          }
          return false;
        }
      cout << "Reordering completed ... " << endl;
      cout << "Number of nonzeros in factors = " << iparm[17] << endl;
      cout << "Number of factorization MFLOPS = " <<  iparm[18] << endl;
    /* -------------------------------------------------------------------- */
    /* .. Numerical factorization. */
    /* -------------------------------------------------------------------- */
      cout << " numerical factorization... ";
      phase = 22;
      PARDISO (pt, &maxfct, &mnum, &mtype, &phase, &n, data, rdata, cdata, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
      if (error != 0)
        {
          cout << "PARDISO ERROR: during numerical factorization: ";
          switch(error)
          {
            case -1     : cout << "input inconsistent"<<endl; break;
            case -2 	: cout << "not enough memory"<<endl; break;
            case -3 	: cout << "reordering problem"<<endl; break;
            case -4 	: cout << "zero pivot, numerical factorization or iterative refinement problem"<<endl; break;
            case -5 	: cout << "unclassified (internal) error"<<endl; break;
            case -6 	: cout << "pre-ordering failed (matrix types 11, 13 only)"<<endl; break;
            case -7 	: cout << "diagonal matrix is singular"<<endl; break;
            case -8 	: cout << "32-bit integer overflow problem"<<endl; break;
            case -9 	: cout << "not enough memory for OOC"<<endl; break;
            case -10 	: cout << "problems with opening OOC temporary files"<<endl; break;
            case -11 	: cout << "read/write problems with the OOC data file"<<endl; break;
            default     : cout << "unknown error"<<endl;  break;
          }
          return false;
        }
      return true;
}


bool CLargeMatrixSparseCR_Pardiso::Solve( const mvector &b, mvector &x )
{
    phase = 33;
    unsigned i;


    double ddum;			/* Double dummy */
    MKL_INT idum;			/* Integer dummy. */
    cout << " solving... ";
    int n=dim;
    //  Loop over 3 solving steps: Ax=b, AHx=b and ATx=b
      for (i = 0; i < 3; i++)
        {
          iparm[11] = i;		/* Conjugate transposed/transpose solve */
          double *pb = const_cast<double *> ( b.GetDataPointer() ), *px = const_cast<double *>( x.GetDataPointer() );
          pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n, data, rdata, cdata, &idum, &nrhs, iparm, &msglvl, pb, px, &error);
          if (error != 0)
          {
                cout << "nPARDISO ERROR during solution: ";
                switch(error)
                {
                  case -1     : cout << "input inconsistent"<<endl; break;
                  case -2 	: cout << "not enough memory"<<endl; break;
                  case -3 	: cout << "reordering problem"<<endl; break;
                  case -4 	: cout << "zero pivot, numerical factorization or iterative refinement problem"<<endl; break;
                  case -5 	: cout << "unclassified (internal) error"<<endl; break;
                  case -6 	: cout << "pre-ordering failed (matrix types 11, 13 only)"<<endl; break;
                  case -7 	: cout << "diagonal matrix is singular"<<endl; break;
                  case -8 	: cout << "32-bit integer overflow problem"<<endl; break;
                  case -9 	: cout << "not enough memory for OOC"<<endl; break;
                  case -10 	: cout << "problems with opening OOC temporary files"<<endl; break;
                  case -11 	: cout << "read/write problems with the OOC data file"<<endl; break;
                  default     : cout << "unknown error"<<endl;  break;
                }
                return false;
          }

          char *uplo;
          if (i == 0)
              uplo = "non-transposed";
                else if (i == 1)
              uplo = "conjugate transposed";
                else
              uplo = "transposed";

          mvector bs( dim );
          double *pbs = const_cast<double *>( bs.GetDataPointer() );
/*

    // Compute residual
          mkl_dcsrgemv(uplo, &n, data, rdata, cdata, px, pbs);
    //        MKL_DCSRGEMV(uplo, &n, a, ia, ja, x, bs);
          double res = 0.0;
          double res0 = 0.0;
          int j;
          for (j = 1; j <= n; j++)
        {
          res += (bs[j - 1] - b[j - 1]) * (bs[j - 1] - b[j - 1]);
          res0 += b[j - 1] * b[j - 1];
        }
          res = sqrt (res) / sqrt (res0);
//          printf ("\nRelative residual = %e", res);
    // Check residual
          if (res > 1e-10)
            {
                printf ("Error: residual is too high!\n");
                exit (10 + i);
            } */
        }
      double *pb = const_cast<double *> ( b.GetDataPointer() ), *px = const_cast<double *>( x.GetDataPointer() );
      phase = -1;
      pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n, data, rdata, cdata, &idum, &nrhs, iparm, &msglvl, pb, px, &error);
      cout << " OK" << endl;
}

#endif
