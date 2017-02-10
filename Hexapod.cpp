/////////////////////////////////////////////////////////////////////////////
// Workfile    : Hexapod.cpp											
// Author      : Daniel Wolfmayr										
// Date        : 2013/11/21															
// Description : 
// Project	   : RoboMotion												
// Revision    : 			
/////////////////////////////////////////////////////////////////////////////

#include "Hexapod.h"
#include <iostream>
#include <unistd.h>

// terminate programm proper
bool run = true;

/// Handler to Catch CTRL + C to terminate Program
void handler(int signum) {
   std::cout << std::endl << "+++ [Ctrl] + C caught: exit programm! +++" << std::endl;
   run = false;
}

//! CTor
/*!
\param PortNbr Portnumber of Server
\param lc Textfile for servo-definition
*/
Hexapod::Hexapod(int PortNbr, LegCtrl * lc) :
		mSocket(PortNbr),
		mCmd(CtrlSocket::TCmd::nocmd) {

	// Init Movement-Algorithmns
	Movement * pTripod = new TripodGait(lc);
	Movement * pWave   = new WaveGait(lc);
	Movement * pRipple = new RippleGait(lc);

	// Add Algorithmns to Container
	mMov.push_back(pTripod);
	mMov.push_back(pWave);
	mMov.push_back(pRipple);

	// Set Tripod-Gait as Default
	mpGait = mMov[TRIPOD];
} // CTor

//! DTor
Hexapod::~Hexapod(){
	// Remove all Algorithmns from Container
	for(size_t i=0; i<mMov.size(); i++){
		delete mMov[i]; mMov[i] = 0;
	}
} // DTor

//! starts the server and the Hexapod and interprets the commands from server
/*!
\return a "1" in case of stop-command
*/
int Hexapod::RunHexapod(){

	// Open Socket
	run = mSocket.Run();

	// Connect Signal Handler
	signal(SIGINT, handler);

	// Wakeup Hexapod
	mpGait->WakeUp();

	while(true){
		// Read Command
		mCmd = mSocket.GetCmd();

		// end server on ctrl-c
		if(!run) mCmd = CtrlSocket::shutdown;

		//interpret command
		switch (mCmd)
		{
			// Move Left
		case CtrlSocket::moveleft:
			mpGait->MoveLeftSide();
			break;

			// Move Right
		case CtrlSocket::moveright:
			mpGait->MoveRightSide();
			break;

			// Move Forward
		case CtrlSocket::moveforward:
			mpGait->MoveForward();
			break;

			// Move Backward
		case CtrlSocket::movereverse:
			mpGait->MoveBackward();
			break;

			// Turn to the left Side
		case CtrlSocket::turnleft:
			mpGait->RotateLeft();
			break;

			// Turn to the right Side
		case CtrlSocket::turnright:
			mpGait->RotateRight();
			break;

			// Select Tripod Gait
		case CtrlSocket::tripod:
			mpGait = mMov[TRIPOD];
			break;

			// Select Wave Gait
		case CtrlSocket::wave:
			mpGait = mMov[WAVE];
			break;

			// Select Ripple Gait
		case CtrlSocket::ripple:
			mpGait = mMov[RIPPLE];
			break;

			// Send Hexapod to Standby
		case CtrlSocket::sleep:
			mpGait->Sleep();
			break;

			// Wakeup Hexapod
		case CtrlSocket::wakeup:
			mpGait->WakeUp();
			break;

			// Do nothing
		case CtrlSocket::nocmd:
			break;

			// shutdown Server
		case CtrlSocket::shutdown:
			mpGait->Sleep();
			usleep(500000);
			mSocket.Stop();
			return 0;
			break;

			// Socketerror
		case CtrlSocket::error:
		 std::cout << std::endl << "+++ Error in Socket Communication +++" << std::endl;
			break;

			// No Valid Command -> do nothing
		default:
			break;
		}
	}

	return 1;
} // RunHexapod
