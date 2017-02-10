/*
 * servo.cpp
 *
 *  Created on: 02.07.2013
 *      Author: josef
 */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
//#include <math.h>
#include <cmath>

#ifdef __unix__
    #include <curses.h>
#endif

#include <string>

#include "Servo.h"

using namespace std;

//Servo::Servo() : mDev(0), mPort(0), mPWM_Min(0), mPWM_Max(0), mAngle_Min(0), mAngle_Max(0) {}

/**
 * \brief	CTor
 * \author	Josef Sommerauer
 * \param	Dev			Block Device of Servo
 * \param	Port		Portnumber of Servo
 * \param	PWM_Min		Minimal PWM-Value
 * \param	PWM_Max		Maximal PWM-Value
 * \param	Angle_Min	Minmal Angle
 * \param	Angle_Max	Maximal Angle
 */
Servo::Servo(std::ostream * Dev, int Port, int PWM_Min, int PWM_Max, int Angle_Min, int Angle_Max) :
		mDev(Dev), mPort(Port), mPWM_Min(PWM_Min), mPWM_Max(PWM_Max), mAngle_Min(Angle_Min), mAngle_Max(Angle_Max), mAngle(0) {};

/**
 * \brief	DTor
 * \author	Josef Sommerauer
 */
Servo::~Servo() {
	Off();
}

/**
 * \brief	sets the pwm value for a servo.
 * \author	Josef Sommerauer
 * \param	value pwm value
 * \return	false if pwm is out of range or in case of an driver error.
*/
bool Servo::SetPWM(int value) {

	// validate Value
	if(mPWM_Min<mPWM_Max && (value > mPWM_Max || value < mPWM_Min))
		return false;

	if(mPWM_Min>mPWM_Max && (value < mPWM_Max || value > mPWM_Min))
		return false;

	// Check if Blockdevice is defined and OK
	if (mDev == 0 || !mDev->good())
		return false;

	// write value to Blockdevice
	(*mDev) << mPort << ":" << value << std::endl;

	return true;
}

/**
 * \brief	deactivates servo, stop to send pwm values to servo.
 * \author	Josef Sommerauer
 * \return false in case of an driver error.
 */
bool Servo::Off() {
	if (mDev == 0 || !mDev->good()) return false;

	(*mDev) << mPort << ":" << "OFF" << std::endl;
	return true;
}

/**
 * \brief	sets the angle of the servo.
 * \author	Josef Sommerauer
 * \param	value	angle in degree
 * \return	false if angle is out of range or in case of an driver error.
 */
bool Servo::SetAngle(double const &value) {

	// Validate Value
	if(std::isnan(value) != 0)
	{
		return false;
	}

	mAngle = value;

	// Calculate Factor
	int AngleRange = abs(mAngle_Max-mAngle_Min);
	int PWMRange = abs(mPWM_Max-mPWM_Min);
	double conFactor = (double)AngleRange / PWMRange;

	int PWMValue = 0;

	// Calculate PWM
	if(mPWM_Min < mPWM_Max) {
		PWMValue = mPWM_Min + ((value+abs(mAngle_Min)) / conFactor);
	} else {
		PWMValue = mPWM_Min - ((value+abs(mAngle_Min)) / conFactor);
	}

	// Write PWM to Blockdevice
	return SetPWM(PWMValue);
}

/**
 * \brief	returns the current angle of the servo.
 * \author	Josef Sommerauer
 *	\return current angle of the servo
 */
int Servo::GetAngle() const{
	return mAngle;
}

/**
 * \brief	return the maximal angle of the servo.
 * \author	Josef Sommerauer
 * \return maximal angle of the servo
 */
int Servo::GetMaxAngle() const{
    return mAngle_Max;
}

/**
 * \brief	return the minimal angle of the servo.
 * \author	Josef Sommerauer
 * \return minimal angle of the servo
 */
int Servo::GetMinAngle() const{
    return mAngle_Min;
}

/**
 * \brief		calibrate servo. the user has the enter + and - to find the maximal und minimal angle of the servo.
 * \author		Josef Sommerauer
 * \attention	{only works on pandaboard with connected servos}
 */
 void Servo::Calibrate() {

	// // ToDo: Implement in Hexapod-Class!!!

	// // ncurses starten und initialisieren
	// initscr();
	// start_color();			/* Start color 			*/
	// init_pair(1, COLOR_RED, COLOR_BLACK);
	// attron(COLOR_PAIR(1));

	// // set minimal Angle
	// if(mAngle_Min == 0) {
		// std::cout << "Min. Angle: ";
		// std::cin >> mAngle_Min;
	// }

	// // set maximal Angle
	// if(mAngle_Max == 0) {
		// std::cout << "Max. Angle: ";
		// std::cin >> mAngle_Max;
	// }


	// mPWM_Min = 0;
	// char Input = 0;

	// char header[256];
	// sprintf(header, "Servo Number %d\n", mPort);

	// printw(header);

	// printw("Try to find PWM Value for Min. Angle\n");
	// printw("  - Press [+] or [-] to find correct Angle\n");
	// printw("  - Press [Enter] if you found the right Angle\n");

	// refresh();

	// noecho();

	// while(Input!='\n') {
		// SetPWM(mPWM_Min);

		// Input=getch();

		// if(Input=='+') mPWM_Min++;
		// if(Input=='-') mPWM_Min--;
	// }

	// Input = 0;

	// printw("Try to find PWM Value for Max. Angle\n");
	// printw("  - Press [+] or [-] to find correct Angle\n");
	// printw("  - Press [Enter] if you found the right Angle\n");

	// refresh();

	// noecho();
	// while(Input!='\n') {
		// SetPWM(mPWM_Max);

		// Input=getch();

		// if(Input=='+') mPWM_Max++;
		// if(Input=='-') mPWM_Max--;
	// }

	// // close ncurses
	// attroff(COLOR_PAIR(1));
	// clear();
	// endwin();
}

/**
 * \brief	prints all settings of an servo.
 * \author	Josef Sommerauer
 * \return	false in case of an driver error.
*/
void Servo::Print(std::ostream &o) const {
	o << "Port:" << mPort << ", PWM_mi" << mPWM_Min << ", PWM_ma" << mPWM_Max << std::endl;
}
