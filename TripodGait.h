/////////////////////////////////////////////////////////////////////////////
// Workfile    : TripodGait.h										
// Author      : Daniel Wolfmayr										
// Date        : 2013/10/16															
// Description : 
// Project	   : RoboMotion												
// Revision    : 			
/////////////////////////////////////////////////////////////////////////////

#include "Movement.h"

/// Implementation of Tripod Gait Movement
class TripodGait : public Movement{

public:
   TripodGait(LegCtrl * lc) : Movement(lc) {}

private:
	/*
	 * Proceed single Step
	 * @return 0 if successfully; Errorcode else
	 */
	int Step();
};

