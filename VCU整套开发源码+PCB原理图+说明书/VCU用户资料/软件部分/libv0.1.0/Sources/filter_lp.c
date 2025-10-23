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

#include <hidef.h>           /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "filter_lp.h"

 void Filter_lp_calc(FILTERLP *p)
{
	long FilterTemp;
  long FilterTemp1;
  
  // y(k)=Ts/(Rc+Ts)*X(K)+(1-Ts/(Rc+Ts))*y(k-1); u=Ts/(Rc+Ts),fc=1/(2*pi*rc)
  
  //y_out(k+1) = alpha*input(k+1)+(1-alpha)*y_out(k)
  
	FilterTemp = p->K2; 
	FilterTemp = 	FilterTemp * p->input;
  FilterTemp = FilterTemp >> 15;
  
	/* Low-pass filter */
	
  p->K3 = (word)32767 - p->K2;     
	FilterTemp1 = p->output;
	FilterTemp1 =	FilterTemp1 * p->K3;
  FilterTemp1 = FilterTemp1 >> 15;
  
	/* Saturate the output */
	p->output =(int) (FilterTemp + FilterTemp1);
	
}

