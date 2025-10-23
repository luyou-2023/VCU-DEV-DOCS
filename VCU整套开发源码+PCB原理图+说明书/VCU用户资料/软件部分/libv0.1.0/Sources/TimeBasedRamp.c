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
 
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "TimeBasedRamp.h"

//---------------------------------------------------------------
/* Notice , this function is based on calculation per 4ms -----*/
/* otherwise the step temp should be changed------------------ */
//---------------------------------------------------------------
void timebasedramp_calc(TIMEBASEDRAMP *v)
{
     long temp = 0;

     temp = v->TargetValue - v->SetpointValue;
      if(temp <0 )    temp = -temp ;
      
     if (v->TargetValue >= v->SetpointValue)
     {
    	 if ( ((int)temp)  > v->RampUpStep)
    	 {
    		 v->SetpointValue += v->RampUpStep;
    		 /* make sure it is not out range */
    		 if (v->SetpointValue > v->RampHighLimit)
    		 {
    			 v->SetpointValue = v->RampHighLimit;
    		 }
     		 if (v->SetpointValue < v->RampLowLimit)
     		 {
     			 v->SetpointValue = v->RampLowLimit;
     		 }
    	 }
    	 else
    	 {
    	       	v->EqualFlag = 1;
    	       	v->SetpointValue = v->TargetValue;
    	 }
      }
      else
      {
     	 if( ((int)temp) > v->RampDownStep)
     	 {
     		 v->SetpointValue -= v->RampDownStep;
    		 if (v->SetpointValue > v->RampHighLimit)
    		 {
    			 v->SetpointValue = v->RampHighLimit;
    		 }
     		 if (v->SetpointValue < v->RampLowLimit)
     		 {
     			 v->SetpointValue = v->RampLowLimit;
     		 }
     	 }
     	 else
     	 {
     	       	v->EqualFlag = 1;
     	       	v->SetpointValue = v->TargetValue;
     	 }
      }
}