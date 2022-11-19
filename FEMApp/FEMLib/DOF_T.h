#pragma once
#include "DOF.h"


class CDOF_T :
        public CDOF
    {
    public:


        //!  A CDOF class
        /*!
            destructor.
        */
        virtual ~CDOF_T(void);

        //! DOF clone member
        /*!
          \return pointer to copy ow the object
        */
        virtual CDOF_T* Clone() const { return new CDOF_T(*this); }

        //! DOF symbol for solution value.
        /*!
          \return returns string contains the DOF solution symbol ( here "t" - nodat temperature ).

        */
        virtual const char* DofName()  const { return "t"; }

        //! DOF parameter.
        /*!
          \return returns string contains the DOF symbol of nodal parameters(  here "F" - heat flux ).

        */
        virtual const char* LoadName() const { return "F"; }

    protected:

        //!  A CDOF class
        /*!
            Constructor.
        */
        CDOF_T();

        friend class CDOFs;
    };
