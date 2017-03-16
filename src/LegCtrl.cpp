#include <curses.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include "Definitions.h" // PI
#include "Servo.h"
#include "LegCtrl.h"
#include "Parser.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h>

#include <boost/asio.hpp>

using namespace std;

/**
 * \brief	Default CTor
 * \author	Josef Sommerauer
 * \param servo_dev Path to Servo-Interface
 */
LegCtrl::LegCtrl(std::string const & servo_dev) : mFs(io) {
    // Open servo file stream

/*
	 mFs.open(servo_dev.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
    if(!mFs.is_open())
        throw "LegCtrl::LegCtrl(): can not open: " + servo_dev;
*/

	 mFs.open(servo_dev);
	 mFs.set_option(boost::asio::serial_port_base::baud_rate(115200));

    // Read default leg pos
    mPars = new Parser("DefaultLegPos.txt");

    if(mPars && mPars->ParseFileDefaultPos() && mPars->GetData(mDefLegPos)) {
        cout << "Parsed default position successfully!" << endl;
    } else {
        throw "LegCtrl::LegCtrl(): can not open: DefaultLegPos.txt";
    }
    if(mPars)
        delete mPars;

    // Read servo values
    mPars = new Parser("ServoValues.txt");

    if(mPars && mPars->ParseFileServos() && mPars->GetData(mServoValues)) {
        cout << "Parsed servo values successfully!" << endl;
    } else {
        throw "LegCtrl::LegCtrl(): can not open: ServoValues.txt";
    }
    if(mPars)
        delete mPars;

    // build legs
    for(int i = 0; i < NUM_OF_LEGS; ++i) {
        mLegs[i] = new Leg(i, mDefLegPos[i],
                         Servo(&mFs, mServoValues[(3*i)+0][0], // Femur
									 mServoValues[(3*i)+0][1],  
                                     mServoValues[(3*i)+0][2],
                                     mServoValues[(3*i)+0][3],
                                     mServoValues[(3*i)+0][4]),
                         Servo(&mFs, mServoValues[(3*i)+1][0],  // Tibia
                                     mServoValues[(3*i)+1][1],
                                     mServoValues[(3*i)+1][2],
                                     mServoValues[(3*i)+1][3],
									 mServoValues[(3*i)+2][4]),
                         Servo(&mFs, mServoValues[(3*i)+2][0],  // Tarsus
                                     mServoValues[(3*i)+2][1],
                                     mServoValues[(3*i)+2][2],
                                     mServoValues[(3*i)+2][3],
									 mServoValues[(3*i)+2][4]));
    }

    mReady = false;

    for(int i = 0; i < NUM_OF_LEGS; i++) { 
		mSleepAngles[i] = mLegs[i]->GetAngle();
	}
}

/**
 * \brief	Default DTor
 * \author	Josef Sommerauer
 */
LegCtrl::~LegCtrl() {
    for(int i = 0; i < NUM_OF_LEGS; i++) {
        mLegs[i]->Off();
        delete mLegs[i];
    }

    mFs.close();
}

/** \brief	Sets a leg to the specified position
 * \author	Josef Sommerauer
 * \param 	ID 	Number of leg
 * \param 	Pos Koordinates of the leg
 * \return 	True if function succeedes
 */
bool LegCtrl::SetXYZ(int ID, T3DPosition const &Pos) {
    if(ID < 0 || ID >= NUM_OF_LEGS)
      throw "LegCtrl::SetXYZ: ID out of range";

    m3DPos[ID] = Pos; // save last position
    return mLegs[ID]->SetAngle(mInvKin.CalculateAngles(ID, Pos));
}

/**
 * \brief	Sets the angles of a leg
 * \author	Josef Sommerauer
 * \param	ID	Number of leg
 * \param	Ang	Angle set of a leg
 * \return	True if function succeeded
 */
bool LegCtrl::SetAngle(int ID, TAngles const &Ang) {
    if(ID < 0 || ID >= NUM_OF_LEGS)
        return false;

    return mLegs[ID]->SetAngle(Ang);
}

/**
 * \brief	Returns the coordinates of a leg
 * \author	Josef Sommerauer
 * \param	ID	Number of leg
 * \return	Current coordinates of a leg
 */
T3DPosition LegCtrl::GetXYZ(int ID) const {
    if(ID < 0 || ID >= NUM_OF_LEGS)
      throw "LegCtrl::GetXYZ: ID out of range";

    return m3DPos[ID];
}

/**
 * \brief	Returns the angles of a leg
 * \author	Josef Sommerauer
 * \param	ID Number of leg
 * \return	angles of a leg
 */
TAngles LegCtrl::GetAngles(int ID) const {
   if(ID < 0 || ID >= NUM_OF_LEGS)
      throw "LegCtrl::GetAngles: ID out of range";

	return mLegs[ID]->GetAngle();
}

/**
 * \brief	Sets all legs to a default position
 * \author	Josef Sommerauer
 */
void LegCtrl::ReadyPosition() {

	std::vector<TAngles> mEndAng;

	for(int i = 0; i < NUM_OF_LEGS; i++) { 
		m3DPos[i] = mLegs[i]->GetOriginPos();
		mEndAng.push_back(mInvKin.CalculateAngles(i, m3DPos[i]));
	}

	int nrSteps = 25;

	for (int j = 1; j <= nrSteps; j++) {
	   for(int i = 0; i < NUM_OF_LEGS; i++) {
	   		TAngles tmp = (mEndAng[i] - mSleepAngles[i]) / nrSteps; // gesamt
	   		TAngles now = mSleepAngles[i] + (tmp * j);
			if(!mLegs[i]->SetAngle(now)) {std::cout << "error";}
			
	   }
	   usleep(10000);
	}

   	mReady = true;
}

/*
 * \brief	Checks if Leg Ctrl is ready
 * \author	Josef Sommerauer
 * \return	True if legs are ready
 */
bool LegCtrl::IsReady() const {
   return mReady;
}

/**
 * \brief	Disables all legs
 * \author	Josef Sommerauer
 */
void LegCtrl::AllOff() {
   for(int i = 0; i < NUM_OF_LEGS; i+=2) {
     mLegs[i]->Off();
   }

   usleep(40000); // wait to reduce peak current 
               // otherwise brownout detection 
               // would shutdown he pandaboard

   for(int i = 1; i < NUM_OF_LEGS; i+=2) {
     mLegs[i]->Off();
   }
}

/**
 * \brief	Sets all legs to sleep mode
 * \author	Josef Sommerauer
 */
void LegCtrl::AllSleep() {
	/*
    for(int i = 0; i < NUM_OF_LEGS; i++) {
        mLegs[i]->Sleep();
    }
    */
    std::vector<TAngles> mStartAng;

	for(int i = 0; i < NUM_OF_LEGS; i++) { 
		mStartAng.push_back(mLegs[i]->GetAngle());
	}

	int nrSteps = 25;

	for (int j = 1; j <= nrSteps; j++) {
	   for(int i = 0; i < NUM_OF_LEGS; i++) {
	   		TAngles tmp = (mSleepAngles[i] - mStartAng[i]) / nrSteps; // gesamt
	   		TAngles now = mStartAng[i] + (tmp * j);
			if(!mLegs[i]->SetAngle(now)) {std::cout << "error";}
	   }
	   usleep(10000);
	}
	usleep(100000);
   	mReady = true;
}

/*
 * \brief	Calculate Leg-Positions to do Movement
 * \author	Josef Sommerauer
 * \param	h	height of Hexapod
 * \param	r	Step-width of Hexapod
 * \param	phi	Direction to Move
 */
std::vector<std::vector<T3DPosition> > LegCtrl::CalcMotionPos(
	int h, int r, double const &phi) {

	// Calculate delta X and delta Y Position
	vector<vector<T3DPosition> > retVal;
	int x = cos ( phi * M_PI / 180.0 ) * abs(r);
	int y = sin ( phi * M_PI / 180.0 ) * abs(r);

	// Calculate Targetpositions of each Leg
	for (int i=0;i<NUM_OF_LEGS;i++) {
		vector<T3DPosition> tmpVec;
		T3DPosition tmp;

		T3DPosition pos = mLegs[i]->GetOriginPos();

		// pos 1
		tmp.X = pos.X - x;
		tmp.Y = pos.Y - y;
		tmp.Z = pos.Z;
		tmpVec.push_back(tmp);

		// pos 2
		tmp.X = pos.X + x;
		tmp.Y = pos.Y + y;
		tmp.Z = pos.Z;
		tmpVec.push_back(tmp);

		// pos 3
		tmp.X = pos.X + x;
		tmp.Y = pos.Y + y;
		tmp.Z = pos.Z + h;
		tmpVec.push_back(tmp);

		// pos 4
		tmp.X = pos.X - x;
		tmp.Y = pos.Y - y;
		tmp.Z = pos.Z + h;
		tmpVec.push_back(tmp);

		retVal.push_back(tmpVec);
	}

	return retVal;
}

/*
 * \brief	Calculate Leg-Positions to do Rotation
 * \author	Josef Sommerauer
 * \param	h		Height of Hexapod
 * \param	alpha	Direction to Rotate
 */
std::vector<std::vector<T3DPosition> > LegCtrl::CalcRotatePos(
      int h, double const &alpha) {

	vector<vector<T3DPosition> > retVal;

	// Calculate Targetpositions of each Leg
	for (int i=0;i<NUM_OF_LEGS;i++) {
		vector<T3DPosition> tmpVec;
		T3DPosition tmp;

		T3DPosition pos = mLegs[i]->GetOriginPos();

		// calc absolute value and phase of xy-legposition
		double absVal = sqrt(pow(pos.X, 2) + pow(pos.Y, 2));
		double phi    = atan(pos.X / pos.Y) * 180 / M_PI;

		if (pos.Y != 0 && pos.X < 0) phi += 180; // 2. half of circuit
		else if (pos.Y == 0) phi += 270;

		std::cout << "x,y:"    << std::setw(4) << pos.X << " " << std::setw(4) << pos.Y << " "
			   << "angle:"  << std::setw(4) << phi << " "
			   << "absVal:" << std::setw(4) << absVal << " new => ";

		// pos 1
		tmp.X = absVal * cos((phi * M_PI / 180.0) - (alpha * M_PI / 180.0));
		tmp.Y = absVal * sin((phi * M_PI / 180.0) - (alpha * M_PI / 180.0));
		tmp.Z = pos.Z;
		std::cout << "x,y:" << std::setw(4) << tmp.X << " " << std::setw(4) << tmp.Y << " ";
		tmpVec.push_back(tmp);

		// pos 2
		tmp.X = absVal * cos((phi * M_PI / 180.0) + (alpha * M_PI / 180.0));
		tmp.Y = absVal * sin((phi * M_PI / 180.0) + (alpha * M_PI / 180.0));
		tmp.Z = pos.Z;
		std::cout << "x,y:" << std::setw(4) << tmp.X << " " << std::setw(4) << tmp.Y << " ";
		tmpVec.push_back(tmp);

		// pos 3
		tmp.X = absVal * cos((phi * M_PI / 180.0) + (alpha * M_PI / 180.0));
		tmp.Y = absVal * sin((phi * M_PI / 180.0) + (alpha * M_PI / 180.0));
		tmp.Z = pos.Z + h;
		std::cout << "x,y:" << std::setw(4) << tmp.X << " " << std::setw(4) << tmp.Y << " ";
		tmpVec.push_back(tmp);

		// pos 4
		tmp.X = absVal * cos((phi * M_PI / 180.0) - (alpha * M_PI / 180.0));
		tmp.Y = absVal * sin((phi * M_PI / 180.0) - (alpha * M_PI / 180.0));
		tmp.Z = pos.Z + h;
		std::cout << "x,y:" << std::setw(4) << tmp.X << " " << std::setw(4) << tmp.Y << " ";
		tmpVec.push_back(tmp);

		retVal.push_back(tmpVec);
		std::cout << std::endl;
	}

	return retVal;
}

void LegCtrl::Calibrate(int leg, int servo) {
	mLegs[leg]->Calibrate(servo);
}

void LegCtrl::Print(int leg, std::ostream &o) {
	mLegs[leg]->Print(o);
}

void LegCtrl::TestSetXYZ() {
   T3DPosition pos;
   int Xoff = 0;
   int Yoff = 0;
   int Zoff = 0;

   char c = 0;

   initscr();

   while(c!='q') {

      for(int i=0;i<NUM_OF_LEGS;++i) {
         pos = mLegs[i]->GetOriginPos();
         if(i<3) pos.X += Xoff;
         else    pos.X -= Xoff;
         pos.Y += Yoff;
         pos.Z += Zoff;
         printw(" %i\n",SetXYZ(i, pos));
      }

      printw("\n");
      printw("Xoff: %i",Xoff); printw(" ");
      printw("Yoff: %i",Yoff); printw(" ");
      printw("Zoff: %i",Zoff); printw("\n");
      refresh();

      c = getch();
      switch(c) {
	      case 'a' : Yoff++;break;
	      case 'y' : Yoff--;break;

	      case 's' : Xoff++;break;
	      case 'x' : Xoff--;break;

	      case 'd' : Zoff++;break;
	      case 'c' : Zoff--;break;

	      default: break;
      }
      clear();

    }
 endwin();
}

