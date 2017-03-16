/*
 * Servo.h
 *
 *  Created on: 02.07.2013
 *      Author: josef, patrick
 */
#ifndef SERVO_H_
#define SERVO_H_

#include "Object.h"
#include <string>

#include <boost/asio/serial_port.hpp> 
#include <boost/asio.hpp> 

/**
 * \brief	Interface between Software and Kernelmodule
 * \author	Josef Sommerauer
 * \date	2013/07/02
 */
class Servo : public Object{
public:
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
/*
	Servo(std::ostream * Dev, int Port, 
         int PWM_Min,        int PWM_Max, 
         int Angle_Min,      int Angle_Max);
*/

	Servo(boost::asio::serial_port * Dev, int Port, 
         int PWM_Min,        int PWM_Max, 
         int Angle_Min,      int Angle_Max);

    /**
     * \brief	DTor
     * \author	Josef Sommerauer
     */
	~Servo();

	/**
	 * \brief	sets the angle of the servo.
	 * \author	Josef Sommerauer
	 * \param	value	angle in degree
	 * \return	false if angle is out of range or in case of an driver error.
	 */
	bool SetAngle(double const &value);

	bool SetSleepAngle(double const &value);

	/**
	 * \brief	returns the current angle of the servo.
	 * \author	Josef Sommerauer
	 *	\return current angle of the servo
	 */
	int  GetAngle() const;

	/**
	 * \brief	return the maximal angle of the servo.
	 * \author	Josef Sommerauer
	 * \return maximal angle of the servo
	 */
	int GetMaxAngle() const;

	/**
	 * \brief	return the minimal angle of the servo.
	 * \author	Josef Sommerauer
	 * \return minimal angle of the servo
	 */
   int GetMinAngle() const;

	/**
	 * \brief	deactivates servo, stop to send pwm values to servo.
	 * \author	Josef Sommerauer
	 * \return false in case of an driver error.
	 */
	bool Off();

	/**
	 * \brief		calibrate servo. the user has the enter + and - to find the maximal und minimal angle of the servo.
	 * \author		Josef Sommerauer
	 * \attention	{only works on pandaboard with connected servos}
	 */
	void Calibrate();

	/**
	 * \brief	prints all settings of an servo.
	 * \author	Josef Sommerauer
	 * \return	false in case of an driver error.
	*/
	void Print(std::ostream & o) const;

private:
	/**
	 * \brief	sets the pwm value for a servo.
	 * \author	Josef Sommerauer
	 * \param	value pwm value
	 * \return	false if pwm is out of range or in case of an driver error.
	*/
	bool SetPWM(int value);

	/// Stream to write on Blockdevice
	//std::ostream *mDev;
	boost::asio::serial_port * mDev;

	/// GPIO Port
	int mPort;

	/// Minimal PWM Value
	int mPWM_Min;

	/// Maximal PWM Value
	int mPWM_Max;

	/// Minimal Angle
	int mAngle_Min;

	/// Maximal Angle
	int mAngle_Max;

	/// Current Angle
	int mSleepAngle;

	/// Current Angle
	int mAngle;

};


#endif /* SERVO_H_ */
