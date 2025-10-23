/*=================================================================================== 
File name:   SMBrakeResponse.c   
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
#include "SMBrakeResponse.h"

int ratiowithoffset(word y1 , long y2 , word a , word b , word scale);
                                                                       
//---------------------------------------------------------------------
/* struct function definitions 										*/
/* notice it is based on 4ms per step										*/
//---------------------------------------------------------------------
void SMBRAKERESPONSE_calc(SMBRAKERESPONSE *v)
{
	long temp1 = 0;
	long temp2 = 0;
	long temp3 = 0;
	long ytemp2 = 0;
  long temp = 0;
  
	if(v->SpeedInput >= 0)
	{
		if(v->SpeedInput < v->LowSpeed)
		{
			ytemp2 = v->LSFullBrakeStep;
		}
		else if(v->SpeedInput > v->HighSpeed)
		{
			ytemp2 = v->HSFullBrakeStep;
		}
		else
		{
		  temp = v->HSFullBrakeStep - v->LSFullBrakeStep;
      temp1 =  (temp << 15)/(v->HighSpeed - v->LowSpeed);
      
      temp =   v->LSFullBrakeStep ;
      temp = temp * v->HighSpeed ;
      temp3 = (v->HSFullBrakeStep );
      temp3 = temp3 * v->LowSpeed;
      temp = 	temp - temp3;
      temp2 =	temp / (v->HighSpeed - v->LowSpeed);
      temp = temp1 * v->SpeedInput;
      temp = temp >> 15;
      ytemp2 = temp + temp2;
		}

		if(v->BrakeInput < 328)		/* 328 = _IQ15(0.01) */
		{
			v->StepOut = v->LowBrakeStep;
		}
		else
		{
			v->StepOut = ratiowithoffset(v->LowBrakeStep , ytemp2 , 328 , 32768 , v->BrakeInput);
		}
	}
	else
	{
		if(v->SpeedInput > -v->LowSpeed)
		{
			ytemp2 = v->LSFullBrakeStep;
		}
		else if(v->SpeedInput < -v->HighSpeed)
		{
			ytemp2 = v->HSFullBrakeStep;
		}
		else
		{
      temp = v->HSFullBrakeStep - v->LSFullBrakeStep;
      temp1 =  (temp << 15)/(-v->HighSpeed + v->LowSpeed);		
      temp =  (v->LSFullBrakeStep );
      temp = temp * v->HighSpeed;
      temp3 = (v->HSFullBrakeStep ); 
      temp3 = temp3 * v->LowSpeed;
      
      temp = (-temp) + temp3;
      temp2 =	temp / (-v->HighSpeed + v->LowSpeed);	
      temp = temp1 * v->SpeedInput;
      temp = temp >>15;
      ytemp2 =  temp + temp2;		
		}

		if(v->BrakeInput < 328)		/* 328 = _IQ15(0.01) */
		{
			v->StepOut = v->LowBrakeStep;
		}
		else
		{
			v->StepOut = ratiowithoffset(v->LowBrakeStep, ytemp2, 328, 32768,v->BrakeInput);
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
   
   temp = (y2-y1); 
   temp =  temp << 15;
   temp = temp / 32440;
   temp = temp *(scale - a);
   temp = temp >> 15; 
   return ((int)(temp + y1));
}
