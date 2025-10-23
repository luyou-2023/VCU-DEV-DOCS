/*=====================================================================================
File name:   ThrottleMap.c   
Description: Throttle process and  check
Created on:  01-12-2014
Author: JT
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 2013-5-29	Version 0.1.0    JT		Initialized version   
-------------------------------------------------------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "throttle_map.h"

word PercentCalc(word x,word y,dword InvDelat,word z);
word DeltaCalc(word a,word b,word c);
word Protection(word a,word b,word c,word IO,word CheckType);
// **************************************************************************
// the functions

THROTTLEMAP_Handle ThrottleMap_init(void *pMemory,const size_t numBytes)
{
	THROTTLEMAP_Handle ThrottleHandle;
	THROTTLEMAP_Obj *Throttle;

  if(numBytes < sizeof(THROTTLEMAP_Obj))
   return((THROTTLEMAP_Handle)NULL);

  // assign the handle
  ThrottleHandle = (THROTTLEMAP_Handle)pMemory;

  // assign the object
  Throttle = (THROTTLEMAP_Obj*)ThrottleHandle;

  memset(Throttle,0,sizeof(THROTTLEMAP_Obj));

  return(ThrottleHandle);
} // end of THROTTLEMAP_init() function

void ThrottleMap_setCheck(THROTTLEMAP_Handle ThrottleHandle,const word CHeckEnable,const word CheckType,const word VerifyRange)
{
	THROTTLEMAP_Obj *throttle = (THROTTLEMAP_Obj *)ThrottleHandle;

	throttle->State.bit.CheckEnable = CHeckEnable;
	throttle->State.bit.CheckType = CheckType;
	throttle->VerifyRange = VerifyRange;
	return;
}// end of THROTTLEMAP_setCheck() function

void ThrottleMap_setVoltage(THROTTLEMAP_Handle ThrottleHandle,const word S1_HighVoltage,\
		const word S1_LowVoltage,const word S2_HighVoltage,const word S2_LowVoltage)
{
	THROTTLEMAP_Obj *throttle = (THROTTLEMAP_Obj *)ThrottleHandle;

	throttle->S1_HighVoltage = S1_HighVoltage;
	throttle->S1_LowVoltage = S1_LowVoltage;
	throttle->DeltaS1HiLowVoltage =  (32768*32768)/(S1_HighVoltage - S1_LowVoltage);//  _IQdiv(_IQ(1.0),(S1_HighVoltage - S1_LowVoltage));
                                         
	throttle->S2_HighVoltage = S2_HighVoltage;
	throttle->S2_LowVoltage = S2_LowVoltage;
	throttle->DeltaS2HiLowVoltage = (32768*32768)/(S2_HighVoltage - S2_LowVoltage);
	return;
} // end of ThrottleMap_setVoltage() function

void ThrottleMap_setProtect(THROTTLEMAP_Handle ThrottleHandle,const word MaxVoltage,const word MinVoltage)
{
	THROTTLEMAP_Obj *throttle = (THROTTLEMAP_Obj *)ThrottleHandle;

	throttle->MaxVoltage = MaxVoltage;
	throttle->MinVoltage = MinVoltage;
	return;
}

void ThrottleMap_Process(THROTTLEMAP_Handle ThrottleHandle,const word Throttle_IO,const word ThrottleSingle1,\
		const word ThrottleSingle2, word *OutThrottle)
{
	THROTTLEMAP_Obj *throttle = (THROTTLEMAP_Obj *)ThrottleHandle;
  long temp = 0;
	throttle->State.bit.ErrorInf = Protection(throttle->MaxVoltage,throttle->MinVoltage,ThrottleSingle1,Throttle_IO,throttle->State.bit.CheckType);

	if(throttle->State.bit.ErrorInf == NORMAL)
	{
		throttle->PercentS1 = PercentCalc(throttle->S1_LowVoltage,throttle->S1_HighVoltage,throttle->DeltaS1HiLowVoltage,ThrottleSingle1);

		if(throttle->State.bit.CheckEnable == TRUE)
		{
			if(NORMAL == Protection(throttle->MaxVoltage,throttle->MinVoltage,ThrottleSingle2,Throttle_IO,throttle->State.bit.CheckType))
			{
				throttle->PercentS2 = PercentCalc(throttle->S2_LowVoltage,throttle->S2_HighVoltage,throttle->DeltaS2HiLowVoltage,ThrottleSingle2);

				switch (throttle->State.bit.CheckType)
				{	
				  case TYPE_0:
					  throttle->PercentS2 = throttle->PercentS1;
						break;
						
					case TYPE_1:	
						break;
						
					case TYPE_2:
						throttle->PercentS2 = HUNDREDPERCENT - throttle->PercentS2;
						break;
						
					default:
						throttle->PercentS2 = throttle->PercentS1;		//???
						break;
				}
        temp = throttle->PercentS1 - throttle->PercentS2;
        if(temp < 0) temp = -temp;
        
				if ((word)temp  > throttle->VerifyRange)
				{
					throttle->State.bit.ErrorInf = OVERVERIFYRANGE;
				}
			}
			else
			{
				throttle->State.bit.ErrorInf = THROTTLES2FAILED;
				throttle->DeltaS2 = DeltaCalc(throttle->MaxVoltage,throttle->MinVoltage,ThrottleSingle2);
			}
		}
		*OutThrottle = throttle->PercentS1;
	}
	else
	{
		throttle->State.bit.ErrorInf = THROTTLES1FAILED;
		throttle->DeltaS1 = DeltaCalc(throttle->MaxVoltage,throttle->MinVoltage,ThrottleSingle1);
		*OutThrottle = ZEROPERCENT;
	}

	throttle->State.bit.OldErrorInf = throttle->State.bit.ErrorInf;
	return;
} // end of ThrottleMap_Process() function

char ThrottleMap_getErrorInf(THROTTLEMAP_Handle ThrottleHandle)
{
	THROTTLEMAP_Obj *throttle = (THROTTLEMAP_Obj *)ThrottleHandle;

	return ((throttle->State.bit.ErrorInf) & 0xff);

} // end of ThrottleMap_getErrorInf() function

word ThrottleMap_getVreifyRange(THROTTLEMAP_Handle ThrottleHandle)
{   
  int temp;
  
	THROTTLEMAP_Obj *throttle = (THROTTLEMAP_Obj *)ThrottleHandle;
	
  temp = (throttle->PercentS1 - throttle->PercentS2);
  
  if(temp>=0)
   	return (temp);
	else
	 return (0-temp);

} // end of ThrottleMap_getVreifyRange() function

void ThrottleMap_getProtectDelta(THROTTLEMAP_Handle ThrottleHandle,word* DeltaS1,word* DeltaS2)
{
	THROTTLEMAP_Obj *throttle = (THROTTLEMAP_Obj *)ThrottleHandle;

	*DeltaS1 = throttle->DeltaS1;
	
	*DeltaS2 = throttle->DeltaS2;
	return ;

}// end of ThrottleMap_getProtectDelta() function

word PercentCalc(word x,word y,dword InvDelat,word z)
{  
  dword temp ;
	if(z <= x)
	{
		z = ZEROPERCENT;
	}
	else if(z >= y)
	{
		z = HUNDREDPERCENT;
	}
	else
	{
	  temp = (InvDelat * (z - x));
		z =  (word)(temp >>15);
	}

	return z;
}
word Protection(word a,word b,word c,word IO,word CheckType)
{   
  if(CheckType != 0) 
  { 
  	if(((c < b) &&(IO == 0))|((c <= a) && (c >= b) && (IO == 1)))
  	{
  		return NORMAL;
  	}
  	else
  	{
  	  return ERROR;
  	}
  } 
  else 
  {
    if((c <= a) && (c >= b))
  	{
  		return NORMAL;
  	}
  	else
  	{
  	  return ERROR;
  	}
  }
}
word  DeltaCalc(word a,word b,word c)
{
	if(c > a)
	{
		c = c - a;
	}
	else if(b > c)
	{
		c = b - c;
	}
	else
	{
		c = 0;
	}
	return c;
}
// end of file
