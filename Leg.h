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

		/*
		 * \brief CTor
		 * \author Josef Sommerauer
		 * \param Femur Servo of Femur
		 * \param Tibia Servo of Tibia
		 * \param Tarsus Servo of Tarsus
		 */
        Leg(int Nr, T3DPosition Ori, 
            Servo const& Femur, 
            Servo const& Tibia, 
            Servo const& Tarsus);

        /*
         * \brief DTor
         * \author Josef Sommerauer
         */
        ~Leg();

        // setParams , getParams ...

        /*
         * \brief 	sets the angles of servos.
         * \author	Josef Sommerauer
		 * \param 	Angles a set of 3 angles. one angle for each servo.
		 * \return 	false if angle is out of range or in case of an driver error.
		 */
        bool    SetAngle(TAngles const &Angles);

        /*
         * \brief Get Angle of Each Servo
         * \author Josef Sommerauer
         * \return angle of each servo.
		*/
        TAngles GetAngle() const;

        /*
         * \brief	returns default position (normal stand) of leg.
         * \author	Josef Sommerauer
         * \return 	default 3D Positions
         */
        T3DPosition GetOriginPos() const;

		/*
		 * \brief	turns of pwm generation for each servo of leg.
		 * \author	Josef Sommerauer
		 * \return 	in case of an driver error.
		*/
        bool Off();

        /*
         * \brief 	pull in leg. parking position.
         * \author	Josef Sommerauer
         */
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
