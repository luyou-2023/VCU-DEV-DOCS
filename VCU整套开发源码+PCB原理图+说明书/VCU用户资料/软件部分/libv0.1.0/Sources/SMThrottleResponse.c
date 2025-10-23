/*=================================================================================== 
File name:   SMThrottleResponse.c   
Description:  
Created on:  01-12-2014
Author: JT
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 2013-5-29	Version 0.1.0    JT		Initialized version   
-------------------------------------------------------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "SMThrottleResponse.h"

 int ratiowithoffset(word y1 , long y2 , word a , word b , word scale);
 
//---------------------------------------------------------------------
/* struct function definitions 										*/
/* notice it is based on 4ms per step										*/
//---------------------------------------------------------------------
void SMTHROTTLERESPONSE_calc(SMTHROTTLERESPONSE *v)
{
	long temp1 = 0;
	long temp2 = 0;
	long temp3 = 0;
	long ytemp2 = 0;
  long temp = 0; 
      
	if(v->DirectionInput == DH_FORWARD)
	{
		if(v->SpeedInput >= 0)
		{
			if(v->SpeedInput < v->LowSpeed)
			{
				ytemp2 = v->LSFullThrottleStep;
			}
			else if(v->SpeedInput > v->HighSpeed)
			{
				ytemp2 = v->HSFullThrottleStep;
			}
			else
			{
			  temp = v->HSFullThrottleStep - v->LSFullThrottleStep;
			  
			  temp1 =  (temp << 15) / (v->HighSpeed - v->LowSpeed);
				temp =  v->LSFullThrottleStep ;
				temp = temp * v->HighSpeed ;
				
				temp3 = v->HSFullThrottleStep; 
				temp3 = temp3 * v->LowSpeed;
				temp = 	temp - temp3;
				
				temp2 =	temp / (v->HighSpeed - v->LowSpeed);
				
				 temp = temp1 * v->SpeedInput;
				 temp = temp >>15;
				 ytemp2 =  temp + temp2;
			}

			if(v->ThrottleInput < 3277)		/* 3277 = _IQ15(0.1) */
			{
				v->StepOut = v->LowThrottleStep;
			}
			else if(v->ThrottleInput > 29491)	/* 29491 = _IQ15(0.9) */
			{
				v->StepOut = (int)ytemp2;
			}
			else
			{
				v->StepOut = ratiowithoffset(v->LowThrottleStep , ytemp2 , 3277 , 29491 , v->ThrottleInput);
			}
		}
	}
	else if(v->DirectionInput == DH_BACKWARD)
	{
		if(v->SpeedInput <= 0)
		{
			if(v->SpeedInput > -v->LowSpeed)
			{
				ytemp2 = v->LSFullThrottleStep;
			}
			else if(v->SpeedInput < -v->HighSpeed)
			{
				ytemp2 = v->HSFullThrottleStep;
			}
			else
			{
			  temp = v->HSFullThrottleStep - v->LSFullThrottleStep;
			  temp1 =  (temp << 15) / (-v->HighSpeed + v->LowSpeed);
				temp =  (v->LSFullThrottleStep );
				temp = 	temp * v->HighSpeed ;
				temp3 = v->HSFullThrottleStep ;
				temp3 = temp3 * v->LowSpeed;
				temp = (-temp) + (temp3);
				
				temp2 =	temp / (-v->HighSpeed + v->LowSpeed);	
		 
				 temp = temp1 * v->SpeedInput;
				 
				 temp = temp >>15;
				 ytemp2 =  temp + temp2;
				
					
				
			}

			if(v->ThrottleInput < 3277)		/* 3277 = _IQ15(0.1) */
			{
				v->StepOut = v->LowThrottleStep;
			}
			else if(v->ThrottleInput > 29491)	/* 29491 = _IQ15(0.9) */
			{
				v->StepOut = (int)ytemp2;
			}
			else
			{
				v->StepOut = ratiowithoffset(v->LowThrottleStep , ytemp2 , 3277 , 29491 ,v->ThrottleInput);
			}
		}
	}
}
 
/* ----------------------------------------
* it returns y value for y = Kx+b
* when x=a , y=y1 , when x=b , y=y2
* when x=scale(between a to b) , y=?
* ---------------------------------------- */
int ratiowithoffset(word y1 , long y2 , word a , word b , word scale)
{
   long temp = b;
   temp = (y2-y1) ;
   temp = temp << 15;
   temp = temp / 26214;
   temp = temp *(scale - a);
   temp = temp >> 15; 
	return ((int)(temp + y1));
}
