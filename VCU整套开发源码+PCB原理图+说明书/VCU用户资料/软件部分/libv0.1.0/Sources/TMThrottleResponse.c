/*=================================================================================== 
File name:   TMThrottleResponse.c   
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
#include "TMThrottleResponse.h"
 
 word ratio(long y1 , long y2 , word scale);
 
//---------------------------------------------------------------------
/* struct function definitions 										*/
//---------------------------------------------------------------------
void TMTHROTTLERESPONSE_calc(TMTHROTTLERESPONSE *v)
{ 
  long temp1 = 0;
	long temp2 = 0;
	long ytemp1 = 0;
  long ytemp2 = 0;

	if(v->DirectionInput == DH_FORWARD)
	{
		/* calculate the 0% throttle curve */
		if((v->SpeedInput >= 0) && (v->SpeedInput < v->NeutralTaperSpeed))
		{ 
		  temp1 = (v->NeutralBrakeTorque + v->CreepTorque);
		   
		  temp1  =  (temp1 << 15);
		
			temp1 =  temp1 / v->NeutralTaperSpeed;
			
			temp1  =    (- temp1) * v->SpeedInput;
			
			temp1 = 	temp1 >> 15;
			
			ytemp1 =  temp1  + v->CreepTorque;
		}
		else if((v->SpeedInput < 0) && (v->SpeedInput >= -v->NeutralTaperSpeed))
		{
		  temp1 = (v->CreepTorque - v->RestraintTorque_F);
		  
		  temp1  =  (temp1 << 15);
		
			temp1 =  temp1 / v->NeutralTaperSpeed;
			
		
	  	temp1  =  (temp1) * v->SpeedInput;
			
			temp1 = 	temp1 >> 15;
			
			ytemp1 = temp1 + v->CreepTorque;
		}
		else if(v->SpeedInput >= v->NeutralTaperSpeed)
		{ 
		  temp1 = v->NeutralBrakeTorque;
		  
			ytemp1 = -temp1;
		}
		else
		{
			ytemp1 = v->RestraintTorque_F;
		}

		/* calculate the 100% throttle curve */
		if((v->SpeedInput >= -v->ReversalSoftenSpeed) && (v->SpeedInput <= v->ReversalSoftenSpeed))
		{
			temp1  = (v->DriveLimit + v->RegenLimit) >> 1;
			
			temp2 = (v->DriveLimit - temp1);
			
			temp2 = temp2 << 15;
			
			temp2 = temp2 / v->ReversalSoftenSpeed;
			
			temp2 = temp2 * v->SpeedInput;
			
			temp2 = temp2 >> 15;
			
			ytemp2 = temp2 + temp1 ;
		}
		else if(v->SpeedInput > v->ReversalSoftenSpeed)
		{
			ytemp2 = v->DriveLimit;
		}
		else
		{
			ytemp2 = v->RegenLimit;
		}
	}
	else if(v->DirectionInput == DH_BACKWARD)
	{
		/* calculate the 0% throttle curve */
		if((v->SpeedInput >= 0) && (v->SpeedInput < v->NeutralTaperSpeed))
		{
      temp1 = v->CreepTorque; 
      temp1 = temp1 - v->RestraintTorque_B;
      
      temp1 = (temp1 << 15);
      
      temp1 = temp1 / v->NeutralTaperSpeed ;


      temp1 = temp1 * v->SpeedInput;
      
      temp1 = temp1 >> 15;

      ytemp1 = temp1 - v->CreepTorque;                              
		
		}
		else if((v->SpeedInput < 0) && (v->SpeedInput >= -v->NeutralTaperSpeed))
    {  
      temp1 = v->NeutralBrakeTorque + v->CreepTorque;

      temp1 = temp1 << 15;
      temp1 =  temp1 / v->NeutralTaperSpeed;

      temp1 = (- temp1) * v->SpeedInput ;

      temp1 =    temp1 >> 15;

      ytemp1 = temp1 - v->CreepTorque;
		}
		
		else if(v->SpeedInput >= v->NeutralTaperSpeed)
		{ 
		 temp1 = v->RestraintTorque_B;
		
			ytemp1 = -temp1;
		}
		else
		{
			ytemp1 = v->NeutralBrakeTorque;
		}

		/* calculate the 100% throttle curve */
		if((v->SpeedInput >= -v->ReversalSoftenSpeed) && (v->SpeedInput <= v->ReversalSoftenSpeed))
		{
		   temp1  = (v->DriveLimit + v->RegenLimit) >> 1;
		   
		   temp2 =  v->RegenLimit; 
		   temp2 = temp2 - temp1;
		   temp2 = temp2 <<  15;                  
		                       
		   temp2 = (-temp2) / v->ReversalSoftenSpeed;
		   temp2  = temp2 * v->SpeedInput;
		   temp2  = temp2 >> 15;
		    
		   ytemp2 = temp2 - temp1;			
	  }
		else if(v->SpeedInput > v->ReversalSoftenSpeed)
		{ 
		  temp2 = v->RegenLimit;
			ytemp2 = - temp2;
		}
		else
		{ temp2 = v->DriveLimit; 
			ytemp2 = - temp2;
		}
	}
  /* calculate torque based on throttle percentage */
	 v->TorqueOut = ratio(ytemp1 , ytemp2 , v->ThrottleInput); 
}

/* ----------------------------------------
* it returns y value for y = Kx+b
* when x=0 , y=y1 , when x=1 , y=y2
* when x=scale(between 0 to 1) , y=?
* ---------------------------------------- */
word ratio(long y1 , long y2 , word scale)
{
  long temp ;
  temp = (y2-y1);
  temp =  temp * scale;
  temp = temp >> 15;  
	return ((word) (temp + y1));
}
