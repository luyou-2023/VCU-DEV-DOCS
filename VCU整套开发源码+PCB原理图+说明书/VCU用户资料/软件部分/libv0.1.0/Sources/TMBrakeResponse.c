/*====================================================================================
 File name:   TMBrakeResponse.c   
 Description:    
 Created on:  01-12-2014
 Author:      JT
======================================================================================
 History:
-------------------------------------------------------------------------------------
01-12-2014	 Version 0.1.0   JT		Initialized version   
-------------------------------------------------------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "TMBrakeResponse.h"
//---------------------------------------------------------------------
/* struct function definitions 										*/
/* 2012-09-29				 										*/
//---------------------------------------------------------------------
void TMBRAKERESPONSE_calc(TMBRAKERESPONSE *v)
{
  long temp = 0;

	/* calculate the brake curve */

	if(v->SpeedInput < -v->BrakeTaperSpeed)
	{
    temp =  v->BrakeLimit; 
    temp = temp * v->BrakeInput;
    temp = temp >> 15;
    v->TorqueOut = (int) temp;
	}
	else if(v->SpeedInput > v->BrakeTaperSpeed)
	{
	
	
	  temp = v->BrakeLimit;
	  temp = temp * v->BrakeInput;
    temp = (-temp) >> 15;
		v->TorqueOut = (int) temp;
	}
	else
	{
		
	  temp = v->BrakeLimit;
	   temp =  temp << 15;
	  temp =  (-temp) / v->BrakeTaperSpeed;
	  temp =  temp * v->SpeedInput;
    temp =  temp  >> 15;
	  temp = temp * v->BrakeInput;
	  temp = (temp) >> 15;
	  v->TorqueOut  = (int)temp;
	}
}


