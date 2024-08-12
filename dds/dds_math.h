#ifndef __DDS_MATH_H__
#define __DDS_MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes -----------------------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"

//Private defines-----------------------------------------------------------------------------//

//Exported types -----------------------------------------------------------------------------//

//Function prototypes ------------------------------------------------------------------------//
void monosignal_init (MONO_SIGNAL_t * );
void monosignal_calc (MONO_SIGNAL_t * , MSG_CTRL_DDS_t * );

//Macro--------------------------------------------------------------------------------------//

//Variables----------------------------------------------------------------------------------//

#ifdef __cplusplus
}
#endif
#endif 


