/*
 * @file    : Movement.h
 * @author  : Daniel Wolfmayr
 * @date    : 2013/10/09
 *
 * Baseclass for Movements
 */

// Changelog:
//		- 2013-11-20 Florian Aigner: Redesign of Movement Classes

#include "Movement.h"

#include "Definitions.h"
#include "LegCtrl.h"

#include <vector>
#include <string>

using namespace std;

/*
 * \brief	CTor
 * \author	Daniel Wolfmayr
 * \param	lc	Legcontrol
 */
Movement::Movement(LegCtrl * lc) : mLegCtrl(lc), mdh(h), mds(s){}

/*
 * \brief	Move Hexapod forward
 * \author	Florian Aigner
 * \return 	0 if Successfull; Errorcode else
 */
int Movement::MoveForward()
{
	mMovePos = mLegCtrl->CalcMotionPos(mdh, mds, phiForward);
	return Step();
}

/*
 * \brief	Move Hexapod backward
 * \author	Florian Aigner
 * \return 0 if Successfull; Errorcode else
 */
int Movement::MoveBackward()
{
	mMovePos = mLegCtrl->CalcMotionPos(mdh, mds, phiBackward);
	return Step();
}

/*
 * \brief	Move Hexapod to left side
 * \author	Florian Aigner
 * \return 	0 if Successfull; Errorcode else
 */
int Movement::MoveLeftSide()
{
	mMovePos = mLegCtrl->CalcMotionPos(mdh, mds, phiLeft);
	return Step();
}

/*
 * \brief	Move Hexapod to right side
 * \author 	Florian Aigner
 * \return	0 if Successfull; Errorcode else
 */
int Movement::MoveRightSide()
{
	mMovePos = mLegCtrl->CalcMotionPos(mdh, mds, phiRight);

	return Step();
}

/*
 * \brief	Rotate Hexapod to left side
 * \author	Florian Aigner
 * \return	0 if Successfull; Errorcode else
 */
int Movement::RotateLeft()
{
   mMovePos = mLegCtrl->CalcRotatePos(mdh, alphaRotateLeft);

	return Step();
}

/*
 * \brief	Rotate Hexapod to right side
 * \author	Florian Aigner
 * \return	0 if Successfull; Errorcode else
 */
int Movement::RotateRight()
{
   mMovePos = mLegCtrl->CalcRotatePos(mdh, alphaRotateRight);
   
	return Step();
}

/*
 * \brief	Move Legs to Sleep Position
 * \author	Florian Aigner
 * \return 	0 if Successfull; Errorcode else
 */
int Movement::Sleep()
{
	mLegCtrl->AllSleep();
	return 0;
}

/*
 * \brief	Move Legs to Startposition
 * \author	Florian Aigner
 * @return 0 if Successfull; Errorcode else
 */
int Movement::WakeUp()
{
	mMovePos = mLegCtrl->CalcMotionPos(mdh, mds, 0);
	mLegCtrl->ReadyPosition();

	return 0;
}

/*
 * \brief	Set Height of Movement
 * \author	Florian Aigner
 * \param 	h Height
 */
void Movement::Setdh(int h)
{
	mdh = h;
}

/*
 * \brief	Get Height of Movement
 * \author	Florian Aigner
 * \return 	Height
 */
int Movement::Getdh()
{
	return mdh;
}

/*
 * \brief	Set Spatium
 * \author	Florian Aigner
 * \param 	s Spatium
 */
void Movement::Setds(int s)
{
	mds = s;
}

/*
 * \brief	Get Spatium
 * \author	Florian Aigner
 * \return	Spatium
 */
int Movement::Getds()
{
	return mds;
}
