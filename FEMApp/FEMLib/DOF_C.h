#ifndef DOF_C_H
#define DOF_C_H

#include "DOF.h"
#include "DOF_types.h"

class CDOF_C : public CDOF
{
    public:
        //!  A CDOF_C class
        /*!
            destructor.
        */
        virtual ~CDOF_C(void);

        //! CDOF_C clone member
        /*!
          \return pointer to copy ow the object
        */
        virtual CDOF_C* Clone() const { return new CDOF_C(*this); }

        //! CDOF_C symbol for solution value.
        /*!
          \return returns string contains the DOF solution symbol ( here "t" - nodat temperature ).

        */
        virtual const char* DofName()  const { return "c"; }

        //! CDOF_C parameter.
        /*!
          \return returns string contains the DOF symbol of nodal parameters(  here "D" - diffusion flux ).

        */
        virtual const char* LoadName() const { return "C"; }

    protected:

        //!  A CDOF_C class
        /*!
            Constructor.
        */
        CDOF_C();

        friend class CDOFs;
};
#endif // DOF_C_H
