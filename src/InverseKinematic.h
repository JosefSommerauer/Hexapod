///////////////////////////////////////////////////////////////////////////
// Workfile    : InverseKinematic.h
// Author      : Patrick Felixberger, Josef Sommerauer
// Date        :
// Description : Calculates leg positions
// Remarks     : -
// Revision    : 0
///////////////////////////////////////////////////////////////////////////
#ifndef INVERSEKINEMATIC_H
#define INVERSEKINEMATIC_H

#include "Definitions.h"
#include "Parser.h"
#include "Object.h"

std::string const hexa_size = "Hexapod.txt";    /**< File with hexapod dimensions */


/** \brief
 * 		class InverseKinematic.
 * 		Calculates the angles of the current position.
 *
 * 	\author Patrick Felixberger, Josef Sommerauer
 * 	\date 	2013/12/04
 */
class InverseKinematic : public Object {
public:
    /** \brief	Default CTor
     * \author	Patrick Felixberger
     */
    InverseKinematic();

    /** \brief Default DTor
     * \author Patrick Felixberger
     */
    ~InverseKinematic();

    /** \brief	Calculates the angles of a leg
     * \author	Patrick Felixberger
     * \param	ID 	Number of leg
     * \param	Pos Position of leg
     * \return	Angles of leg
     */
    TAngles CalculateAngles(int ID, T3DPosition const &Pos) const;

private:
    /*
     * \brief Get Angle of Femur
     * ToDo: Explain Paramter
     */
    double AngleFemur(  double const &x, double const &ny) const;
    double AngleTarsus( double const &a, double const &b,
                        double const &c) const;
    double AngleTibia(  double const &a, double const &b,
                        double const &c, double const &z,
                        double const &dist) const;

    HexapodSize mSizeData;

};


#endif // INVERSEKINEMATIC_H
