#ifndef __TYPEDEF_H
#define __TYPEDEF_H

// Includes -----------------------------------------------------------------------//
#include "MDR32F9Qx_config.h"

// Defines-----------------------------------------------------------------//
#define 		DIV36 		(0x1000000000)
#define 		FOUT1			(5000000UL)
#define 		FOUT2			(5000000UL)
#define 		FCLK			(30000000UL)
#define 		FSC				(100UL)
// Exported types -----------------------------------------------------------------//
#pragma anon_unions(1)
#pragma pack(1) 
//---------------------------------------------------------------------------------//
typedef enum {UART_RX_MODE = 0, UART_TX_MODE = 1} UartMode; //режим УАРТа

//---------------------------------------------------------------------------------//
typedef enum {	addr0 = 0, 
								addr10 = 0x10, 
								addr11, 
								addr12,	
								addr14=0x14, 
								addr15, 
								addr16, 
								addr18=0x18,
								addr19, 
								addr1A, 
								addr1C=0x1C,
								addr1D, 
								addr1E}  number_registr;

//---------------------------------------------------------------------------------//
#pragma pack(1) 
typedef union
{
	struct
	{
		union
		{
			struct
			{	
				uint8_t enable_dithering	: 1; //младший
				uint16_t empty3						: 7;				
				uint16_t empty2						: 4;
				uint8_t DAC_cotrol_lsb		: 4;
				uint8_t DAC_cotrol_msb		:	3;
				uint8_t REF_on						: 1;
				uint8_t empty1						: 4;
				uint16_t empty0						: 12;	//старший			
			}	registr_common;
			
			struct
			{
				uint8_t inc4					:	1;
				uint16_t inc3 				:	16;
				uint16_t inc2 				:	16;
				uint8_t inc1 					:	3;
			} registr_10;	
			
		/*	struct
			{			
				uint8_t freq_offset1				:	8;
				uint8_t freq_offset2				:	8;
				uint8_t freq_offset3				:	8;
				uint8_t freq_offset4				:	8;
				uint8_t freq_offset5				:	4;
			} registr_11;*/
			
			struct
			{
				uint16_t phase_shift1 		:	14;
				uint8_t LCHM 							:	1;
				uint8_t direct 						:	1;
				uint8_t reserve1 					:	8;
				uint8_t reserve2					:	8;
				uint8_t reserve3					:	4;
			} registr_12;	
		};	
		
	};
	uint64_t RAW_data;
}MSG_CTRL_DDS_t;

//----------------------------------------------------------------
typedef struct
{
	uint64_t divider;
	uint64_t f_clk;
	uint64_t f_out1;
	uint64_t f_out2;
	uint64_t f_out3;
	uint64_t f_out4;
	uint64_t increment1;	
	uint64_t increment2;	
	uint64_t increment3;	
	uint64_t increment4;	
} MONO_SIGNAL_t;

#pragma pack()
#pragma anon_unions()

//Private defines ------------------------------------------------------------------//
/* DEBUG information */
#ifdef __USE_DBG
#define DBG(...)  printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

//Constants ----------------------------------------------------------------------//


#endif
