/*====================================================================================
* File name: 	 checkio.c
* Description:  
* Created on:  01-15-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-15-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "checkio.h"  	// Include header of average.c
 
word Check_IO(word IO,int* pdata,word CountNum) 
 { 
     
     if(IO == 1) 
     {
        (*pdata)++;
        
        if((*pdata) >= CountNum)
        {
          (*pdata) = CountNum; 
           return (1);
        } 
        else 
        {
           return (0);
        }
     } 
     else
     {
        (*pdata)--;
        if((*pdata) <= 1) 
        {
          (*pdata) = 1;
          
          return (0);
        } 
        else
        { 
          return (1);
        }
     }
 }