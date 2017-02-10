#include "Leg.h"
#include <unistd.h>

/*
 * \brief CTor
 * \author Josef Sommerauer
 * \param Femur Servo of Femur
 * \param Tibia Servo of Tibia
 * \param Tarsus Servo of Tarsus
 */
Leg::Leg(int Nr, T3DPosition Ori,
         Servo const& Femur, 
         Servo const& Tibia, 
         Servo const& Tarsus) :
        mNr(Nr), mOriginPos(Ori), mFemur(Femur), mTibia(Tibia), mTarsus(Tarsus) {}

/*
 * \brief DTor
 * \author Josef Sommerauer
 */
Leg::~Leg() {}

/*
 * \brief 	sets the angles of servos.
 * \author	Josef Sommerauer
 * \param 	Angles a set of 3 angles. one angle for each servo.
 * \return 	false if angle is out of range or in case of an driver error.
 */
bool Leg::SetAngle(TAngles const &Angles) {
   mFemur.SetAngle(Angles.AngleFemur);   usleep(20000);
   mTibia.SetAngle(Angles.AngleTibia);   usleep(20000);
   mTarsus.SetAngle(Angles.AngleTarsus); usleep(20000);

   return true;
}

/*
 * \brief Get Angle of Each Servo
 * \author Josef Sommerauer
 * \return angle of each servo.
*/
TAngles Leg::GetAngle() const{
   TAngles tmp;

   tmp.AngleFemur  = mFemur.GetAngle();
   tmp.AngleTibia  = mTibia.GetAngle();
   tmp.AngleTarsus = mTarsus.GetAngle();

   return tmp;
}

/*
 * \brief	returns default position (normal stand) of leg.
 * \author	Josef Sommerauer
 * \return 	default 3D Positions
 */
T3DPosition Leg::GetOriginPos() const{
   return mOriginPos;
}

/*
 * \brief	turns of pwm generation for each servo of leg.
 * \author	Josef Sommerauer
 * \return 	in case of an driver error.
*/
bool Leg::Off() {
    return mFemur.Off() &&
           mTibia.Off() &&
           mTarsus.Off();
}

/*
 * \brief 	pull in leg. parking position.
 * \author	Josef Sommerauer
 */
void Leg::Sleep() {
   mFemur.SetAngle(0);
   mTibia.SetAngle(mTibia.GetMaxAngle());
   mTarsus.SetAngle(mTarsus.GetMinAngle());
}
