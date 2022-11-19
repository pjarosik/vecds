#ifndef LARGEMATRIXSPARSESTREAM_H
#define LARGEMATRIXSPARSESTREAM_H

#include "LargeMatrix.h"
#include <list>

class CLargeMatrixSparseStream : public CLargeMatrix
{
        public:
            struct CMatrixCell {
                unsigned row, col;
                double value;
            };

            class CSlice
            {
            public:
                const CLargeMatrixSparseStream *LM;
                CMatrixCell *data;
                const double *pin;
                double *pout;
                unsigned nthreads, ten;
                CSlice( const CLargeMatrixSparseStream *lm, CMatrixCell *mc, const double *i, double *o, unsigned n, unsigned t ):LM(lm),
                    data(mc), pin(i), pout( o ), nthreads( n ), ten( t ) { }
            };
            CLargeMatrixSparseStream( bool ss );
            CLargeMatrixSparseStream(const CLargeMatrixSparseStream& orig);
            virtual ~CLargeMatrixSparseStream();

            //!  Copy matrix
            /*!
                \param K CLargeMatrix
                Creates a copy..
            */
            virtual void Copy( CLargeMatrix &K );

            //!  Reseting matrix
            /*!
                Deallocation memory.
            */
            virtual void Reset();

            //!  Assigns 0 to all members
            /*!
                Assigns 0 to all members.
            */
            virtual void SetZero();

            //!  Matrix accumulation
            /*!
                Adds other Largem matrix multiplied by a factor.
            */
                virtual void Add( double w, CLargeMatrix *M ) { }
            //!  Initialises the matrix with given dimension
            /*!
                \param ndof dimension of the CLargeMatrix
                Initialises dimension of the matrix.
            */

                //!  LargeMatrix assignment
            /*!
                Assigns LargeMatrix
            */
                virtual void Set( CLargeMatrix &M ) { }
                //!  Initialises the matrix with given dimension
            /*!
                \param ndof dimension of the CLargeMatrix
                Initialises dimension of the matrix.
            */
                virtual bool Create( unsigned ndof );

                //!  Matrix DOF erasing
            /*!
                Erases a DOF by nulyfying row and columns.
            */
                 virtual void EraseDof( unsigned dof );

            //!  Allocation of a matrix
            /*!
                \param al - allocation vector
                \param m - small (element) matrix
                Allocates the matrix based on given allocation vector.
            */
            virtual void AllocateMatrix( valarray<unsigned> &alr, valarray<unsigned> &alc, const matrix &m );

            //!  Allocation of a matrix
            /*!
                Finds the greatest diagonal member an multiply it by 10e5;
            */
            virtual void FindFactorC( );

            //!  Creation a matrix
            /*!
                Allocates memory for the matrix.
            */
            virtual bool CreateMatrix( );

            //!  Copy structure
            /*!
                \param K CLargeMatrix
                Copies structure of other CLargeMatrix.
            */
            virtual bool CreateStructure( CLargeMatrix *K );


             //!  Updates profile.
            /*!
                 Updates profile of a matrix using element allocation vector.
                  \param alr - row numbers allocation vector
                  \param alc - column numbers allocation vector
            */
             virtual void UpdateProfile( const valarray<unsigned> &alr, const valarray<unsigned> &alc );
            //!  Updates profile.
           /*!
                Updates profile of a given matrix member.
                  \param row - row number
                  \param col - column number
           */
            void UpdateProfile( unsigned row, unsigned col );

             //!  Matrix decomposition
            /*!
                Decomposes a matrix.
            */
             virtual bool Decompose(CProgress *pr = 0) { return true; }

             virtual void Sum( double, CLargeMatrix* );

            virtual void MultiplyColumn( double* W, unsigned ndof, double wr );
            virtual void MultiplyVector( const mvector &in, mvector &out ) const;
            virtual void MultiplyVectorSlice( CSlice& ) const;
            virtual void print();
            virtual void Read( const char *nm );
            virtual void Save( const char *nm );

        protected:

            bool symmetric_storage;
            CMatrixCell  *data;
            vector< list< unsigned > > lines;
            vector< unsigned > blines;
            unsigned *blines_raw, *elines_raw, *cols_raw;

            double *data_raw;

            CMatrixCell* GetData() { return data; }
            const vector< list< unsigned > >& GetLines() const { return lines; }
            const vector< unsigned >& GetBeginLines() const { return blines; }


};


#endif // LARGEMATRIXSPARSESTREAM_H
