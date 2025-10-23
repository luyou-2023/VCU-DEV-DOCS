/*====================================================================================
* File name: 	 limit_map.c
* Description: vehicle related.  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "limit.h"		 // Include header of limit.c

void Limit_Map(LIMIT *limit)
{

  long temp;
  
	if(limit->Status.Mode == 0)
	{
		limit->SpeedPtr = &limit->EleSpeed[0];
		limit->TorquePtr = &limit->EleTorque[0];
	}
	else
	{
		limit->SpeedPtr = &limit->GenSpeed[0];
		limit->TorquePtr = &limit->GenTorque[0];
	}
	
	limit->Status.SpeedCnt = 0;
	
  if(limit->SpeedFbk<0)
  {
    limit->SpeedFbk = 0 - limit->SpeedFbk;   
  }
	
	while((limit->SpeedFbk >= (*limit->SpeedPtr)) && (limit->Status.SpeedCnt < 6))
	{
		limit->Status.SpeedCnt++;
		limit->SpeedPtr++;
		limit->TorquePtr++;
	}

	limit->InteralSpeed = (*limit->SpeedPtr) - (*(limit->SpeedPtr - 1));
	
	limit->DeltaSpeed = limit->SpeedFbk - (*(limit->SpeedPtr - 1));
	
	temp =  limit->DeltaSpeed << 15 ;
	
	temp = temp / limit->InteralSpeed;
	
	limit->RatioSpeed =  (int)temp;

	limit->InteralToque =  (*limit->TorquePtr);
	limit->InteralToque = limit->InteralToque - *(limit->TorquePtr - 1);
	
	temp = limit->RatioSpeed * limit->InteralToque;
	
	limit->Coeff =  (int)(temp >> 15) + *(limit->TorquePtr - 1);
	
}


