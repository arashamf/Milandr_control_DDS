#ifndef __TYPEDEF_H
#define __TYPEDEF_H

// Includes -----------------------------------------------------------------------//
#include "MDR32F9Qx_config.h"

// Defines-----------------------------------------------------------------//
#define 		DIV36 		(0x1000000000)
//#define 		FOUT1			(5317460UL)
#define 		FOUT1			(4000000UL)
//#define 		FOUT2			(5317461UL)
#define 		FOUT2			(2000000UL)
#define 		FCLK			(30000000UL)
#define 		FSC				(64UL)
// Exported types -----------------------------------------------------------------//
#pragma anon_unions(1)
#pragma pack(1)

//---------------------------------------------------------------------------------//
typedef enum {UART_RX_MODE = 0, UART_TX_MODE = 1} UartMode; //ðåæèì ÓÀÐÒà

//---------------------------------------------------------------------------------//
typedef enum {	addr0 = 0, addr10 = 0x10, addr11, addr12,	addr14, addr15, addr16, 
addr18, addr19, addr1A, addr1C, addr1D, addr1E}  number_registr;

//---------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		union
		{
			struct
			{
				uint8_t reserve4					: 1;
				uint8_t reserve3					: 8;
				uint8_t reserve22					: 3;	
				uint8_t DAC_reserve				: 4;
				uint8_t REF_on						: 1;
				uint8_t DAC_cotrol				: 7;
				uint8_t reserve21					: 1;
				uint8_t reserve1					: 8;	
				uint8_t reserve0					: 2;	
				uint8_t enable_dithering	: 1;
				uint8_t address						:	5;	
			}	registr_common;
			
			struct
			{
				uint8_t inc4					:	8;
				uint16_t inc3 				:	16;
				uint16_t inc2 				:	16;
				uint8_t inc1 					:	3;
				uint8_t address 			:	5;
			} registr_10;	
			
			struct
			{			
				uint8_t address 						:	5;
				uint8_t freq_offset1				:	8;
				uint8_t freq_offset2				:	8;
				uint8_t freq_offset3				:	8;
				uint8_t freq_offset4				:	8;
				uint8_t freq_offset5				:	4;
			} registr_11;
			
			struct
			{
				uint8_t address 					:	5;
				uint8_t phase_shift1 			:	8;
				uint8_t phase_shift2 			:	6;
				uint8_t LCHM 							:	1;
				uint8_t direct 						:	1;
				uint8_t reserve1 					:	8;
				uint8_t reserve2					:	8;
				uint8_t reserve3					:	4;
			} registr_12;	
		};	
	};

	uint8_t RAW_data[6];

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
