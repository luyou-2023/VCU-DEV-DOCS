/*=====================================================================================
 File name:   RefMap.c  (IQ version)
 Description:    
 Created on:  01-12-2014
 Author:      JT
=====================================================================================
 History:
-------------------------------------------------------------------------------------
01-12-2014	 Version 0.1.0   JT		Initialized version   
-------------------------------------------------------------------------------------*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "refmap.h"
 
 
REFMAP_Handle RefMap_init(void *pMemory,const size_t numBytes)
{

	REFMAP_Handle RefMapHandle;
	REFMAP_Obj *RefMap;
   long temp ;
	 if(numBytes < sizeof(REFMAP_Obj))
		return((REFMAP_Handle)NULL);

	 // assign the handle
	 RefMapHandle = (REFMAP_Handle)pMemory;

	 // assign the object
	 RefMap = (REFMAP_Obj*)RefMapHandle;
 	 memset(RefMap,0,sizeof(REFMAP_Obj));
	 
	 RefMap->MidVolPercent = MIDVOLPRECENR;
	 
   temp = MAXVOLPRECENR;
   temp = temp << 15;
   temp = temp / RefMap->MidVolPercent;
    
	 RefMap->ratio1 = (word) temp;
	 
   temp = MAXVOLPRECENR;
   temp = temp << 15;
    temp = temp / RefMap->MidVolPercent;
    
	 RefMap->ratio2 = (word) temp;
	 
	 return(RefMapHandle);

}// end of RefMap_init() function


void RefMap_set(REFMAP_Handle RefMapHandle,const word MidVol_RefPercent,const word Offset_RefPercent)
{
	REFMAP_Obj *RefMap = (REFMAP_Obj *)RefMapHandle;
  
  long temp = 0;
 
 	//if(RefMap->MidVol_RefPercent != MidVol_RefPercent)
	{
    temp =	(MidVol_RefPercent - Offset_RefPercent) ;
    temp = temp * RefMap->ratio1;
	
		RefMap->ratio_K1 = (word)(temp >>15);
		
		RefMap->ratio_b1 = Offset_RefPercent;
		
		temp = (MAXVOLPRECENR-MidVol_RefPercent); 
	 temp = temp * RefMap->ratio2;
		
		RefMap->ratio_K2 = (word)(temp >>15);

		RefMap->ratio_b2 = MAXVOLPRECENR - RefMap->ratio_K2;

		RefMap->MidVol_RefPercent = MidVol_RefPercent;
	}
	return;
}// end of RefMap_setPara() function

void RefMap_Process(REFMAP_Handle RefMapHandle,const word VoltagePercent,word *OutRefPercent)
{
	REFMAP_Obj *RefMap = (REFMAP_Obj *)RefMapHandle;
	
  long temp = 0;
    temp = VoltagePercent; 
    
	if(VoltagePercent < RefMap->MidVolPercent)
	{
	   temp = temp * RefMap->ratio_K1;
	  
		*OutRefPercent = (word)((temp >> 15) + RefMap->ratio_b1);
	}
	else if(VoltagePercent > RefMap->MidVolPercent)
	{
	    temp = temp * RefMap->ratio_K2;
	    temp = temp>>15;
	    
		*OutRefPercent = (word)(temp + RefMap->ratio_b2);
	}
	else
	{
		*OutRefPercent = RefMap->MidVol_RefPercent;
	}
	return;
}// end of RefMap_Process() function




