/*====================================================================================
* File name: 	 MC9S12XEP100EVICE.h
* Description:  
* Created on:  01-12-2014
* Author:  JT
=======================================================================================
 History:
---------------------------------------------------------------------------------------
01-12-2014		Version 1.0.0       JT		Initialized version
-------------------------------------------------------------------------------------*/ 

#ifndef _MC9S12XEP100EVICE_H
#define _MC9S12XEP100EVICE_H

#define  BUS_CLOCK		   40000000	   //总线频率
#define  OSC_CLOCK		   16000000	   //晶振频率

//OUTPUT
#define CHARGE_CONTACTOR              PORTA_PA0
#define CHARGE_CONTACTOR_DIR          DDRA_DDRA0

#define VACUUM_PUMP         PORTA_PA1
#define VACUUM_PUMP_DIR     DDRA_DDRA1

#define MCU_PRE_CHARGE      PORTA_PA2
#define MCU_PRE_CHARGE_DIR  DDRA_DDRA2

#define MCU_CONTACTOR       PORTA_PA3
#define MCU_CONTACTOR_DIR   DDRA_DDRA3

#define FAN_OUT          PORTA_PA4
#define FAN_OUT_DIR      DDRA_DDRA4

#define DCDC_OUT        PORTA_PA5
#define DCDC_IO_DIR     DDRA_DDRA5

#define PTC_OUT         PORTA_PA6
#define PTC_IO_DIR      DDRA_DDRA6

#define AIR_CONDITIONER     PORTA_PA7
#define AIR_CONDITIONER_DIR DDRA_DDRA7

//INPUT

#define THRO_IO         PORTB_PB0
#define THRO_IO_DIR     DDRB_DDRB0

#define R               PORTB_PB1
#define R_DIR           DDRB_DDRB1

#define ON              PORTB_PB2
#define ON_DIR          DDRB_DDRB2

#define D               PORTB_PB3
#define D_DIR           DDRB_DDRB3

#define START           PORTB_PB4
#define START_DIR       DDRB_DDRB4

#define AC_SWITCH       PORTB_PB5
#define AC_SWITCH_DIR   DDRB_DDRB5

#define PTC_SWITCH      PORTB_PB6
#define PTC_SWITCH_DIR  DDRB_DDRB6

#define BRAKE_IO        PORTB_PB7
#define BRAKE_IO_DIR    DDRB_DDRB7 
                 
#define AIRHIGH         PORTK_PK0
#define AIRHIGH_DIR     DDRK_DDRK0

#define AIRLOW          PORTK_PK1
#define AIRLOW_DIR      DDRK_DDRK1

#define CHARGE_SWITCH       PORTK_PK2
#define CHARGE_SWITCH_DIR   DDRK_DDRK2

#define BACKUP_IO       PORTK_PK3
#define BACKUP_IO_DIR   DDRK_DDRK3
                

#define LED       PORTK_PK4
#define LED_DIR   DDRK_DDRK4

void System_Init(void);
void Init_WatchDog(void);

#include "ATD.h"
#include "sci.h"
#include "pit.h"
#include "can.h"
#endif