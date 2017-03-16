///////////////////////////////////////////////////////////////////////////
// Workfile    : Leg.h
// Author      : Patrick Felixberger, Josef Sommerauer
// Date        :
// Description : Leg
// Remarks     : -
// Revision    : 1
///////////////////////////////////////////////////////////////////////////
#ifndef LEG_H
#define LEG_H

#include "Object.h"
#include "Servo.h"
#include "Definitions.h"

/**
 * \brief Controls the Servos of a Leg
 * \author JosefSommerauer, Patrick Felixberger
 * \date 2013/12/04
 */
class Leg : public Object {
    public:
        Leg(int Nr, T3DPosition Ori, 
            Servo const& Femur, 
            Servo const& Tibia, 
            Servo const& Tarsus);

        ~Leg();

        bool SetAngle(TAngles const &Angles);

        TAngles GetAngle() const;

        T3DPosition GetOriginPos() const;

        void Calibrate(int servo);

        void Print(std::ostream &o);

        bool Off();

        void Sleep();

    private:
        
        /// leg number
        int         mNr;
        /// position on hexapod
        T3DPosition mOriginPos;

        /// Femur-Servo
        Servo mFemur;
        /// Tibia-Servo
        Servo mTibia;
        /// Tarsus-Servo
        Servo mTarsus;        
};


#endif // LEG_H
