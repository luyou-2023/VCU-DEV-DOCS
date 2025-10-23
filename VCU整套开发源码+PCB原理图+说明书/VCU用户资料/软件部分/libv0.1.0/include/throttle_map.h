/*=====================================================================================
File name:   ThrottleMap.h   
Description: Throttle process and  check
Created on:  01-12-2014
Author: JT
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 2013-5-29	Version 0.1.0    JT		Initialized version   
-------------------------------------------------------------------------------------*/
#ifndef _THROTTLE_MAP_H_
#define _THROTTLE_MAP_H_

// **************************************************************************
// the includes
#include "string.h"

// **************************************************************************
// the defines

#ifndef TRUE
#define TRUE  1
#define FLASE 0
#endif

// Error information
#define NORMAL              0
#define THROTTLES1FAILED   	1
#define THROTTLES2FAILED   	2
#define OVERVERIFYRANGE     3
#define ERROR				8
// Throttle Check type
#define TYPE_0      0  // 单油门
#define TYPE_1      1  // _IQabs(S1-S2)双油门 都是0-5V 变化
#define TYPE_2      2	 // _IQabs(5-s1-s2)双油门一路0-5V变化 一路5V-0变化

#define ZEROPERCENT 0
#define HUNDREDPERCENT  32767
// **************************************************************************
// the typedefs

//! \brief Defines the Throttle Map object
//!
struct THROTTLESTATE_BITS{
	word CheckEnable:1;	// 0:disable throttle check ,1:enable throttle
	word CheckType:3;		// Type of check tow throttle signal
	word ErrorInf:4;				// Error information of throttle,0:normal;1: out range;2:S1 failed ;3:S2 failed
	word OldErrorInf:4;           // Last error information
	word revesed:4;
};

union THROTTLESET_UNION {
	word		 all;
	struct	THROTTLESTATE_BITS bit;
};

typedef struct {
  word VerifyRange;			// Range of for check throttle eg, _IQabs(S1-2S2) ,_IQabs(S1+S2-MaxVoltage)

	word S1_HighVoltage;			// the voltage of S1 for 100% throttle ,IQ15
	word S1_LowVoltage;			// the voltage of S1 for 0% throttle,IQ15

	word S2_HighVoltage;			//  the voltage of S2 for 100% throttle ,IQ15
	word S2_LowVoltage;			//  the voltage of S2 for 10% throttle,IQ15

	dword DeltaS1HiLowVoltage;	// throttle signal1, 1/(high voltage - low voltage),IQ15
	dword DeltaS2HiLowVoltage;	// throttle signal2, 1/(high voltage - low voltage),IQ15

	word PercentS1;		// throttle signal(S1) percent ,IQ15
	word PercentS2;		// throttle  signal(S2) percent ,IQ15

	word MaxVoltage;		// the Max voltage of Throttle Signal 1 for Protection ,over it throttle failed
	word MinVoltage;    	// the Min voltage of Throttle Signal 1 for Protection ,Under it throttle failed

	word DeltaS1;				// throttle signal(S1) Protect Voltage Delta ,IQ15
	word DeltaS2;				// throttle signal(S2) Protect Voltage Delta ,IQ15
	
	union THROTTLESET_UNION State; 	// throttle state
 }THROTTLEMAP_Obj;

//! \brief Defines the Throttle Map handle
//!
typedef THROTTLEMAP_Obj *THROTTLEMAP_Handle;

// **************************************************************************
// the function prototypes

//! \brief     Initializes THROTTLEMAP_Obj
//! \param[in] pMemory   A pointer to the memory for THROTTLEMAP_Obj
//! \param[in] numBytes  The number of bytes allocated for THROTTLEMAP_Obj, bytes
//! \return THROTTLEMAP_Handle
extern THROTTLEMAP_Handle ThrottleMap_init(void *pMemory,const size_t numBytes);

//! \brief     Sets throttle check
//! \param[in] ThrottleHandle  The THROTTLEMAP handle
//! \param[in] CHeckEnable
//! \param[in] CheckType
//! \param[in] VerifyRange
extern void ThrottleMap_setCheck(THROTTLEMAP_Handle ThrottleHandle,const word CHeckEnable,const word CheckType,const word VerifyRange);

//! \brief     Sets throttle voltage
//! \param[in] ThrottleHandle  The THROTTLEMAP handle
//! \param[in] S1_HightVoltage      the voltage of S1 for 100% throttle ,IQ24
//! \param[in] S1_LowVoltage        the voltage of S1 for 0% throttle ,IQ24
//! \param[in] S2_HightVoltage      the voltage of S2 for 100% throttle ,IQ24
//! \param[in] S2_LowVoltage        the voltage of S2 for 0% throttle ,IQ24
extern void ThrottleMap_setVoltage(THROTTLEMAP_Handle ThrottleHandle,const word S1_HighVoltage,const word S1_LowVoltage,const word S2_HighVoltage,const word S2_LowVoltage);

//! \brief     Sets throttle protect
//! \param[in] ThrottleHandle	The THROTTLEMAP handle
//! \param[in] MaxVoltage	 	throttle protect maxvoltage
//! \param[in] MinVoltage     	throttle protect minvoltage
extern void ThrottleMap_setProtect(THROTTLEMAP_Handle ThrottleHandle,const word MaxVoltage,const word MinVoltage);

//! \brief     Process throttle and check
//! \param[in] ThrottleHandle  The THROTTLEMAP handle
//! \param[in] S1_HightVoltage      the voltage of throttle S1,IQ24
//! \param[in] S2_HightVoltage      the voltage of throttle S2,IQ24
//! \param[in] OutThrottle          The pointer to the output data ,IQ24
extern void ThrottleMap_Process(THROTTLEMAP_Handle ThrottleHandle,const word Throttle_IO,const word ThrottleSingle1,const word ThrottleSingle2, word *OutThrottle);

//! \brief     get throttle eerror information
//! \param[in] pidHandle  The THROTTLEMAP handle
//! \return ErrorInf
extern char ThrottleMap_getErrorInf(THROTTLEMAP_Handle ThrottleHandle);

//! \brief     get throttle verify range
//! \param[in] pidHandle  The THROTTLEMAP handle
//! \return Present VreifyRange
extern word ThrottleMap_getVreifyRange(THROTTLEMAP_Handle ThrottleHandle);

//! \brief     get throttle protect voltage delta
//! \param[in] pidHandle  The THROTTLEMAP handle
//! \param[in] DeltaS1,DeltaS2         The pointer to the output data ,IQ24
extern void ThrottleMap_getProtectDelta(THROTTLEMAP_Handle ThrottleHandle,word* DeltaS1,word* DeltaS2);

#endif /* THROTTLEMAP_H_ */
