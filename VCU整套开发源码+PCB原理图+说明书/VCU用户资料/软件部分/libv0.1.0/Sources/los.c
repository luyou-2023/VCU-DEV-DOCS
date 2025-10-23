/*====================================================================================
* File name: 	 los.c
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
#include "los.h"


void Los_Coeff(LOS *los)
{
	static int los_times = sizeof(LOS_IQPARA)/2;
	int i = 0;
	int DeltaValue;
	
	word	InteralValue;
  int temp = 0;
  long temp1 = 0;

	los->Coeff = 32768;													// clear the summary
	
	for(i = 0;i < los_times;i++)
	{ 
	
		if((los->En.all) &  (1 << i) )
		{
		  temp = los->Upper.Los[i];
		  temp = temp - los->Down.Los[i];
		  
		  /*if(temp < 0) temp = 0 - temp;*/
		  
			InteralValue = (word)temp;  
			
			DeltaValue = los->Input.Los[i];
			DeltaValue = DeltaValue - los->Down.Los[i];
			
			if(DeltaValue <0 )  DeltaValue = 0;
			
			if(DeltaValue > InteralValue) 
			{
			  (word)DeltaValue = InteralValue; 
			}
			
			temp1 = (long)DeltaValue <<15;
			
			temp1 = temp1 / InteralValue;
			
			los->Output.Los[i] = (word)temp1;

			if((los->Cmp.all) & (1 << i))										//0-limit down  1-limit up
			{
				los->Output.Los[i] = (32768 - los->Output.Los[i]);
			}

		/*	if(los->Output.Los[i] < 0)
			{
				los->Output.Los[i] = 0;
			}
			else if(los->Output.Los[i] > 32768)
			{
				los->Output.Los[i] = 32768;
			}  */

			if(los->Output.Los[i] < los->Coeff)
			{
				los->Coeff = los->Output.Los[i];
			}
		}
		else
		{
			los->Output.Los[i] = 32768;
		}
	}
	if(los->Coeff > (los->CoeffOld + los->CoeffStep))						//add step when
	{
		los->Coeff = los->CoeffOld + los->CoeffStep;
	}
	los->CoeffOld = los->Coeff;
}
