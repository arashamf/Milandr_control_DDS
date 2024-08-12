#ifndef __DDS1367_H__
#define __DDS1367_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes -----------------------------------------------------------------------------------//
#include "main.h"
#include "typedef.h"

// Private defines-----------------------------------------------------------------------------//

//Exported types ------------------------------------------------------------------------------//

// Function prototypes -----------------------------------------------------------------------//
void registr_activated (uint8_t );
void dds_put_msg (uint8_t , uint64_t );
void dds_init_registr0 (void);
void dds_init_registr10(MONO_SIGNAL_t * );
void dds_init_registr12(void);
void dds_init_registr14 (MONO_SIGNAL_t * );
void dds_init_registr (uint8_t , uint64_t );
//Macro--------------------------------------------------------------------------------------//

//VARIABLES----------------------------------------------------------------------------------//

#ifdef __cplusplus
}
#endif
#endif 


