#ifndef DOF_D_H
#define DOF_D_H
#include "DOF.h"
#include "DOF_types.h"

class CDOF_D : public CDOF
{
    public:
        //!  A CDOF class
        /*!
            destructor.
        */
        virtual ~CDOF_D(void);

        //! DOF clone member
        /*!
          \return pointer to copy ow the object
        */
        virtual CDOF_D* Clone() const { return new CDOF_D(*this); }

        //! DOF symbol for solution value.
        /*!
          \return returns string contains the DOF solution symbol ( here "t" - nodat temperature ).

        */
        virtual const char* DofName()  const { return "d"; }

        //! DOF parameter.
        /*!
          \return returns string contains the DOF symbol of nodal parameters(  here "D" - diffusion flux ).

        */
        virtual const char* LoadName() const { return "D"; }

    protected:

        //!  A CDOF class
        /*!
            Constructor.
        */
        CDOF_D();

        friend class CDOFs;
};


#endif // DOF_D_H
