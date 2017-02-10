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
    /**
     * \brief	Default CTor
     * \author	Josef Sommerauer
     * \param servo_dev Path to Servo-Interface
     */
    LegCtrl(std::string const & servo_dev);

    /**
     * \brief	Default DTor
     * \author	Josef Sommerauer
     */
    ~LegCtrl();

    /** \brief	Sets a leg to the specified position
     * \author	Josef Sommerauer
     * \param 	ID 	Number of leg
     * \param 	Pos Koordinates of the leg
     * \return 	True if function succeedes
     */
    bool SetXYZ(int ID, T3DPosition const &Pos);

    /**
     * \brief	Sets the angles of a leg
     * \author	Josef Sommerauer
     * \param	ID	Number of leg
     * \param	Ang	Angle set of a leg
     * \return	True if function succeeded
     */
    bool SetAngle(int ID, TAngles const &Ang);

    //void TestSetXYZ();

    /**
     * \brief	Returns the coordinates of a leg
     * \author	Josef Sommerauer
     * \param	ID	Number of leg
     * \return	Current coordinates of a leg
     */
    T3DPosition GetXYZ(int ID) const;

    /**
     * \brief	Returns the angles of a leg
     * \author	Josef Sommerauer
     * \param	ID Number of leg
     * \return	angles of a leg
     */
    TAngles GetAngles(int ID) const;

    /**
     * \brief	Sets all legs to a default position
     * \author	Josef Sommerauer
     */
    void ReadyPosition();

    /**
     * \brief	Checks if Leg Ctrl is ready
     * \author	Josef Sommerauer
     * \return	True if legs are ready
     */
    bool IsReady() const;

    /**
     * \brief	Disables all legs
     * \author	Josef Sommerauer
     */
    void AllOff();

    /**
     * \brief	Sets all legs to sleep mode
     * \author	Josef Sommerauer
     */
    void AllSleep();

    /*
     * \brief	Calculate Leg-Positions to do Movement
     * \author	Josef Sommerauer
     * \param	h	height of Hexapod
     * \param	r	Step-width of Hexapod
     * \param	phi	Direction to Move
     */
    std::vector<std::vector<T3DPosition> > CalcMotionPos(
         int h, int r, double const &phi);

    /*
     * \brief	Calculate Leg-Positions to do Rotation
     * \author	Josef Sommerauer
     * \param	h		Height of Hexapod
     * \param	alpha	Direction to Rotate
     */
    std::vector<std::vector<T3DPosition> > CalcRotatePos(
      int h, double const &alpha);

private:
    /// Positions of each Leg
    T3DPosition m3DPos[NUM_OF_LEGS];

    /// Default Leg Positions
    DefaultLegPos mDefLegPos;

    /// Geometric Data from Servos
    TServoValues mServoValues;

    /// Legs
    Leg *mLegs[NUM_OF_LEGS];

    /// Filestream to Get Data
    std::fstream mFs;

    /// Parser to get Geometric Settings
    Parser *mPars;

    /// Inverse Kinematic
    InverseKinematic mInvKin;

    /// Hexapod is in Readyposition
    bool mReady;
};


#endif // LEGCTRL_H
