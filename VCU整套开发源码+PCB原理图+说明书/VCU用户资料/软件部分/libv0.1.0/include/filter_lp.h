/*====================================================================================
* File name: 	 filter_lowpass.c
* Description: vehicle related.  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 
#ifndef __FILTERLP_H__
#define __FILTERLP_H__

typedef struct {
	   int 		input;  		  // Input(pu)
     int 		output;      	// Output(pu)
     word 		K2;     		// Parameter: Constant (pu)
     word 		K3;     		// Parameter: Constant (pu)
             } FILTERLP;

typedef FILTERLP *FILTERLP_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the FILTERLP object.
-----------------------------------------------------------------------------*/                     
#define FILTERLP_DEFAULTS   {	0, \
								0, \
								0, \
								0, \
								}

/*-------------------------------------------
Prototypes for the functions in FILTERLP.C
-------------------------------------------*/
void Filter_lp_calc(FILTERLP_handle);

#endif // __FILTERLP_H__
