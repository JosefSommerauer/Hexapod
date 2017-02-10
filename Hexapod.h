/////////////////////////////////////////////////////////////////////////////
// Workfile    : Hexapod.h
// Author      : Daniel Wolfmayr
// Date        : 2013/11/20
// Description :
// Project	   : RoboMotion
// Revision    :
/////////////////////////////////////////////////////////////////////////////

#ifndef HEXAPOD_H_INCLUDED
#define HEXAPOD_H_INCLUDED

#define SOCK_DELAY 1000000

#include <signal.h>

#include "Object.h"
#include "SocketServer.h"
#include "Movement.h"
#include "TripodGait.h"
#include "WaveGait.h"
#include "RippleGait.h"
#include <unistd.h>
#include <vector>
#include <string>

typedef std::vector<Movement*> Movements;

/*!
 * \brief This is the Mainclass of Hexapod and includes CtrlSocket-Class and Movement-Class
 * \date 2013/11/20
 * \author Daniel Wolfmayr
 */
class Hexapod : public Object{
public:

	//! CTor
	/*!
	\param PortNbr Portnumber of Server
	\param lc Textfile for servo-definition
	\author Daniel Wolfmayr
	*/
	Hexapod(int PortNbr, LegCtrl * lc);

	//! DTor
	/*!
	 * \author Daniel Wolfmayr
	 */
	~Hexapod();

	//! Enumeration of Movement-Styles
	/*
	 * \author Daniel Wolfmayr
	 */
	enum TGait {TRIPOD, WAVE, RIPPLE};
	
	//! starts the server and the Hexapod and interprets the commands from server
	/*!
	 * \author	Daniel Wolfmayr
	 * \return 	a "1" in case of stop-command
	*/
	int RunHexapod();
private:
	//!Vector with all gaits
	Movements mMov;
	//!Pointer to an specified gait
	Movement * mpGait;
	CtrlSocket mSocket;
	//bool isSocketRunning;
	CtrlSocket::TCmd mCmd;
};

#endif
