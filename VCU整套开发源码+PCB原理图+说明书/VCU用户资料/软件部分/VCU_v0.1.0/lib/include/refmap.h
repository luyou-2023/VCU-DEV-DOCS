/*=====================================================================================
 File name:   RefMap.h   
 Description:    
 Created on:  01-12-2014
 Author:      JT
=====================================================================================
 History:
-------------------------------------------------------------------------------------
01-12-2014	 Version 0.1.0   JT		Initialized version   
-------------------------------------------------------------------------------------*/
#ifndef REF_MAP_H_
#define REF_MAP_H_

// **************************************************************************
// the includes
#include "string.h"
// **************************************************************************
// the defines
#define MAXVOLPRECENR 	32767
#define MIDVOLPRECENR  	16384
#define MINVOLPRECENR 	0

// **************************************************************************
// the typedefs

//! \brief Defines the Ref Map object
//!

typedef struct _REF_MAP_Obj_
{
	word MidVolPercent;  	// the middle voltage percent
	word MidVol_RefPercent;	// the ref percent for middle voltage percent
	word ratio1;				// 1/(MidVolPercent-0%)
	word ratio2;     		// 1/(100%-MidVolPercent)
	word ratio_K1;			// k1 = (MidVol_RefPercent-0%)/(MidVolPercent-0%)
  word ratio_b1;	
	word ratio_K2;			// k2 =  (100%-MidVol_RefPercent)/(100%-MidVolPercent)
	word ratio_b2;			// b = 100% - K2

}REFMAP_Obj;

//! \brief Defines the RefMap handle
//!
typedef struct REFMAP_Obj *REFMAP_Handle;

// **************************************************************************
// the function prototypes

//! \brief     Initializes REFMAP_Obj
//! \param[in] pMemory   A pointer to the memory for THROTTLEMAP_Obj
//! \param[in] numBytes  The number of bytes allocated for THROTTLEMAP_Obj, bytes
//! \return REFMAP_Handle
extern REFMAP_Handle RefMap_init(void *pMemory,const size_t numBytes);

//! \brief     RefMap_setPara
//! \param[in] RefMapHandle  the REFMAP_Handle
//! \param[in] MidVol_RefPercent  the ref percent for  middle voltage percent
//! \return THROTTLEMAP_Handle
extern void RefMap_set(REFMAP_Handle RefMapHandle,const word MidVol_RefPercent,const word Offset_RefPercent);

//! \brief     RefMap_setPara
//! \param[in] RefMapHandle  the REFMAP_Handle
//! \param[in] VoltagePercent  the input voltage percent
//! \param[in] OutRefPercent   A pointer to output ref percent
//! \param[in] MidVol_RefPercent  the ref percent for  middle voltage percent
extern void RefMap_Process(REFMAP_Handle RefMapHandle,const word VoltagePercent, word *OutRefPercent);

#endif /* REF_MAP_H_ */
