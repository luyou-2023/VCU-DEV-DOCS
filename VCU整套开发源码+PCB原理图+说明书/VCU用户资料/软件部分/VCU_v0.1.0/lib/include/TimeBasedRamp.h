/*====================================================================================
* File name: 	  TimeBasedRamp.C
* Description: The Ramp Control
* Created on:  01-20-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-20-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/
#ifndef __TIMEBASEDRAMP_H__
#define __TIMEBASEDRAMP_H__

typedef struct { int TargetValue; 	// Input: Target input (pu)
				 int RampLowLimit;	// Parameter: Minimum limit (pu)
				 int RampHighLimit;	// Parameter: Maximum limit (pu)
				 int SetpointValue;	// Output: Target output (pu)
				 int RampUpStep;
				 int RampDownStep;
				 word EqualFlag;		// Output: Flag output (Q0) - independently with global Q
				 } TIMEBASEDRAMP;

typedef TIMEBASEDRAMP *TIMEBASEDRAMP_handle;
/*------------------------------------------------------------------------------
Prototypes for the functions in TIMEBASEDRAMP.C
------------------------------------------------------------------------------*/
void timebasedramp_calc(TIMEBASEDRAMP_handle);

#endif // __TIMEBASEDRAMP_H__
