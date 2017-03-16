///////////////////////////////////////////////////////////////////////////
// Workfile    : LegCtrl.h
// Author      : Patrick Felixberger, Josef Sommerauer
// Date        :
// Description : Leg Control
// Remarks     : -
// Revision    : 2
///////////////////////////////////////////////////////////////////////////
#ifndef LEGCTRL_H
#define LEGCTRL_H

#include <fstream>

#include <boost/asio/serial_port.hpp> 
#include <boost/asio.hpp> 

#include "Object.h"
#include "Definitions.h"
#include "InverseKinematic.h"
#include "Leg.h"


//std::string const servo_fs = "/dev/servodrive0";

/** \brief
 * 		class LegCtrl.
 * 		Manages the legs of the hexapod.
 * 	\author	Josef Sommerauer
 * 	\date	2013/12/04
 */
class LegCtrl : public Object {
public:

    LegCtrl(std::string const & servo_dev);

    ~LegCtrl();

    bool SetXYZ(int ID, T3DPosition const &Pos);

    bool SetAngle(int ID, TAngles const &Ang);

    void TestSetXYZ();

    void Calibrate(int leg, int servo);

    T3DPosition GetXYZ(int ID) const;

    TAngles GetAngles(int ID) const;

    void ReadyPosition();

    bool IsReady() const;

	 void Print(int leg, std::ostream &o);

    void AllOff();

    void AllSleep();

    std::vector<std::vector<T3DPosition> > CalcMotionPos(
         int h, int r, double const &phi);

    std::vector<std::vector<T3DPosition> > CalcRotatePos(
      int h, double const &alpha);

private:
    /// Positions of each Leg
    T3DPosition m3DPos[NUM_OF_LEGS];
    TAngles mSleepAngles[NUM_OF_LEGS];

    /// Default Leg Positions
    DefaultLegPos mDefLegPos;

    /// Geometric Data from Servos
    TServoValues mServoValues;

    /// Legs
    Leg *mLegs[NUM_OF_LEGS];

    /// Filestream to Get Data
    //std::fstream mFs;
	 boost::asio::io_service io;
	 boost::asio::serial_port mFs;

    /// Parser to get Geometric Settings
    Parser *mPars;

    /// Inverse Kinematic
    InverseKinematic mInvKin;

    /// Hexapod is in Readyposition
    bool mReady;
};


#endif // LEGCTRL_H
