#include <cmath>
#include <iostream>
#include <iomanip>

#include <iostream>

#include <cmath>

#include "Parser.h"
#include "InverseKinematic.h"

using namespace std;

/** \brief	Default CTor
 * \author	Patrick Felixberger
 */
InverseKinematic::InverseKinematic() {
   // Create Fileparser
   Parser parser(hexa_size);

   // Parse File to Get Hexapod Dimensions
   if(parser.ParseFile()) {
	   parser.GetData(mSizeData);
	   cout << "Parsed hexapod geometry successfully!" << endl;
   } else {
	   // Throw Exception on Error
	   throw "LegCtrl::LegCtrl(): can not open: Hexapod.txt";
   }
} // CTor

/** \brief Default DTor
 * \author Patrick Felixberger
 */
InverseKinematic::~InverseKinematic() {}

/** \brief	Calculates the angles of a leg
 * \author	Patrick Felixberger
 * \param	ID 	Number of leg
 * \param	Pos Position of leg
 * \return	Angles of leg
 */
TAngles InverseKinematic::CalculateAngles(int ID, T3DPosition const &Pos) const {
	// See sheet "InverseKinematik" in documentation!
	
	TAngles tmp;
    double nPosX = 0.0;
    double nPosY = 0.0;
    double dist = 0.0;
    double c = 0.0;

	// 1. Select leg (0 to 5)
	// 2. To calculate the angles, the mounting point of the leg will be set to 0/0/0
	// => Subtract <hexapod size> from <Pos>
    switch(ID) {
        case 0:
            nPosX = Pos.X + mSizeData.OM;
            nPosY = Pos.Y + mSizeData.OQ;
            break;

        case 1:
            nPosX = Pos.X + mSizeData.ON;
            nPosY = Pos.Y;
            break;

        case 2:
            nPosX = Pos.X + mSizeData.OM;
            nPosY = Pos.Y - mSizeData.OQ;
            break;

        case 3:
            nPosX = Pos.X - mSizeData.OM;
            nPosY = Pos.Y - mSizeData.OQ;
            break;

        case 4:
            nPosX = Pos.X - mSizeData.ON;
            nPosY = Pos.Y;
            break;

        case 5:
            nPosX = Pos.X - mSizeData.OM;
            nPosY = Pos.Y + mSizeData.OQ;
            break;
    }

    double tau = 0;
    double gamma = 0;
    double rho = 0;
	
	// right and left side need different values
    if(ID > 2) {    // right side; legs 3, 4, 5
        // Calculate lengths
        dist = sqrt(pow(nPosX - mSizeData.NB, 2) + pow(nPosY, 2));
        c = sqrt(pow(Pos.Z, 2) + pow(dist, 2));
		
		// calculate angles
        rho   = AngleFemur(nPosX, -nPosY);
        tau   = AngleTibia(mSizeData.a, mSizeData.b, c, Pos.Z, dist);
        gamma = AngleTarsus(mSizeData.a, mSizeData.b, c);

    } else {		// left side
		// calculate lengths
        dist = sqrt(pow(nPosX + mSizeData.NB, 2) + pow(nPosY, 2));
        c =    sqrt(pow(Pos.Z, 2) + pow(dist, 2));
		
		// calculate angles
        rho   = AngleFemur(nPosX, nPosY);
        tau   = AngleTibia(mSizeData.a, mSizeData.b, c, Pos.Z, dist);
        gamma = AngleTarsus(mSizeData.a, mSizeData.b, c);

    }
	
	// Debug info - remove later
	/*
	std::cout << "[Leg:" << ID << "]"
             << std::setiosflags (std::ios::fixed)
             << std::setprecision(0)
             << " x:" << std::setw(4) << Pos.X
             << " y:" << std::setw(4) << Pos.Y
             << " z:" << std::setw(4) << Pos.Z
             << std::fixed << std::setprecision(0) << " [Angles]"
             << " Rho:"   << std::setw(4) << rho
             << " Tau:"   << std::setw(4) << tau
             << " Gamma:" << std::setw(4) << gamma << endl;

	*/

	tmp.AngleFemur  = rho;
	tmp.AngleTibia  = tau;
	tmp.AngleTarsus = gamma;

    return tmp;
}

double InverseKinematic::AngleFemur(double const &x, double const &ny) const {
    double rho = 0.0;

    if(ny != 0) {
        rho = ((atan(ny / x) * 180) / M_PI);
    } else
        rho = 0.0;

    return rho;
}

double InverseKinematic::AngleTibia(double const &a, double const &b,
                                    double const &c, double const &z,
                                    double const &dist) const {

    double x = (pow(c, 2) + pow(a, 2) - pow(b, 2)) / (2*c*b);

    if(x > 1.0 || x < -1.0) {
		std::cout << "            ++++++++Tibia: invalid pos+++++++" << std::endl;
		std::cout << "            a:" << a << ";b:" << b <<";c:" << c << ";z:" << z << ";dist:" << dist << endl;
    }

    double beta = acos( x ) * 180 / M_PI;

    double tau = atan(dist / abs(z)) * 180 / M_PI;
    tau += beta;
    tau -= 90;

    return tau;
}

double InverseKinematic::AngleTarsus(double const &a, double const &b,
                                     double const &c) const {

    double x = (pow(b, 2) + pow(a, 2) - pow(c, 2)) / (2*a*b);

    if(x > 1.0 || x < -1.0) {
		std::cout << "              ++++++++Tarsus: invalid pos+++++++|" << std::endl;
		std::cout << "              a:" << a << ";b:" << b <<";c:" << c << endl;
    }

    double gamma = acos( x ) * 180 / M_PI;

    gamma -= 90;

    return gamma;
}
